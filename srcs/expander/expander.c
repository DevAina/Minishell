/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:32:05 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/28 10:06:08 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_for_expand_token(int *i, int *in_single_quotes,
	int	*in_double_quotes)
{
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

t_token	*split_word(char *word)
{
	char	**split;
	int		i;
	int		index_tmp;
	t_token	*token;
	t_token	*tmp;
	t_token	*last;

	token = NULL;
	last = NULL;
	i = 0;
	split = ft_split(word, ' ');
	while (split[i] != NULL)
	{
		tmp = create_token(TOKEN_WORD, split[i], &index_tmp, -1);
		if (token == NULL)
		{
			token = tmp;
			last = tmp;
		}
		else
		{
			last->next = tmp;
			last = tmp;
		}
		i++;
	}
	free_split(split);
	return (token);
}

t_expand_result	expand_token(char *str, char **env, int exit_status)
{
	t_expand_result	result;
	char			*tmp;
	int				i;
	int				in_single_quotes;
	int				in_double_quotes;

	tmp = NULL;
	result.value = NULL;
	result.create_token = 0;
	init_for_expand_token(&i, &in_single_quotes, &in_double_quotes);
	while (str[++i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (should_expand(str, i, in_single_quotes))
		{
			tmp = expand_special_char(str, env, &i, exit_status);
			result.value = str_append(result.value, tmp);
			if (!in_double_quotes)
			{
				result.create_token = 1;
			}
			free(tmp);
		}
		else if (is_ignored_dollar(str[i], str[i + 1],
				in_double_quotes, in_single_quotes) == 1)
			result.value = char_append(result.value, str[i]);
	}
	result.value = finalize_result(result.value);
	return (result);
}

t_token	*expand_tokens(t_token *tokens, char **env, int exit_status)
{
	t_token			*expanded_tokens;
	t_token			*new_token;
	t_token			*current;
	t_token			*last;
	int				skip_next;
	t_expand_result	expanded;

	current = tokens;
	expanded_tokens = NULL;
	last = NULL;
	new_token = NULL;
	skip_next = 0;
	while (current)
	{
		if (skip_next == 1)
		{
			skip_next = 0;
			new_token = create_token(current->type, current->value, &expanded.create_token, -1);
			if (!expanded_tokens)
				expanded_tokens = new_token;
			else
				last->next = new_token;
			last = new_token;
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_HEREDOC)
		{
			skip_next = 1;
			new_token = create_token(current->type, current->value, &expanded.create_token, -1);
			if (!expanded_tokens)
				expanded_tokens = new_token;
			else
				last->next = new_token;
			last = new_token;
		}
		else if (current->type == TOKEN_WORD)
		{
			expanded = expand_token(current->value, env, exit_status);
			if (expanded.create_token == 1)
				new_token = split_word(expanded.value);
			else
				new_token = create_token(TOKEN_WORD, expanded.value, &expanded.create_token, -1);
			if (!expanded_tokens)
				expanded_tokens = new_token;
			else
				last->next = new_token;
			while (new_token && new_token->next)
				new_token = new_token->next;
			last = new_token;
			free(expanded.value);
		}
		else
		{
			new_token = create_token(current->type, current->value, &expanded.create_token, -1);
			if (!expanded_tokens)
				expanded_tokens = new_token;
			else
				last->next = new_token;
			last = new_token;
		}
		current = current->next;
	}
	return (expanded_tokens);
}
