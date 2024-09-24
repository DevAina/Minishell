/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:13:40 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/24 08:53:51 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env(char **env, char **cmd)
{
	int	i;

	if (cmd[1])
	{
		ft_putstr_fd("Env error\n", 2);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i ++;
	}
	return (EXIT_SUCCESS);
}
