/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:04:33 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/29 16:35:57 by trarijam         ###   ########.fr       */
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

void	free_ast(t_ast_node **node)
{
	if (node == NULL || *node == NULL)
		return ;
	if ((*node)->type != AST_PIPE && (*node)->args != NULL)
		free_matrix((*node)->args);
	if ((*node)->type != AST_PIPE && (*node)->assignement != NULL)
		free_matrix((*node)->assignement);
	if ((*node)->type != AST_PIPE && (*node)->input != NULL)
		free((*node)->input);
	if ((*node)->type != AST_PIPE && (*node)->output != NULL)
		free((*node)->output);
	if ((*node)->type != AST_PIPE && (*node)->heredoc != NULL)
		free((*node)->heredoc);
	if ((*node)->type != AST_PIPE && (*node)->output_append != NULL)
		free((*node)->output_append);
	if ((*node)->type != AST_PIPE && (*node)->input_output != NULL)
	    free((*node)->input_output);
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
    node->input = NULL;
    node->output = NULL;
	node->output_append = NULL;
	node->heredoc = NULL;
	node->input_output = NULL;
    return (node);
}

void	count_redirection(t_token **tokens, int *count)
{
	if ((*tokens)->type == TOKEN_REDIR_IN)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[INPUT_COUNT] += 1;
	}
	if ((*tokens)->type == TOKEN_REDIR_OUT)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[OUTPUT_COUNT] += 1;
	}
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[APPEND_COUNT] += 1;
	}
	if ((*tokens)->type == TOKEN_HEREDOC)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_WORD)
			count[HEREDOC_COUNT] += 1;
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
		if (tmp->type == TOKEN_REDIT_IN_OUT)
		{
			tmp = tmp->next;
			if (tmp->type == TOKEN_WORD)
				count[INPUT_OUTPUT_COUNT] += 1;
		}
		count_redirection(&tmp, count);
		tmp = tmp->next;
	}
}

void	init_args_input_output_file(t_ast_node **cmd, int *count)
{
	if (count[ARG_COUNT] != 0)
		(*cmd)->args = (char **)malloc(sizeof(char *) * (count[ARG_COUNT] + 1));
	if (count[INPUT_COUNT] != 0)
		(*cmd)->input =  (t_redirection *)malloc(sizeof(t_redirection)
			* (count[INPUT_COUNT] + 1));
	if (count[OUTPUT_COUNT] != 0)
		(*cmd)->output = (t_redirection *)malloc(sizeof(t_redirection)
			* (count[OUTPUT_COUNT] + 1));
	if (count[HEREDOC_COUNT] != 0)
		(*cmd)->heredoc =  (t_redirection *)(sizeof(t_redirection)
			* (count[HEREDOC_COUNT] + 1));
	if (count[INPUT_OUTPUT_COUNT] != 0)
		(*cmd)->input_output = (t_redirection *)(sizeof(t_redirection)
		* (count[INPUT_OUTPUT_COUNT] + 1));
	if (count[APPEND_COUNT] != 0)
		(*cmd)->output_append =  (t_redirection *)(sizeof(t_redirection)
			* (count[APPEND_COUNT] + 1));
	if (count[ASSIGNEMENT_COUNT] != 0)
		(*cmd)->assignement = (char **)malloc(sizeof(char *)
			* (count[ASSIGNEMENT_COUNT] + 1));
}

void handle_redirection(t_token **tokens, t_redirection *redirection,
	int *file_count, int count)
{
	redirection[*file_count].fd = (*tokens)->fd;
	*tokens = (*tokens)->next;
	if ((*tokens)->type == TOKEN_WORD && count != 0)
	{
		redirection[*file_count].target = ft_strdup((*tokens)->value);
		(*file_count)++;
	}
}

void set_null_terminators(t_ast_node *cmd, int count[5], int counts[5])
{
	if (count[ARG_COUNT] != 0)
		cmd->args[counts[ARG_COUNT]] = NULL;
	if (count[INPUT_OUTPUT_COUNT] != 0)
		cmd->input[counts[INPUT_OUTPUT_COUNT]].target = NULL;
	if (count[INPUT_COUNT] != 0)
		cmd->input[counts[INPUT_COUNT]].target = NULL;
	if (count[OUTPUT_COUNT] != 0)
		cmd->output[counts[OUTPUT_COUNT]].target = NULL;
	if (count[HEREDOC_COUNT] != 0)
		cmd->heredoc[counts[HEREDOC_COUNT]].target = NULL;
	if (count[APPEND_COUNT] != 0)
		cmd->output_append[counts[APPEND_COUNT]].target = NULL;
	if (count[ASSIGNEMENT_COUNT] != 0)
		cmd->assignement[counts[ASSIGNEMENT_COUNT]] = NULL;
}

void process_token(t_token **tokens, t_ast_node *cmd, int count[5],
	int counts[5])
{
	if ((*tokens)->type == TOKEN_WORD && count[ARG_COUNT] != 0)
		cmd->args[counts[ARG_COUNT]++] = ft_strdup((*tokens)->value);
	if ((*tokens)->type == TOKEN_ASSIGNEMENT && count[ASSIGNEMENT_COUNT] != 0)
		cmd->assignement[counts[ASSIGNEMENT_COUNT]++] =
			ft_strdup((*tokens)->value);
	if ((*tokens)->type == TOKEN_REDIR_IN)
		handle_redirection(tokens, cmd->input,
			&counts[INPUT_COUNT], count[INPUT_COUNT]);
	if ((*tokens)->type == TOKEN_REDIT_IN_OUT)
		handle_redirection(tokens, cmd->input_output,
		&counts[INPUT_OUTPUT_COUNT], count[INPUT_COUNT]);
	if ((*tokens)->type == TOKEN_REDIR_OUT)
		handle_redirection(tokens, cmd->output,
			&counts[OUTPUT_COUNT], count[OUTPUT_COUNT]);
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
		handle_redirection(tokens, cmd->output_append,
			&counts[APPEND_COUNT], count[APPEND_COUNT]);
	if ((*tokens)->type == TOKEN_HEREDOC)
		handle_redirection(tokens, cmd->heredoc,
			&counts[HEREDOC_COUNT], count[HEREDOC_COUNT]);
}

t_ast_node *parse_token(t_token **tokens, t_ast_node *cmd)
{
	int counts[7];
	int count[7];
	int	i;

	i = 0;
	while (i < 7)
	{
		counts[i] = 0;
		count[i] = 0;
		i++;
	}
	count_type_token(*tokens, count);
	init_args_input_output_file(&cmd, count);
	while (tokens != NULL && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
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
		pipe = (t_ast_node *)malloc(sizeof(t_ast_node));
		pipe->type = AST_PIPE;
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
