/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:40:19 by trarijam          #+#    #+#             */
/*   Updated: 2024/07/30 15:04:40 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



int	mns_cd(char *path)
{
	if (ft_strncmp(path, "~", 1) == 0)
	{
		if (chdir(getenv("HOME")) == -1)
		{
			perror("cd");
			return (-1);
		}
		uptdate_env();
	}
	else
	{
		if (chdir(path) == -1)
		{
			perror("cd");
			return (-1);
		}
	}
	return (0);
}