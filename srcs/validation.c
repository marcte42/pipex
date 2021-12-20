/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 19:24:15 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/20 15:59:38 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	is_dir(char *path)
{
	int	fd;

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
	char	**paths;
	char	*path_to_bin;
	char	*tmp;
	int		i;

	(void) data;
	if (open(cmd, O_RDONLY) > 0)
		return (1);
	paths = ft_split(getenv("PATH"), ':');
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path_to_bin = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path_to_bin, F_OK | X_OK) == 0)
		{
			free(path_to_bin);
			free_split(paths);
			return (1);
		}
		free(path_to_bin);
	}
	free_split(paths);
	return (0);
}
