/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:17:50 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/11 15:11:03 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <complex.h>

int		ft_exit(char **cmd)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	if (cmd[2])
	{
		ft_putstr_fd("too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd[1])
	{
		status = ft_atoi(cmd[1]);
		while (cmd[1][i])
		{
			if (cmd[1][0] == '+' || cmd[1][0] == '-')
				i++;
			if (cmd[1][i] < '0' || cmd[1][i] > '9')
			{
				ft_putstr_fd("numeric argument requiered\n", 2);
				status = 2;
				break ;
			}
			i++;
		}
	}
	exit(status);
	return (EXIT_SUCCESS);
}
