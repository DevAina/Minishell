/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/12 13:28:42 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void	redir_input(char *input)
{
	int	fd;

		fd = open(input, O_RDONLY);
		if (fd < 0)
		{
			perror (" ");
			exit(EXIT_FAILURE);
		}
		dup2(fd, 0);
}

void	redir_output(char *output)
{
	int	fd;

	fd = open(output, O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC
		, 0644);
	if (fd < 0)
		exit (EXIT_FAILURE);
	dup2(fd, 1);
}

void	output_append(char *out_append)
{
	int	fd;

	fd = open(out_append, O_RDONLY | O_WRONLY
		   | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		exit (EXIT_FAILURE);
	dup2(fd, 1);
}

void	here_doc(void)
{
	int		fd;

	fd = open(".tmp", O_RDONLY);
	dup2(fd, 0);
	close(fd);
}

void	check_redirection(t_ast_node *ast)
{
	int	i;

	i = 0;
	while (ast->redirection[i].target)
	{
		if (ast->redirection[i].type_redirection == REDIRECTION_IN)
			redir_input(ast->redirection[i].target);
		else if (ast->redirection[i].type_redirection == REDIRECTION_OUT)
			redir_output(ast->redirection[i].target);
		else if (ast->redirection[i].type_redirection == REDIRECTION_APPEND)
			output_append(ast->redirection[i].target);
		if (ast->redirection[i].type_redirection == REDIRECTION_HEREDOC)
			here_doc();
		i++;
	}
}

int		check_n_exec_built_in(char **cmd, char **env, char **assignement)
{
	if (ft_strncmp(cmd[0], "pwd", 3) == 0)
	{
		ft_pwd(cmd, env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "echo", 5) == 0)
	{
		ft_echo(cmd);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
	{
		ft_env(env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
	{
		ft_export(cmd,assignement, &env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
	{
		mns_cd(cmd, &env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
	{
		ft_unset(cmd, &env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "exit", 5) == 0)
	{
		ft_exit(cmd);
		return (1);
	}
	return (0);
}

void	exec_cmd(char **envp, char **cmd, t_ast_node *ast)
{
	char	**path_list;
	char	*path;

	if (ast->redirection)
		check_redirection(ast);
	if (check_n_exec_built_in(cmd, envp, ast->assignement) == 1)
	{
		free_ast(&ast);
		free_split(envp);
		exit(EXIT_SUCCESS);
	}
	path_list = find_path_list(envp);
	path = find_path(path_list, cmd[0]);
	free_split(path_list);
	if (path == NULL)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(RED" : command not found\n"RESET, 2);
		free_ast(&ast);
		free_split(envp);
		exit(127);
	}
	else if (access(path, X_OK) < 0)
	{
		ft_putstr_fd(" command not found\n", 2);
		free_ast(&ast);
		free_split(envp);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		if (access(path, X_OK) < 0)
			ft_putstr_fd("Permission denied\n", 2);
		free_ast(&ast);
		free_split(envp);
		exit(126);
	}
}

void	pipe_cmd(char **envp, t_ast_node *ast)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid1;
	int		status;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		executor(envp, ast->left);
		free_ast(&ast);
		free_split(envp);
		exit (0);
	}
	pid1 = fork();
	if (pid1 == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		executor(envp, ast->right);
		free_ast(&ast);
		free_split(envp);

		exit (0);
	}
	close(fd[1]);
	close(fd[0]);
	waitpid(pid, NULL, 0);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
}

void	executor(char **envp, t_ast_node *ast)
{
	if (ast->type == 0)
		exec_cmd(envp, ast->args, ast);
	else if (ast->type == 1)
		pipe_cmd(envp, ast);
}
