/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:17:50 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/16 10:28:41 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <complex.h>

int		ft_exit(char **cmd, t_ast_node *ast, char **envp)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	if (cmd[1] && cmd[2])
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
	free_ast(&ast);
	free_split(envp);
	exit(status);
	return (EXIT_SUCCESS);
}
