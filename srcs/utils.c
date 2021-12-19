/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 19:25:10 by mterkhoy          #+#    #+#             */
/*   Updated: 2021/12/19 20:54:47 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

void	free_data(t_data *data)
{
	int	i;
	int	j;

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
