/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:46:23 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/04 11:09:20 by trarijam         ###   ########.fr       */
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
	return (0);
}

void	exec_cmd(char **envp, char **cmd, t_ast_node *ast)
{
	char	**path_list;
	char	*path;

	check_redirection(ast);
	if (check_n_exec_built_in(cmd, envp, ast->assignement) == 1)
		exit(0);
	path_list = find_path_list(envp);
	path = find_path(path_list, cmd[0]);
	free_split(path_list);
	if (path == NULL)
	{
		ft_putendl_fd("command not found", 2);
		exit(1);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_putendl_fd("command not found", 2);
		exit(1);
	}
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
