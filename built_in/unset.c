/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:04:05 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/09 16:00:58 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_one(t_list **env_lst, char *to_remove)
{
	t_list	*tmp;

	tmp = *env_lst;
	if (tmp == NULL || to_remove == NULL)
		return ;
	if (ft_strncmp(to_remove, (char *)tmp->content, ft_strlen(to_remove)) == 0)
	{
		*env_lst = tmp->next;
		free(tmp->content);
		free(tmp);
	}
	else
		remove_one(&tmp->next, to_remove);
}

int		ft_unset(char **cmd, char ***env)
{
	t_list	*env_lst;
	int		i;

	i = 1;
	env_lst = get_env_lst(*env);
	while (cmd[i])
	{
		remove_one(&env_lst, cmd[i]);
		i++;
	}
	free_split(*env);
	*env = list_to_tab(env_lst);
	free_env_lst(env_lst);
	return (EXIT_SUCCESS);
}
