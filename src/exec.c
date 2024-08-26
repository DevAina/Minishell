/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/22 16:34:16 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <readline/readline.h>

void	redir_input(t_ast_node *ast)
{
	int	i;
	int	fd;

	i = 0;
	while (ast->input_file[i])
	{
		fd = open(ast->input_file[i], O_RDONLY);
		dup2(fd, 0);
		i++;
	}
}

void	redir_output(t_ast_node *ast)
{
	int	i;
	int	fd;

	i = 0;
	while (ast->output_file[i])
	{
		fd = open(ast->output_file[i], O_RDONLY | O_WRONLY | O_CREAT, 0777);
		dup2(fd, 1);
		i++;
	}
}

void	output_append(t_ast_node *ast)
{
	int	i;
	int	fd;

	i = 0;
	while (ast->output_append[i])
	{
		fd = open(ast->output_append[i], O_RDONLY | O_WRONLY
				| O_CREAT | O_APPEND, 0777);
		dup2(fd, 1);
		i++;
	}
}

void	check_redirection(t_ast_node *ast)
{
	char	*line;

	if (ast->input_file)
		redir_input(ast);
	if (ast->output_file)
		redir_output(ast);
	if (ast->output_append)
		output_append(ast);
	if (ast->heredoc_delimiter)
	{
		int	i = 0;
		int		fd;

		fd = dup(0);
		line = readline("heredoc> ");
		while (ft_strncmp(ast->heredoc_delimiter[i], line, ft_strlen(ast->heredoc_delimiter[i])) != 0)
		{
			free(line);
			line = readline("heredoc> ");
		}
		free(line);
		dup2(fd, 0);
	}
}

void	exec_cmd(char **envp, char **cmd, t_ast_node *ast)
{
	char	**path_list;
	char	*path;

	path_list = find_path_list(envp);
	path = find_path(path_list, cmd[0]);
	free_split(path_list);
	if (path == NULL)
	{
		ft_putendl_fd("command not found", 2);
		return ;
	}
	check_redirection(ast);
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

void	executor(char **envp, t_ast_node *ast)
{
	if (ast->type == 0)
		exec_cmd(envp, ast->args, ast);
	else if (ast->type == 1)
		pipe_cmd(envp, ast);
}
