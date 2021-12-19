/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 09:50:18 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/19 18:05:28 by mterkhoy         ###   ########.fr       */
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

int	valid_in(t_data *data)
{
	return (access(data->in, F_OK | R_OK) == 0);
}

int	valid_out(t_data *data)
{
	if (access(data->out, F_OK) == 0 && (access(data->out, W_OK) == -1 
											|| is_dir(data->out)))
		return (0);
	return (1);
}

int	init_data(t_data *data, char *argv[], char *envp[])
{
	data->envp = envp;
	data->in = argv[1];
	data->out = argv[4];
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
	
	if (valid_in(data))
	{
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
	}
	else
	{
		perror("bash");
	}
	if (valid_out(data))
	{
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
	}
	else
	{
		perror("bash");
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
	if (!init_data(&data, argv, envp))
	{
		perror("error");
		return (-1);
	}
	execute(&data);
	free_data(&data);
	return 0;
}
