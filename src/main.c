/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:48:11 by trarijam          #+#    #+#             */
/*   Updated: 2024/07/30 10:50:59 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)envp;
	(void)argv;
	char	**split;
	int		i;
	char	dir[1024];
	char	*line;

	while (1)
	{
		getcwd(dir, sizeof(dir));
		ft_putstr_fd(getenv("USER"), 1);
		ft_putstr_fd(dir, 1);
		line = readline("$ ");
		if (line == NULL)
			break ;
		if (*line == '\0')
		{
			free(line);
			break ;
		}
		split = ft_split(line, ' ');
		if (ft_strncmp(split[0], "cd", 2) == 0)
			mns_cd(split[1]);
		i = 0;
		while (split[i] != NULL)
		{
			free(split[i]);
			i++;
		}
		free(split);
		free(line);
	}
	return (0);
}