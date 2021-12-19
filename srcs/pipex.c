/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 09:50:18 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/19 19:11:08 by mterkhoy         ###   ########.fr       */
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
	if (access(data->in, F_OK | R_OK) == -1)
		return (-1);
	return (open(data->in, O_RDONLY));
}

int	valid_out(t_data *data)
{
	if (access(data->out, F_OK) == 0 && (access(data->out, W_OK) == -1 
											|| is_dir(data->out)))
		return (-1);
	return (open(data->out, O_WRONLY | O_CREAT | O_TRUNC, 0666));
}

int	valid_cmd(t_data *data, char *cmd)
{
	(void) data;
	char	**paths;
	char	*path_to_bin;
	char	*tmp;
	int		i;

	if (open(cmd, O_RDONLY) > 0)
		return (1);
	paths = ft_split(getenv("PATH"), ':');
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path_to_bin = ft_strjoin(tmp, cmd);
		free(tmp);
		if (open(path_to_bin, O_RDONLY) > 0)
			return (1);
		free(path_to_bin);
	}
	return (0);
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
	int pid[2];
	int status[2];
	int fd;

	pipe(pipefd);
	
	if ((fd = valid_in(data)) > 0)
	{
		if (valid_cmd(data, data->cmds[0][0]))
		{
			pid[0] = fork();
			if (pid[0] == 0)
			{
				close(pipefd[0]);
				dup2(fd, STDIN_FILENO);
				dup2(pipefd[1], STDOUT_FILENO);
				exec_path(data, data->cmds[0]);
			}
		}
		else
		{
			status[0] = 127;
			perror("bash : 1");
		}
	}
	else
	{
		status[0] = 1;
		perror("bash");
	}
	if ((fd = valid_out(data)) > 0)
	{
		if (valid_cmd(data, data->cmds[1][0]))
		{
			pid[1] = fork();
			if (pid[1] == 0)
			{
				close(pipefd[1]);
				dup2(fd, STDOUT_FILENO);
				dup2(pipefd[0], STDIN_FILENO);
				exec_path(data, data->cmds[1]);
			}
		}
		else
		{
			status[1] = 127;
			perror("bash : 2");
		}
	}
	else
	{
		status[1] = 1;
		perror("bash");
	}
	close(pipefd[0]);
	close(pipefd[1]);
	wait(&status[0]);
	wait(&status[1]);
	return (status[1]);
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
	int		status;
	
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
	status = execute(&data);
	free_data(&data);
	return (status);
}
