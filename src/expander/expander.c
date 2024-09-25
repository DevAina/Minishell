/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:32:05 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/25 14:26:50 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	init_for_expand_token(int *i, int *in_single_quotes,
	int	*in_double_quotes, char **result)
{
	*result = NULL;
	*i = -1;
	*in_single_quotes = 0;
	*in_double_quotes = 0;
}

static	char	*finalize_result(char *result)
{
	if (result == NULL)
		return (ft_strdup(""));
	return (result);
}

static int	is_ignored_dollar(char current, char next,
	int in_double_quotes, int in_single_quotes)
{
	if (current == '$' && (next == '\'' || next == '"')
		&& (in_double_quotes || in_single_quotes))
		return (1);
	if (current == '$' && (next == '\'' || next == '"'))
		return (0);
	return (1);
}

char	*expand_token(char *str, char **env, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	tmp = NULL;
	init_for_expand_token(&i, &in_single_quotes, &in_double_quotes, &result);
	while (str[++i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (should_expand(str, i, in_single_quotes))
		{
			tmp = expand_special_char(str, env, &i, exit_status);
			result = str_append(result, tmp);
			free(tmp);
		}
		else if (is_ignored_dollar(str[i], str[i + 1],
				in_double_quotes, in_single_quotes) == 1)
			result = char_append(result, str[i]);
	}
	return (finalize_result(result));
}

void	expand_tokens(t_token *tokens, char **env, int exit_status)
{
	t_token	*current;
	int		skip_next;
	char	*expanded;

	current = tokens;
	skip_next = 0;
	while (current)
	{
		if (skip_next == 1)
		{
			skip_next = 0;
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_HEREDOC)
			skip_next = 1;
		else if (current->type == TOKEN_WORD
			|| current->type == TOKEN_ASSIGNEMENT)
		{
			expanded = expand_token(current->value, env, exit_status);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}
