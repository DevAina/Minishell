/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:11:34 by traveloa          #+#    #+#             */
/*   Updated: 2024/10/16 08:26:09 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int	ft_pwd(char **args, char **env)
{
	char	*pwd;

	(void)args;
	(void)env;
	pwd = NULL;
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		ft_putendl_fd(pwd, 1);
		free(pwd);
	}
	else
	{
		pwd = get_env_value("PWD", env);
		ft_putendl_fd(pwd, 1);
	}
	return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
