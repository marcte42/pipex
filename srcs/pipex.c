/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 09:50:18 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/19 17:41:32 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec_path(t_data *data, char *argv[])
{
	(void) data;
	char	**paths;
	char	*path_to_bin;
	char	*tmp;
	int		i;

	if (open(argv[0], O_RDONLY) > 0)
		execve(argv[0], argv, NULL);
	paths = ft_split(getenv("PATH"), ':');
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path_to_bin = ft_strjoin(tmp, argv[0]);
		free(tmp);
		if (open(path_to_bin, O_RDONLY) > 0)
			execve(path_to_bin, argv, NULL);
		free(path_to_bin);
	}
	write (1, "Command not found\n", 18);
	exit(127);
}

int	is_dir(char *path)
{
	int fd;

	errno = 0;
	fd = open(path, O_WRONLY);
	if (errno == EISDIR)
		return (1);
	if (fd > 0)
		close(fd);
	return (0);
}

int	get_redirects(t_data *data, char *argv[])
{
	data->in = argv[1];
	data->out = argv[4];
	if (access(data->out, F_OK) == 0 && (access(data->out, W_OK) == -1 
											|| is_dir(data->out)))
		return (0);
	return (access(data->in, F_OK | R_OK) == 0);
}

int	get_commands(t_data *data, char *argv[])
{
	data->cmds = calloc(3, sizeof(char *));
	data->cmds[0] = ft_split(argv[2], ' ');
	data->cmds[1] = ft_split(argv[3], ' ');
	data->cmds[2] = NULL;
	return (1);
}

int	execute(t_data *data)
{
	int pipefd[2];
	int pid;
	int fd;

	pipe(pipefd);
	
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		fd = open(data->in, O_RDONLY);
		if (fd < 0)
			perror("error");
		dup2(fd, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		exec_path(data, data->cmds[0]);
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[1]);
		fd = open(data->out, O_WRONLY | O_CREAT, 0666);
		if (fd < 0)
			perror("error");
		dup2(fd, STDOUT_FILENO);
		dup2(pipefd[0], STDIN_FILENO);
		exec_path(data, data->cmds[1]);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	wait(NULL);
	wait(NULL);
	return (0);
}

void	free_data(t_data *data)
{
	int	i;
	int j;

	i = -1;
	while (data->cmds[++i])
	{
		j = -1;
		while (data->cmds[i][++j])
		{
			free(data->cmds[i][j]);
		}
		free(data->cmds[i]);
	}
	free(data->cmds);
}

int main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	
	if (argc != 5)
	{
		ft_putstr("Error : invalid number of arguments\n");
		return (1);
	}
	if (!get_redirects(&data, argv))
	{
		perror("bash");
		return (-1);
	}
	if (!get_commands(&data, argv))
	{
		perror("error");
		return (-1);
	}
	data.envp = envp;
	execute(&data);
	free_data(&data);
	return 0;
}
