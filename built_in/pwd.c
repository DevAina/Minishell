/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:11:34 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/09 16:01:30 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_pwd(char **args, char **env)
{
	int		i;
	char	**pwd;

	(void)args;
	i = 0;
	while (ft_strnstr(env[i], "PWD", 3) == 0)
		i++;
	pwd = ft_split(env[i], '=');
	ft_putendl_fd(pwd[1], 1);
	free_split(pwd);
	return (EXIT_SUCCESS);
}
