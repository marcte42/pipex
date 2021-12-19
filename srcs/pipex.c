/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 09:50:18 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/19 19:37:27 by mterkhoy         ###   ########.fr       */
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

int exec_left(t_data *data, int *pid, int *pipefd)
{
	int fd;
	
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
			perror("bash");
			return (127);
		}
	}
	else
	{
		perror("bash");
		return (1);
	}
	return (0);
}

int exec_right(t_data *data, int *pid, int *pipefd)
{
	int fd;
	
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
			perror("bash");
			return (127);
		}
	}
	else
	{
		perror("bash");
		return (1);
	}
	return (0);
}

int	execute(t_data *data)
{
	int pipefd[2];
	int pid[2];
	int status[2];

	pipe(pipefd);
	
	status[0] = exec_left(data, pid, pipefd);
	status[1] = exec_right(data, pid, pipefd);
	
	close(pipefd[0]);
	close(pipefd[1]);
	wait(&status[0]);
	wait(&status[1]);
	return (status[1]);
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
