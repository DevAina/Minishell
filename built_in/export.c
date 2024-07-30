/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 07:54:36 by traveloa          #+#    #+#             */
/*   Updated: 2024/07/30 10:47:56 by traveloa         ###   ########.fr       */
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

void	print_env(t_list *env_lst)
{
	int		i;
	char	*tmp;
	while (env_lst)
	{
		i = 0;
		tmp = (char *)env_lst->content;
		ft_putstr_fd("declare -x ", 1);
		while (tmp[i] != '=')
		{
			ft_putchar_fd(tmp[i], 1);
			i++;
		}
		ft_putchar_fd(tmp[i], 1);
		i++;
		ft_putchar_fd('"', 1);
		ft_putstr_fd((char *)env_lst->content + i, 1);
		ft_putchar_fd('"', 1);
		ft_putstr_fd("\n", 1);
		env_lst = env_lst->next;
	}
}

void	add_to_env_lst(t_list *env_lst, char *content)
{
	ft_lstadd_back(&env_lst, ft_lstnew((void *)content));
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_list	*env_lst;

	env_lst = get_env_lst(envp);
	sort_env_lst(&env_lst);
	add_to_env_lst(env_lst, "var=a b c");
	print_env(env_lst);

}
