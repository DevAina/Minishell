/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mns_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:51:26 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/16 10:00:53 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	mns_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

void	free_env_lst(t_list *env_lst)
{
	t_list	*tmp;

	while (env_lst)
	{
		tmp = env_lst;
		env_lst = env_lst->next;
		free(tmp->content);
		free(tmp);
	}
	free(env_lst);
}

void	close_tmp(void)
{
	char	*name;
	char	*nb;
	int		i;

	i = 0;
	while (1)
	{
		nb = ft_itoa(i);
		name = ft_strjoin(".tmp", nb);
		if (unlink(name) == -1)
		{
			free(nb);
			free(name);
			break ;
		}
		free(nb);
		free(name);
		i++;
	}
}
