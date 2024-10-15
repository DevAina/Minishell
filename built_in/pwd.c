/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:11:34 by traveloa          #+#    #+#             */
/*   Updated: 2024/10/15 14:11:45 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(char **args, char **env)
{
	char	*pwd;

	(void)args;
	pwd = NULL;
	pwd = get_env_value("PWD", env);
	if (pwd)
		ft_putendl_fd(pwd, 1);
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
