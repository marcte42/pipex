/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 09:50:18 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/20 13:51:42 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec_path(t_data *data, char *argv[])
{
	char	**paths;
	char	*path_to_bin;
	char	*tmp;
	int		i;

	(void) data;
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
			execve(path_to_bin, argv, data->envp);
		free(path_to_bin);
	}
}

int	exec_left(t_data *data, int *pid, int *pipefd)
{
	int	fd;

	fd = valid_in(data);
	if (fd <= 0)
	{
		perror("bash");
		return (1);
	}
	if (!valid_cmd(data, data->cmds[0][0]))
	{
		ft_putstr("bash: Command not found\n");
		return (127);
	}
	pid[0] = fork();
	if (pid[0] == -1)
		return (errno);
	if (pid[0] == 0)
	{
		close(pipefd[0]);
		dup2(fd, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		exec_path(data, data->cmds[0]);
	}
	return (0);
}

int	exec_right(t_data *data, int *pid, int *pipefd)
{
	int	fd;

	fd = valid_out(data);
	if (fd <= 0)
	{
		perror("bash");
		return (1);
	}
	if (!valid_cmd(data, data->cmds[1][0]))
	{
		ft_putstr("bash: Command not found\n");
		return (127);
	}
	pid[1] = fork();
	if (pid[1] == -1)
		return (errno);
	if (pid[1] == 0)
	{
		close(pipefd[1]);
		dup2(fd, STDOUT_FILENO);
		dup2(pipefd[0], STDIN_FILENO);
		exec_path(data, data->cmds[1]);
	}
	return (0);
}

int	execute(t_data *data)
{
	int	pipefd[2];
	int	pid[2];
	int	status[2];

	if (pipe(pipefd) == -1)
		return (errno);
	status[0] = exec_left(data, pid, pipefd);
	status[1] = exec_right(data, pid, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	return (status[1]);
}

int	main(int argc, char *argv[], char *envp[])
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
