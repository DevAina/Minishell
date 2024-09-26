/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:17:50 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/26 10:34:05 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_exit_status(int *status, char **cmd)
{
	int	i;

	i = 0;
	while (cmd[1][i])
	{
		if (cmd[1][0] == '+' || cmd[1][0] == '-')
			i++;
		if (cmd[1][i] < '0' || cmd[1][i] > '9')
		{
			ft_putstr_fd("numeric argument requiered\n", 2);
			*status = 2;
			break ;
		}
		i++;
	}
}

int	ft_exit(char **cmd, t_ast_node *ast, char **envp, int flag)
{
	int	status;

	status = 0;
	if (cmd[1] && cmd[2])
	{
		ft_putstr_fd("too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd[1])
	{
		status = ft_atoi(cmd[1]);
		get_exit_status(&status, cmd);
	}
	if (flag == 1)
	{
		free_ast(&ast);
		free_split(envp);
		exit(status);
	}
	return (status);
}
