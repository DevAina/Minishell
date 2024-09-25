/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:14:45 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/25 17:19:35 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	print_syntax_error(void)
{
	ft_putstr_fd(RED"Syntax error: expected filename after redirection"
		" or expected delimiter after heredoc\n" RESET, 2);
	return (0);
}

static int	check_redirection(t_token **current_token,
							char **env, int exit_status)
{
	if ((*current_token)->type == TOKEN_HEREDOC)
		return (handle_heredoc(current_token, env, exit_status));
	*current_token = (*current_token)->next;
	if (is_invalid_redirection(*current_token))
		return (print_syntax_error());
	return (1);
}

static int	check_pipe(t_token *current_token, int arg_count)
{
	if (current_token->next == NULL || arg_count == 0
		|| current_token->next->type == TOKEN_EOF)
	{
		ft_putstr_fd(RED"Syntax error: expected command after pipe.\n"
			RESET, 2);
		return (0);
	}
	return (1);
}

int	analyze_tokens(t_token *tokens, char **env, int exit_status)
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
			if (check_redirection(&current, env, exit_status) == 0)
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
