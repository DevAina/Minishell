/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/13 14:25:11 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_cmd(char **envp, char **cmd)
{
	char	**path_list;
	char	*path;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		path_list = find_path_list(envp);
		path = find_path(path_list, cmd[0]);
		execve(path, cmd, envp);
	}
	waitpid(pid, NULL, 0);
}

void	executor(char **envp, t_ast_node *ast)
{
	if (ast == NULL)
		return;
	if (ast->type == 0)
		exec_cmd(envp, ast->args);
}
