/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:41:54 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/24 14:49:47 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**find_path_list(char **env)
{
	int		i;
	char	*tmp;
	char	**path;

	i = 0;
	path = 0;
	while (env[i])
	{
		tmp = ft_strnstr(env[i], "PATH", 4);
		if (tmp != 0)
			break ;
		i++;
	}
	path = ft_split(tmp + 5, ':');
	return (path);
}

char	*check_path(char **path_list, char *cmd)
{
	int		i;
	char	*path;
	char	*tmp;

	i = 0;
	while (path_list[i])
	{
		tmp = ft_strjoin(path_list[i], "/");
		path = ft_strjoin(tmp, cmd);
		if (access(path, F_OK) == 0)
		{
			free(tmp);
			return (path);
		}
		else
		{
			free(tmp);
			free(path);
			i++;
		}
	}
	return (0);
}

char	*find_path(char **path_list, char *cmd)
{
	if (cmd == 0)
		return (0);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
		return (0);
	}
	return (check_path(path_list, cmd));
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
