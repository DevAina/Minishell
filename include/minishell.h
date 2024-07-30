/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 08:44:10 by trarijam          #+#    #+#             */
/*   Updated: 2024/07/30 09:53:29 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>

/***built_in**/
int	mns_cd(char *path);

/******utils********/
int		mns_strcmp(char *s1, char *s2);
t_list	*get_env_lst(char **envp);

#endif