/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:04:05 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/29 15:56:25 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_unset(char **cmd, char ***env)
{
	t_list	*env_lst;
	t_list	*head;
	t_list	*tmp;
	int		i;

	i = 1;
	env_lst = get_env_lst(*env);
	if (ft_strncmp((char *)env_lst->content, cmd[i], ft_strlen(cmd[i]) == 0))
	{
		head = env_lst->next;
		free(env_lst->content);
		free(env_lst);
	}
	head = env_lst;
	while (env_lst)
	{
		if (ft_strncmp((char *)env_lst->next->content, cmd[i], ft_strlen(cmd[i]) == 0))
		{
			tmp = env_lst->next;
			env_lst->next = tmp->next;
			free(tmp->content);
			free(tmp);
		}
		else
		{
			printf("%s\n", (char *)env_lst->content);
			env_lst = env_lst->next;
		}
	}
/*	while (head)
	{
		printf("%s\n", (char *)head->content);
		head = head->next;
	}*/
}
