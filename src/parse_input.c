/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:04:33 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/16 09:08:25 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_ast(t_ast_node **node)
{
	int			i;

	if (node == NULL || *node == NULL)
		return ;
	if ((*node)->type != AST_PIPE && (*node)->args != NULL)
	{
		i = 0;
		while ((*node)->args[i] != NULL)
		{
			free((*node)->args[i]);
			i++;
		}
		free((*node)->args);
	}
	if ((*node)->type != AST_PIPE && (*node)->input_file != NULL)
		free((*node)->input_file);
	if ((*node)->type != AST_PIPE && (*node)->output_file != NULL)
		free((*node)->output_file);
	if ((*node)->type != AST_PIPE && (*node)->heredoc_delimiter != NULL)
		free((*node)->heredoc_delimiter);
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
    node->input_file = NULL;
    node->output_file = NULL;
	node->append_output = -1;
	node->heredoc_delimiter = NULL;
    return (node);
}

void	block_if_for_parse_redirection(t_token **tokens, t_ast_node *cmd,
	t_ast_node_type type)
{
	cmd->type = type;
	*tokens = (*tokens)->next;
	if (type == AST_REDIR_IN)
		cmd->input_file = ft_strdup((*tokens)->value);
	else if (type == AST_REDIR_OUT)
	{
		cmd->output_file = ft_strdup((*tokens)->value);
		cmd->append_output = 0;
	}
	else if (type == AST_REDIR_HERDOC)
		cmd->heredoc_delimiter = ft_strdup((*tokens)->value);
	else if (type == AST_REDIR_APPEND)
	{
		cmd->output_file = ft_strdup((*tokens)->value);
		cmd->append_output = 1;
	}
}

t_ast_node	*parse_redirection(t_token **tokens, t_ast_node *cmd)
{
	while (*tokens != NULL && (((*tokens)->type == TOKEN_HEREDOC || 
		(*tokens)->type == TOKEN_REDIR_APPEND ||
		(*tokens)->type == TOKEN_REDIR_IN ||
		(*tokens)->type == TOKEN_REDIR_OUT)))
	{
		if ((*tokens)->type == TOKEN_REDIR_IN && (*tokens)->next->type == 1)
			block_if_for_parse_redirection(tokens, cmd, AST_REDIR_IN);
		if ((*tokens)->type == TOKEN_REDIR_OUT && (*tokens)->next->type == 1)
			block_if_for_parse_redirection(tokens, cmd, AST_REDIR_OUT);
		if ((*tokens)->type == 5 && (*tokens)->next->type == 1)
			block_if_for_parse_redirection(tokens, cmd, AST_REDIR_APPEND);
		if ((*tokens)->type == TOKEN_HEREDOC && (*tokens)->next->type == 1)
			block_if_for_parse_redirection(tokens, cmd, AST_REDIR_HERDOC);
		*tokens = (*tokens)->next;
	}
	return (cmd);
}

int	token_is_redir(t_token *token)
{
	if (token->type == TOKEN_HEREDOC || token->type == TOKEN_REDIR_APPEND
		|| token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT)
		return (1);
	return (0);
}

char	**get_args(t_token **tokens)
{
	int			i;
	int			arg_count;
	char		**args;
	t_token		*current;

	arg_count = 0;
	current = *tokens;
	while (current != NULL && current->type == TOKEN_WORD)
	{
		arg_count++;
		current = current->next;
	}
	args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	i = 0;
	while (*tokens != NULL && i < arg_count)
	{
		args[i] = ft_strdup((*tokens)->value);
		(*tokens) = (*tokens)->next;
		i++;
	}
	args[arg_count] = NULL;
	return (args);
}

t_ast_node	*parse_token(t_token **tokens, t_ast_node *cmd)
{
	while (*tokens != NULL && ((*tokens)->type == TOKEN_WORD ||
		token_is_redir(*tokens) == 1))
	{
		if (token_is_redir(*tokens) == 1)
			cmd = parse_redirection(tokens, cmd);
		else
			cmd->args = get_args(tokens);
	}
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
