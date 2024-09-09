/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:17:50 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/09 10:23:58 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_exit(char **cmd)
{
	int	status;

	status = 0;
	if (cmd[1])
		status = ft_atoi(cmd[1]);
	exit(status);
}
