/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:17:50 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/11 09:54:43 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_exit(char **cmd)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	if (cmd[2])
	{
	//	ft_putstr_fd("Exit\n", 1);
		ft_putstr_fd("too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd[1])
	{
		while (cmd[1][i])
		{
			if (cmd[1][0] == '+' || cmd[1][0] == '-')
				i++;
			if (cmd[1][i] < '0' || cmd[1][i] > '9')
			{
				ft_putstr_fd("numeric argument requiered\n", 2);
				break ;
			}
			i++;
		}
		status = ft_atoi(cmd[1]);
	}
	exit(status);
	return (EXIT_SUCCESS);
}
