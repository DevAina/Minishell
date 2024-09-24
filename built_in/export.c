/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 07:54:36 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/24 09:45:20 by traveloa         ###   ########.fr       */
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

void	print_var_value(char *value, int i, t_list *env_lst)
{
	ft_putchar_fd(value[i], 1);
	i++;
	ft_putchar_fd('"', 1);
	ft_putstr_fd((char *)env_lst->content + i, 1);
	ft_putchar_fd('"', 1);
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
			print_var_value(tmp, i, env_lst);
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
	int		i;
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

int	check_var_name(char *name)
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
		{
			if (name[i] == '+' && name[i + 1] == '=')
				i++;
			else
				return (0);
		}
		i++;
	}
	return (1);
}

char	*get_var_name(char *var)
{
	int		i;
	int		j;
	char	*name;

	i = 0;
	j = 0;
	while (var[i] && var[i] != '+' && var[i] != '=')
		i++;
	name = ft_calloc(i + 1, sizeof(char));
	while (j < i)
	{
		name[j] = var[j];
		j++;
	}
	name[j] = '\0';
	return (name);
}

char	*get_var_value(char *var)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	j = 0;
	while (var[i] && var[i] != '=')
		i++;
	if (var[i] == '\0')
		return (0);
	i++;
	value = ft_calloc(ft_strlen(var) - i + 1, sizeof(char));
	while (var[i])
	{
		value[j] = var[i];
		i++;
		j++;
	}
	return (value);
}

void	concatenate_var(t_list *env_lst, char *tmp, char *var_value
			, char *var_name)
{
	while (env_lst)
	{
		tmp = get_var_name((char *)env_lst->content);
		if (ft_strncmp(var_name, tmp, ft_strlen(var_name)) == 0)
		{
			free(tmp);
			tmp = (char *)env_lst->content;
			env_lst->content = ft_strjoin(tmp, var_value);
			free(tmp);
			break ;
		}
		free(tmp);
		env_lst = env_lst->next;
	}
}

void	handle_var_concatenation(t_list *env_lst, char *var_name
			, char *var_value)
{
	char	*tmp;
	t_list	*head;

	head = env_lst;
	tmp = NULL;
	concatenate_var(env_lst, tmp, var_value, var_name);
	if (env_lst == NULL)
	{
		tmp = ft_strjoin(var_name, "=");
		free(var_name);
		var_name = ft_strjoin(tmp, var_value);
		add_to_env_lst(head, var_name);
		free(tmp);
	}
}

void	add_new_var(t_list *env_lst, char *var)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(var);
	if (ft_strchr(var, '+') == NULL)
	{
		remove_one(&env_lst, var_name);
		add_to_env_lst(env_lst, var);
	}
	else
	{
		var_value = get_var_value(var);
		handle_var_concatenation(env_lst, var_name, var_value);
		free(var_value);
	}
	free(var_name);
}

void	export_assignement(char **assignement, t_list *env_lst, int	*status)
{
	int		j;

	j = 0;
	while (assignement && assignement[j])
	{
		if (check_var_name(assignement[j]) == 0)
		{
			ft_putstr_fd(assignement[j], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			*status = EXIT_FAILURE;
			j++;
			continue ;
		}
		add_new_var(env_lst, assignement[j]);
		j++;
	}
}

void	export_without_assignement(char **cmd, t_list *env_lst, int *status)
{
	int		i;

	i = 1;
	while (cmd[i])
	{
		if (check_var_name(cmd[i]) == 0)
		{
			ft_putstr_fd(cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			*status = EXIT_FAILURE;
			i++;
			continue ;
		}
		add_new_var(env_lst, cmd[i]);
		i++;
	}
}

int	ft_export(char **cmd, char **assignement, char ***env)
{
	t_list	*env_lst;
	char	**tmp;
	int		status;

	tmp = *env;
	status = 0;
	env_lst = get_env_lst(*env);
	if (!cmd[1] && !assignement)
		print_export(env_lst);
	else
	{
		export_without_assignement(cmd, env_lst, &status);
		export_assignement(assignement, env_lst, &status);
		free_split(tmp);
		*env = list_to_tab(env_lst);
	}
	free_env_lst(env_lst);
	return (status);
}
