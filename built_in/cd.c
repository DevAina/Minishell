/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:40:19 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/29 11:03:09 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	mns_cd(char **cmd)
{
	char	*home_path;

	if (!cmd[1] || ft_strncmp(cmd[1], "~", 2) == 0)
	{
		//change path to $HOME in env
		//need to change oldpwd and pwd
		home_path = getenv("HOME");
		if (home_path == NULL)
			return (-1);
		if (chdir(home_path) == -1)
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
