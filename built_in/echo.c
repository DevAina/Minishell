/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:42:10 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/27 09:43:14 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_echo(char **arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		ft_putstr_fd(arg[i], 1);
		i++;
	}
}
