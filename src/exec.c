/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/08/28 08:04:24 by traveloa         ###   ########.fr       */
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
	while (ast->input[i].target)
	{
		fd = open(ast->input[i].target, O_RDONLY);
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
	while (ast->output[i].target)
	{
		fd = open(ast->output[i].target, O_RDONLY | O_WRONLY | O_CREAT, 0777);
		dup2(fd, 1);
		i++;
	}
}

void	output_append(t_ast_node *ast)
{
	int	i;
	int	fd;

	i = 0;
	while (ast->output_append[i].target)
	{
		fd = open(ast->output_append[i].target, O_RDONLY | O_WRONLY
				| O_CREAT | O_APPEND, 0777);
		dup2(fd, 1);
		i++;
	}
}

void	read_input_heredoc(int fd[2], t_ast_node *ast)
{
	char	*line;
	int		i;

	i = 0;
	close(fd[0]);
	while (ast->heredoc[i].target)
	{
		while (1)
		{
			line = readline("> ");
			if (ft_strncmp(line, ast->heredoc[i].target,
					ft_strlen(ast->heredoc[i].target) + 1) == 0)

			{
				free (line);
				break ;
			}
			ft_putendl_fd(line, fd[1]);
			free(line);
		}
		i++;
	}
	close(fd[1]);
	exit(0);
}

void	here_doc(t_ast_node *ast)
{
	int		fd[2];

	pipe(fd);
	if (fork() == 0)
		read_input_heredoc(fd, ast);
	wait(0);
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
}

void	check_redirection(t_ast_node *ast)
{
	if (ast->input)
		redir_input(ast);
	if (ast->output)
		redir_output(ast);
	if (ast->output_append)
		output_append(ast);
	if (ast->heredoc)
		here_doc(ast);
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
