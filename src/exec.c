/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/22 10:16:15 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>

void	exec_cmd(char **envp, char **cmd)
{
	char	**path_list;
	char	*path;

	path_list = find_path_list(envp);
	path = find_path(path_list, cmd[0]);
	free_split(path_list);
	if (path == NULL)
		return ;
	execve(path, cmd, envp);
}

void	pipe_cmd(char **envp, t_ast_node *ast)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid1;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		executor(envp, ast->left);
	}
	pid1 = fork();
	if (pid1 == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		executor(envp, ast->right);
	}
	close(fd[1]);
	close(fd[0]);
	waitpid(pid, NULL, 0);
	waitpid(pid1, NULL, 0);

}

void	redir_output(char **envp, t_ast_node *ast)
{
	int		outfile_fd;

	outfile_fd = open(ast->output_file, O_RDONLY | O_WRONLY | O_CREAT, 0777);
	if (outfile_fd < 0)
		return ;
	dup2(outfile_fd, 1);
	executor(envp, ast->left);
}

void	executor(char **envp, t_ast_node *ast)
{
	if (ast->type == 0)
		exec_cmd(envp, ast->args);
	else if (ast->type == 1)
		pipe_cmd(envp, ast);
	else if (ast->type == 3)
		redir_output(envp, ast);
}
