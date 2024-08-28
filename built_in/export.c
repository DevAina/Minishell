/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 07:54:36 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/28 11:22:15 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*get_env_lst(char **envp)
{
	int		i;
	t_list	*env_lst;

	i = 0;
	env_lst = ft_lstnew((void *)envp[i]);
	i++;
	while (envp[i])
	{
		ft_lstadd_back(&env_lst, ft_lstnew((void *)envp[i]));
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
}

char	*alloc_n_cpy(char *src, char *dest, int len)
{
	int	i;

	i = 0;
	dest = malloc(len + 1);
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

char	*search_var_name(t_list *env_lst, char *var_name)
{
	int		len;

	len = ft_strlen(var_name);
	while (env_lst)
	{
		if (ft_strncmp(var_name, (char *)env_lst->content, len) == 0)
			return ((char *)env_lst->content + len + 1);
		env_lst = env_lst->next;
	}
	return (0);
}

void	add_to_env_lst(t_list *env_lst, char *content)
{
	int		i;
	int		j;
	char	*tmp;
	char	*var_name;

	i = 0;
	tmp = NULL;
	var_name = NULL;
	if (ft_strchr(content, '$') != NULL)
	{
		while (content[i] != '$')
			i++;
		tmp = alloc_n_cpy(content, tmp, i);
		i++;
		j = i;
		while (ft_isalnum(content[i]) == 1)
			i++;
		var_name = alloc_n_cpy(content + j, var_name, (i - j));
		var_name = search_var_name(env_lst, var_name);
		content = ft_strjoin(tmp, var_name);
		free(tmp);
	}
	ft_lstadd_back(&env_lst, ft_lstnew((void *)content));
}
