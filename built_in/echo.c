/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:42:10 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/09 15:58:16 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_echo(char **args)
{
	int	opt;
	int	i;

	i = 1;
	opt = 0;
	if (args[1] != NULL && ft_strncmp(args[1], "-n", 3) == 0)
	{
		opt = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (opt == 0)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}
