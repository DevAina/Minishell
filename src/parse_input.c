/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:04:33 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/12 12:59:50 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i] != NULL)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	free_redirection(t_redirection *redirection)
{
	int	i;

	i = 0;
	while (redirection[i].target != NULL)
	{
		free(redirection[i].target);
		i++;
	}
	free(redirection);
}

void	free_ast(t_ast_node **node)
{
	if (node == NULL || *node == NULL)
		return ;
	if ((*node)->type != AST_PIPE && (*node)->args != NULL)
		free_matrix((*node)->args);
	if ((*node)->type != AST_PIPE && (*node)->redirection != NULL)
		free_redirection((*node)->redirection);
	if ((*node)->left != NULL)
		free_ast(&(*node)->left);
	if ((*node)->right != NULL)
		free_ast(&(*node)->right);
	free(*node);
}

t_ast_node	*init_node(t_ast_node_type type)
{
	t_ast_node *node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
    node->type = type;
    node->args = NULL;
    node->left = NULL;
    node->right = NULL;
	node->assignement = NULL;
    node->redirection = NULL;
    return (node);
}

void	count_redirection(t_token **tokens, int *count)
{
	if ((*tokens)->type == TOKEN_REDIR_IN)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[REDIR_COUNT] += 1;
	}
	if ((*tokens)->type == TOKEN_REDIR_OUT)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[REDIR_COUNT] += 1;
	}
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[REDIR_COUNT] += 1;
	}
	if ((*tokens)->type == TOKEN_HEREDOC)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[REDIR_COUNT] += 1;
	}
}


void	count_type_token(t_token *tokens, int *count)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp != NULL && tmp->type != TOKEN_PIPE
		&& tmp->type != TOKEN_EOF)
	{
		if (tmp->type == TOKEN_WORD)
			count[ARG_COUNT] += 1;
		if (tmp->type == TOKEN_ASSIGNEMENT)
			count[ASSIGNEMENT_COUNT] += 1;
		if (tmp->type == TOKEN_REDIR_IN_OUT)
		{
			tmp = tmp->next;
			if (tmp->type == TOKEN_WORD)
				count[REDIR_COUNT] += 1;
		}
		count_redirection(&tmp, count);
		tmp = tmp->next;
	}
}

void	init_args_input_output_file(t_ast_node **cmd, int *count)
{
	if (count[ARG_COUNT] != 0)
		(*cmd)->args = (char **)ft_calloc(count[ARG_COUNT] + 1,
			sizeof(char *));
	if (count[REDIR_COUNT] != 0)
		(*cmd)->redirection = (t_redirection *)ft_calloc(count[REDIR_COUNT] + 1,
			sizeof(t_redirection));
	if (count[ASSIGNEMENT_COUNT] != 0)
		(*cmd)->assignement = (char **)ft_calloc(count[ASSIGNEMENT_COUNT] + 1,
			sizeof(char *));
}

void handle_redirection(t_token **tokens, t_redirection *redirection,
	int *file_count, int count)
{
	redirection[*file_count].fd = (*tokens)->fd;
	if ((*tokens)->type == TOKEN_REDIR_IN)
		redirection[*file_count].type_redirection = REDIRECTION_IN;
	if ((*tokens)->type == TOKEN_REDIR_OUT)
		redirection[*file_count].type_redirection = REDIRECTION_OUT;
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
		redirection[*file_count].type_redirection = REDIRECTION_APPEND;
	if ((*tokens)->type == TOKEN_HEREDOC)
		redirection[*file_count].type_redirection = REDIRECTION_HEREDOC;
	*tokens = (*tokens)->next;
	if ((*tokens)->type == TOKEN_WORD && count != 0)
	{
		redirection[*file_count].target = ft_strdup((*tokens)->value);
		(*file_count)++;
	}
}

void set_null_terminators(t_ast_node *cmd, int *count, int *counts)
{
	if (count[ARG_COUNT] != 0)
		cmd->args[counts[ARG_COUNT]] = NULL;
	if (count[REDIR_COUNT] != 0)
		cmd->redirection[counts[REDIR_COUNT]].target = NULL;
	if (count[ASSIGNEMENT_COUNT] != 0)
		cmd->assignement[counts[ASSIGNEMENT_COUNT]] = NULL;
}

void process_token(t_token **tokens, t_ast_node *cmd, int *count,
	int *counts)
{
	if ((*tokens)->type == TOKEN_WORD && count[ARG_COUNT] != 0)
	{
		if ((*tokens)->value != NULL)
			cmd->args[counts[ARG_COUNT]++] = ft_strdup((*tokens)->value);
		else
			cmd->args[counts[ARG_COUNT]++] = NULL;
	}
	if ((*tokens)->type == TOKEN_ASSIGNEMENT && count[ASSIGNEMENT_COUNT] != 0)
		cmd->assignement[counts[ASSIGNEMENT_COUNT]++] =
			ft_strdup((*tokens)->value);
	if ((*tokens)->type == TOKEN_REDIR_IN)
		handle_redirection(tokens, cmd->redirection,
			&counts[REDIR_COUNT], count[REDIR_COUNT]);
	if ((*tokens)->type == TOKEN_REDIR_OUT)
		handle_redirection(tokens, cmd->redirection,
			&counts[REDIR_COUNT], count[REDIR_COUNT]);
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
		handle_redirection(tokens, cmd->redirection,
			&counts[REDIR_COUNT], count[REDIR_COUNT]);
	if ((*tokens)->type == TOKEN_HEREDOC)
		handle_redirection(tokens, cmd->redirection,
			&counts[REDIR_COUNT], count[REDIR_COUNT]);
}

t_ast_node *parse_token(t_token **tokens, t_ast_node *cmd)
{
	int counts[3];
	int count[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		counts[i] = 0;
		count[i] = 0;
		i++;
	}
	count_type_token(*tokens, count);
	init_args_input_output_file(&cmd, count);
	while (tokens != NULL && (*tokens)->type != TOKEN_PIPE
		&&	(*tokens)->type != TOKEN_EOF)
	{
		process_token(tokens, cmd, count, counts);
		*tokens = (*tokens)->next;
	}

	set_null_terminators(cmd, count, counts);
	return (cmd);
}


t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;

	node = init_node(AST_COMMAND);
	node = parse_token(tokens, node);
	return (node);
}

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*pipe;

	left = parse_command(tokens);
	if (*tokens != NULL && (*tokens)->type == TOKEN_PIPE)
	{
		pipe = init_node(AST_PIPE);
		pipe->left = left;
		*tokens = (*tokens)->next;
		pipe->right = parse_pipeline(tokens);
		return (pipe);
	}
	return (left);
}

t_ast_node	*parse(t_token *tokens)
{
	t_ast_node	*root;

	root = parse_pipeline(&tokens);
	return (root);
}
