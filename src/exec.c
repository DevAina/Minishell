/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/23 12:18:00 by traveloa         ###   ########.fr       */
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
		if (fd < 0)
		{
			exit(1);
		}
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
		int		tmp_fd;
		int		dif = 1;

		if (fork() == 0)
		{
			tmp_fd = open("tmp", O_WRONLY | O_RDONLY | O_CREAT, 0777);
			while (dif != 0)
			{
				line = readline("heredoc> ");
				dif = ft_strncmp(line, ast->heredoc_delimiter[i], ft_strlen(ast->heredoc_delimiter[i]) + 1);
				ft_putendl_fd(line, tmp_fd);
				free(line);
			}
			close(tmp_fd);
			exit(0);
		}
		tmp_fd = open("tmp", O_RDONLY);
		wait(0);
		dup2(tmp_fd, 0);
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
