/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:40:19 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/28 14:36:55 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	mns_cd(char **cmd)
{
	if (!cmd[1])
	{
		//change path to $HOME in env
		//need to change oldpwd and pwd
		if (chdir("/home/traveloa") == -1)
		{
			perror("cd");
			return (-1);
		}
	}
	else
	{
		if (chdir(cmd[1]) == -1)
		{
			perror("cd");
			return (-1);
		}
	}
	return (0);
}
