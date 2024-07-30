/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 07:54:36 by traveloa          #+#    #+#             */
/*   Updated: 2024/07/30 09:32:54 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "libft/libft.h"

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
		if (ft_strncmp((char *)env_tmp->content, (char *)env_tmp->next->content, 500) > 0)
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
