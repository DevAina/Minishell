/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/19 16:42:52 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void	redir_input(char *input, t_ast_node *node, char **envp)
{
	int	fd;

	fd = open(input, O_RDONLY);
	if (fd < 0)
	{
		perror (input);
		free_ast(&node);
		free_split(envp);
		exit(EXIT_FAILURE);
	}
	dup2(fd, 0);
}

void	redir_output(char *output, t_ast_node *node, char **envp)
{
	int	fd;

	fd = open(output, O_RDONLY | O_WRONLY | O_CREAT
			| O_TRUNC, 0644);
	if (fd < 0)
	{
		perror (output);
		free_ast(&node);
		free_split(envp);
		exit (EXIT_FAILURE);
	}
	dup2(fd, 1);
}

void	output_append(char *out_append, t_ast_node *node, char **envp)
{
	int	fd;

	fd = open(out_append, O_RDONLY | O_WRONLY
			| O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror (out_append);
		free_ast(&node);
		free_split(envp);
		exit (EXIT_FAILURE);
	}
	dup2(fd, 1);
}

void	here_doc(void)
{
	int		fd;

	fd = open(".tmp", O_RDONLY);
	dup2(fd, 0);
}

int	check_redirection_exec(t_ast_node *ast, char **envp)
{
	int	i;
	//int	fd;

	i = 0;
	//fd = dup(STDOUT_FILENO);
	while (ast->redirection[i].target)
	{
		if (ast->redirection[i].type_redirection == REDIRECTION_IN)
			redir_input(ast->redirection[i].target, ast, envp);
		else if (ast->redirection[i].type_redirection == REDIRECTION_OUT)
			redir_output(ast->redirection[i].target, ast, envp);
		else if (ast->redirection[i].type_redirection == REDIRECTION_APPEND)
			output_append(ast->redirection[i].target, ast, envp);
		if (ast->redirection[i].type_redirection == REDIRECTION_HEREDOC)
			here_doc();
		i++;
	}
	return (0);
}

int	check_n_exec_built_in(char **cmd, char **env, t_ast_node *ast)
{
	if (ft_strncmp(cmd[0], "pwd", 4) == 0)
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
		ft_env(env, cmd);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
	{
		ft_export(cmd, ast->assignement, &env);
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
		ft_exit(cmd, ast, env);
		return (1);
	}
	return (0);
}

char	**check_void_cmd(char **cmd, char **envp, t_ast_node *ast)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = cmd;
	while (ft_strlen(tmp[i]) == 0 && tmp[i] != NULL)
		i++;
	if (tmp[i] == NULL)
	{
		free_split(envp);
		free_ast(&ast);
		exit(EXIT_SUCCESS);
	}
	tmp += i;
	return (tmp);
}

void	execute(t_ast_node *ast, char **envp, char **cmd)
{
	char	**path_list;
	char	*path;

	path_list = find_path_list(envp);
	path = find_path(path_list, cmd[0]);
	free_split(path_list);
	if (path == NULL)
	{
		ft_putstr_fd(cmd[0], 2);
		perror(" ");
		free_ast(&ast);
		free_split(envp);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_putstr_fd(cmd[0], 2);
		perror(" ");
		free_ast(&ast);
		free_split(envp);
		exit(126);
	}
}

void	exec_cmd(char **envp, char **cmd, t_ast_node *ast)
{
	char	**tmp;

	tmp = check_void_cmd(cmd, envp, ast);
	if (ast->redirection)
		check_redirection_exec(ast, envp);
	if (check_n_exec_built_in(tmp, envp, ast) == 1)
		return ;
	execute(ast, envp, cmd);
}

void	pipe_exec_left(int fd[2], t_ast_node *ast, char **envp)
{
	close(fd[0]);
	dup2(fd[1], 1);
	executor(envp, ast->left);
	close(fd[1]);
	free_ast(&ast);
	free_split(envp);
	exit (EXIT_FAILURE);
}

void	pipe_exec_right( int fd[2], t_ast_node *ast, char **envp)
{
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	executor(envp, ast->right);
	free_ast(&ast);
	free_split(envp);
	exit (EXIT_FAILURE);
}

void	pipe_cmd(char **envp, t_ast_node *ast)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
		pipe_exec_left(fd, ast, envp);
	waitpid(pid, NULL, 0);
	pipe_exec_right(fd, ast, envp);
	close(fd[1]);
	close(fd[0]);
}

void	executor(char **envp, t_ast_node *ast)
{
	if (ast == NULL)
	{
		free_ast(&ast);
		free_split(envp);
		exit (EXIT_FAILURE);
	}
	if (ast->type == 0)
		exec_cmd(envp, ast->args, ast);
	else if (ast->type == 1)
		pipe_cmd(envp, ast);
}
