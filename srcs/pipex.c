/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 09:50:18 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/19 12:24:36 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	get_redirects(t_data *data, const char **argv)
{
	int in;
	int out;

	data->in = argv[1];
	data->out = argv[4];
	in = access(data->in, F_OK | R_OK) == 0 && open(data->in, O_RDWR) != -1;
	out = access(data->out, F_OK | W_OK) == 0 && open(data->out, O_RDWR) != -1;
	return (in && out);
}

int	get_commands(t_data *data, int argc, const char **argv)
{
	int i;

	data->cmds = calloc(5, sizeof(char *));
	i = 0;
	while (++i < argc)
	{
		data->cmds[i - 1] = ft_split(argv[i], ' ');
		if (data->cmds[i - 1] == NULL)
			return (0);
	}
	return (1);
}

int	execute(t_data *data)
{
	int pipedes[2];
	int pid;
	int i;
	int fd;

	pipe(pipedes);
	i = -1;
	while (++i < 2)
	{
		pid = fork();
		if (pid == 0)
		{
			if (i == 0)
			{
				fd = open(data->in, O_WRONLY);
				dup2(fd, STDIN_FILENO);
			}
			else
			{
				fd = open(data->out, O_WRONLY | O_TRUNC | O_CREAT);
				dup2(fd, STDOUT_FILENO);
			}
			close(pipedes[0]);
			close(pipedes[1]);
			execve("/bin/cat", data->cmds[i], NULL);
		}
		close(pipedes[0]);
		close(pipedes[1]);
		wait(NULL);
	}
	

	return (0);
}

int main(int argc, char const *argv[])
{
	t_data	data;
	
	if (argc != 5)
	{
		ft_putstr("Error : invalid number of arguments\n");
		return (1);
	}
	if (!get_redirects(&data, argv))
	{
		perror("error");
		return (-1);
	}
	if (!get_commands(&data, argc, argv))
	{
		perror("error");
		return (-1);
	}
	execute(&data);
	return 0;
}
