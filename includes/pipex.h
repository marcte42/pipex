/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 12:27:18 by pravry            #+#    #+#             */
/*   Updated: 2021/12/19 19:31:08 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>

typedef struct 	s_data 
{
	char	*in;
	char	*out;
	char	***cmds;
	char	**envp;
}				t_data;

int	init_data(t_data *data, char *argv[], char *envp[]);
void	free_data(t_data *data);
void	free_split(char **split);
int	valid_cmd(t_data *data, char *cmd);
int	valid_in(t_data *data);
int	valid_out(t_data *data);
int	is_dir(char *path);

#endif
