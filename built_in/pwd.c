/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:11:34 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/28 08:36:56 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pwd(char **args, char **env)
{
	int		i;
	char	**pwd;
	int		opt;

	i = 0;
	opt = 0;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
		opt = 1;
	if ((opt == 1 && args[2] && *args[2]) || (opt == 0 && args[1] && *args[1]))
	{
		ft_putendl_fd("too many arguments or bad option", 2);
		exit(1);
	}
	while (ft_strnstr(env[i], "PWD", 3) == 0)
		i++;
	pwd = ft_split(env[i], '=');
	if (opt == 0)
		ft_putendl_fd(pwd[1], 1);
	else if (opt == 1)
		ft_putstr_fd(pwd[1], 1);
	free_split(pwd);
}
