/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:40:19 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/30 14:32:55 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_home_path(char **env)
{
	int		i;
	char	*path;
	char	**tmp;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("HOME", env[i], 4) == 0)
			break;
		i++;
	}
	tmp = ft_split(env[i], '=');
	path = ft_strdup(tmp[1]);
	free_split(tmp);
	return (path);
}

void	get_old_pwd(t_list *env_lst)
{
	char	*pwd;
	char	**tmp;

	while (env_lst)
	{
		if (ft_strncmp("PWD", (char *)env_lst->content, 3) == 0)
			pwd = (char *)ft_strdup(env_lst->content);
		if (ft_strncmp("OLDPWD", (char *)env_lst->content, 3) == 0)
		{
			free(env_lst->content);
			tmp = ft_split(pwd, '=');
			env_lst->content = ft_strjoin("OLDPWD=", tmp[1]);
			free_split(tmp);
			break ;
		}
		env_lst = env_lst->next;
	}
}

void	get_pwd(char ***env, char *path)
{
	t_list	*env_lst;
	t_list	*head;

	env_lst = get_env_lst(*env);
	head = env_lst;
	get_old_pwd(env_lst);
	while (env_lst)
	{
		if (ft_strncmp("PWD", (char *)env_lst->content, 3) == 0)
		{
			free(env_lst->content);
			env_lst->content = ft_strjoin("PWD=", path);
			break ;
		}
		env_lst = env_lst->next;
	}
	free_split(*env);
	*env = list_to_tab(head);
	free_env_lst(head);
}

int	mns_cd(char **cmd, char ***env)
{
	char	*home_path;

	if (!cmd[1] || ft_strncmp(cmd[1], "~", 2) == 0)
	{
		home_path = get_home_path(*env);
		if (home_path == NULL)
			return (-1);
		if (chdir(home_path) == -1)
		{
			perror("cd");
			free(home_path);
			return (-1);
		}
		get_pwd(env, home_path);
	}
	else
	{
		if (chdir(cmd[1]) == -1)
		{
			perror("cd");
			return (-1);
		}
		//need to handle => if cmd[1] = (..)
		get_pwd(env, cmd[1]);
	}
	return (0);
}
