/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:42:10 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/28 08:19:34 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	opt_cmp(char *str)
{
	int		i;

	i = 1;
	if (str[0] != '-')
		return (0);
	while (str[i] == 'n' && str[i])
		i++;
	if (i == (int)ft_strlen(str))
		return (1);
	else
		return (0);
}

int	ft_echo(t_ast_node *ast)
{
	int	opt;
	int	i;

	i = 1;
	opt = 0;
	while (ast->args[i] != NULL && opt_cmp(ast->args[i]) == 1)
	{
		opt = 1;
		i++;
	}
	while (ast->args[i])
	{
		ft_putstr_fd(ast->args[i], 1);
		if (ast->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (opt == 0)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}
