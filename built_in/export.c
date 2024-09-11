/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 07:54:36 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/11 07:51:50 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*get_env_lst(char **envp)
{
	int		i;
	t_list	*env_lst;

	i = 0;
	env_lst = ft_lstnew((void *)ft_strdup(envp[i]));
	i++;
	while (envp[i])
	{
		ft_lstadd_back(&env_lst, ft_lstnew((void *)ft_strdup(envp[i])));
		i++;
	}
	return (env_lst);
}

void	sort_env_lst(t_list	**env_lst)
{
	t_list	*env_tmp;
	char	*tmp;

	env_tmp = *env_lst;
	while (env_tmp->next)
	{
		if (mns_strcmp((char *)env_tmp->content,
				(char *)env_tmp->next->content) > 0)
		{
			tmp = (char *)env_tmp->content;
			env_tmp->content = env_tmp->next->content;
			env_tmp->next->content = tmp;
			env_tmp = *env_lst;
		}
		else
			env_tmp = env_tmp->next;
	}
}

void	print_export(t_list *env_lst)
{
	int		i;
	char	*tmp;

	sort_env_lst(&env_lst);
	while (env_lst)
	{
		i = 0;
		tmp = (char *)env_lst->content;
		ft_putstr_fd("declare -x ", 1);
		while (tmp[i] != '=' && tmp[i])
		{
			ft_putchar_fd(tmp[i], 1);
			i++;
		}
		if (tmp[i])
		{
			ft_putchar_fd(tmp[i], 1);
			i++;
			ft_putchar_fd('"', 1);
			ft_putstr_fd((char *)env_lst->content + i, 1);
			ft_putchar_fd('"', 1);
		}
		ft_putstr_fd("\n", 1);
		env_lst = env_lst->next;
	}
	free_env_lst(env_lst);
}

void	add_to_env_lst(t_list *env_lst, char *content)
{
	ft_lstadd_back(&env_lst, ft_lstnew((void *)ft_strdup(content)));
}

char	**list_to_tab(t_list *env_lst)
{
	int		size;
	char	**env;
	int		i;

	i = 0;
	size = ft_lstsize(env_lst);
	env = malloc(sizeof(char *) * (size + 1));
	while (env_lst)
	{
		env[i] = ft_strdup((char *)env_lst->content);
		i++;
		env_lst = env_lst->next;
	}
	env[i] = NULL;
	return (env);
}

char	**cpy_env(char **env)
{
	int	i;
	char	**env_cpy;

	i = 0;
	while (env[i])
		i++;
	env_cpy = malloc(sizeof(char *) *(i + 1));
	i = 0;
	while (env[i])
	{
		env_cpy[i] = ft_strdup(env[i]);
		i++;
	}
	env_cpy[i] = NULL;
	return (env_cpy);
}

void	check_n_remove(t_list **env_lst, char *content)
{
	t_list	*tmp;
	char	**content_split;

	tmp = *env_lst;
	content_split = ft_split(content, '=');
	remove_one(env_lst, content_split[0]);
	free_split(content_split);
}

int		check_var_name(char *name)
{
	int	i;

	i = 0;
	if (ft_isalpha(name[i]) == 0 && name[i] != '_')
		return (0);
	i++;
	while (name[i])
	{
		if (name[i] == '=')
			break ;
		if (name[i] == '_')
			i++;
		if (ft_isalnum(name[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int		ft_export(char **cmd, char **assignement, char ***env)
{
	t_list	*env_lst;
	int		i;
	int		j;
	char	**tmp;

	i = 1;
	j = 0;
	tmp = *env;
	env_lst = get_env_lst(*env);
	if (!cmd[1] && !assignement)
		print_export(env_lst);
	else
	{
		while (cmd[i])
		{
			if (check_var_name(cmd[i]) == 0)
			{
				ft_putstr_fd(cmd[i], 2);
				ft_putstr_fd(": not a valid identifier\n", 2);
				free_env_lst(env_lst);
				return (EXIT_FAILURE);
			}
			check_n_remove(&env_lst, cmd[i]);
			add_to_env_lst(env_lst, cmd[i]);
			i++;
		}
		while (assignement && assignement[j])
		{
			if (check_var_name(assignement[j]) == 0)
			{
				ft_putstr_fd(assignement[j], 2);
				ft_putstr_fd(": not a valid identifier\n", 2);
				free_env_lst(env_lst);
				return (EXIT_FAILURE);
			}
			check_n_remove(&env_lst, assignement[j]);
			add_to_env_lst(env_lst, assignement[j]);
			j++;
		}
		free_split(tmp);
		*env = list_to_tab(env_lst);
	}
	free_env_lst(env_lst);
	return (EXIT_SUCCESS);
}
