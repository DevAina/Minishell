/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:11:34 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/04 11:18:41 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pwd(char **args, char **env)
{
	int		i;
	char	**pwd;

	(void)args;
	i = 0;
	/*if (args[1] && *args[1])
	{
		ft_putendl_fd("too many arguments", 2);
		return ;
	}*/
	while (ft_strnstr(env[i], "PWD", 3) == 0)
		i++;
	pwd = ft_split(env[i], '=');
	ft_putendl_fd(pwd[1], 1);
	free_split(pwd);
}
