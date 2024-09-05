/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:14:45 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/05 13:52:04 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redirection(t_tokentype type)
{
	if (type == TOKEN_HEREDOC || type == TOKEN_REDIR_APPEND ||
		type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
		type == TOKEN_REDIR_IN_OUT)
		return (1);
	return (0);
}

/*
	Checks whether after a redirection token there is a file name(token word)
*/
static int	check_redirection(t_token **current_token)
{
	*current_token = (*current_token)->next;
	if (*current_token == NULL || (*current_token)->type != TOKEN_WORD)
	{
		ft_putstr_fd(RED"Syntax error: expected filename after redirection\n"
			RESET, 2);
		return (0);
	}
	return (1);
}

int	check_pipe(t_token *current_token, int	arg_count)
{
	if (current_token->next == NULL || arg_count == 0 ||
		current_token->next->type == TOKEN_EOF)
	{
		ft_putstr_fd(RED"Syntax error: expected command after pipe.\n"
			RESET, 2);
		return (0);
	}
	return (1);
}

int	analyze_tokens(t_token *tokens)
{
	t_token	*current;
	int		arg_count;

	current = tokens;
	arg_count = 0;
	while (current != NULL && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			arg_count++;
		if (is_redirection(current->type))
		{
			if (check_redirection(&current) == 0)
				return (0);
		}
		if (current->type == TOKEN_PIPE)
		{
			if (check_pipe(current, arg_count) == 0)
				return (0);
			arg_count = 0;
		}
		current = current->next;
	}
	return (1);
}
