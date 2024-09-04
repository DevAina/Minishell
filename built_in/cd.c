/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:40:19 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/04 11:28:01 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_path(char **env, char *path_name)
{
	int		i;
	char	*path;
	char	**tmp;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(path_name, env[i], 4) == 0)
			break;
		i++;
	}
	tmp = ft_split(env[i], '=');
	path = ft_strdup(tmp[1]);
	free_split(tmp);
	return (path);
}

void	update_old_pwd(t_list *env_lst, char *old_pwd)
{
	while (env_lst)
	{
		if (ft_strncmp("OLDPWD", (char *)env_lst->content, 3) == 0)
		{
			free(env_lst->content);
			env_lst->content = ft_strjoin("OLDPWD=", old_pwd);
			break ;
		}
		env_lst = env_lst->next;
	}
}

void	update_pwd(char ***env, char *old_pwd)
{
	t_list	*env_lst;
	t_list	*head;
	char	*pwd;

	env_lst = get_env_lst(*env);
	pwd = getcwd(NULL, 0);
	head = env_lst;
	update_old_pwd(env_lst, old_pwd);
	while (env_lst)
	{
		if (ft_strncmp("PWD", (char *)env_lst->content, 3) == 0)
		{
			free(env_lst->content);
			env_lst->content = ft_strjoin("PWD=", pwd);
			free(pwd);
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
	char	*path_name;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cmd[2])
	{
		ft_putendl_fd("too many arguments", 2);
		return (0);
	}
	if (!cmd[1] || ft_strncmp(cmd[1], "~", 2) == 0)
	{
		path_name = get_path(*env, "HOME");
		if (path_name == NULL)
			return (-1);
		if (chdir(path_name) == -1)
		{
			perror("cd");
			free(path_name);
			free(cwd);
			return (-1);
		}
		update_pwd(env, cwd);
	}
	else if (ft_strncmp(cmd[1], "-", 2) == 0)
	{
		path_name = get_path(*env, "OLDPWD");
		if (path_name == NULL)
			return (-1);
		if (chdir(path_name) == -1)
		{
			perror("cd");
			free(path_name);
			free(cwd);
			return (-1);
		}
		update_pwd(env, cwd);
	}
	else
	{
		if (chdir(cmd[1]) == -1)
		{
			perror("cd");
			free(cwd);
			return (-1);
		}
		update_pwd(env, cwd);
	}
	return (0);
}
