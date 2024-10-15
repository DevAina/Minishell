/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/10/15 17:28:07 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_cmd(char **envp, char **cmd, t_ast_node *ast, int *flag, int in_pipe)
{
	char	**tmp;

	tmp = check_void_cmd(cmd, envp, ast);
	if (ast->redirection)
		check_redirection_exec(ast, envp, in_pipe);
	if (ast->args == NULL)
		return ;
	if (check_n_exec_built_in(tmp, envp, ast, flag) == 1
		|| check_n_exec_built_in1(tmp, envp, flag) == 1)
		return ;
	execute(ast, envp, tmp, flag);
}

void	pipe_exec_left(int fd[2], t_ast_node *ast, char **envp, int *flag, int in_pipe)
{
	close(fd[0]);
	dup2(fd[1], 1);
	close(fd[1]);
	executor(envp, ast->left, flag, in_pipe);
}

int	pipe_exec_right( int fd[2], t_ast_node *ast, char **envp, int *flag, int in_pipe)
{
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	executor(envp, ast->right, flag, in_pipe);
	return (*flag);
}

void	pipe_cmd(char **envp, t_ast_node *ast, int *flag)
{
	int		fd[2];
	static int	in_pipe;
	pid_t	pid;
	pid_t	pid1;

	pid1 = 1;
	if (in_pipe != 0)
		in_pipe += 1;
	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
		pipe_exec_left(fd, ast, envp, flag, in_pipe);
	else
	{
		in_pipe += 1;
		pid1 = fork();
		if (pid1 == 0)
			pipe_exec_right(fd, ast, envp, flag, in_pipe);
	}
	close(fd[1]);
	close(fd[0]);
	waitpid(pid, NULL, 0);
	waitpid(pid1, flag, 0);
	if (WIFEXITED(*flag))
		*flag = WEXITSTATUS(*flag);
}

void	executor(char **envp, t_ast_node *ast, int *flag, int in_pipe)
{
	if (ast->type == 0)
		exec_cmd(envp, ast->args, ast, flag, in_pipe);
	else if (ast->type == 1)
		pipe_cmd(envp, ast, flag);
}
