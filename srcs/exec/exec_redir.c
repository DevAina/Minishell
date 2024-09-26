/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 07:13:33 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/26 10:34:57 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	close(fd);
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
	close(fd);
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
	close(fd);
}

void	here_doc(void)
{
	int		fd;

	fd = open(".tmp", O_RDONLY);
	dup2(fd, 0);
	close(fd);
}

int	check_redirection_exec(t_ast_node *ast, char **envp)
{
	int	i;

	i = 0;
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
