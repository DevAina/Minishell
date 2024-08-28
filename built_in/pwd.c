/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:11:34 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/28 09:21:28 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pwd(char **args, char **env)
{
	int		i;
	char	**pwd;

	i = 0;
	if (args[1] && *args[1])
	{
		ft_putendl_fd("too many arguments", 2);
		return ;
	}
	while (ft_strnstr(env[i], "PWD", 3) == 0)
		i++;
	pwd = ft_split(env[i], '=');
	ft_putendl_fd(pwd[1], 1);
	free_split(pwd);
}
