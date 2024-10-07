/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:32:05 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/07 14:36:43 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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


void	process_token_split(t_token **token, t_token **last, t_token *new_token)
{
	if (*token == NULL)
	{
		*token = new_token;
		*last = new_token;
	}
	else
	{
		(*last)->next = new_token;
		*last = new_token;
	}
}

t_token	*split_word(char *word)
{
	char	**split;
	int		i;
	t_token	*token;
	t_token	*last;

	token = NULL;
	last = NULL;
	i = 0;
	split = ft_split(word, ' ');
	if (split[i] == NULL)
		return (free_split(split), create_token(TOKEN_WORD, "", &i, -1));
	while (split[i] != NULL)
	{
		process_token_split(&token, &last,
			create_token(TOKEN_WORD, split[i], NULL, -1));
		i++;
	}
	free_split(split);
	return (token);
}

void	init_expand_vars(t_expand_result *result, int *i)
{
	*i = -1;
	result->value = NULL;
	result->create_token = 0;
	result->prev = 0;
}

void	init_expand_context(t_expand_context *ctx, t_expand_result *result, int exit_status)
{
	ctx->result = result;
	ctx->exit_status = exit_status;
	ctx->in_single_quotes = 0;
	ctx->in_double_quotes = 0;
}

int	handle_quotes_expander(char c, int *in_single_quotes, int *in_double_quotes)
{
	if (c == '\'' && !(*in_double_quotes))
	{
		*in_single_quotes = !(*in_single_quotes);
		return (1);
	}
	else if (c == '"' && !(*in_single_quotes))
	{
		*in_double_quotes = !(*in_double_quotes);
		return (1);
	}
	return (0);
}

void	expand_char(t_expand_context *ctx)
{
	char	*tmp;

	tmp = expand_special_char(ctx->str, ctx->env, ctx->i, ctx->exit_status);
	ctx->result->value = str_append(ctx->result->value, tmp);
	if (!ctx->in_double_quotes && ctx->str[*(ctx->i) - 1] != '=')
		ctx->result->create_token = 1;
	free(tmp);
}

void	handle_non_expand_char(t_expand_context *ctx, char *str, int i)
{
	if (is_ignored_dollar(str[i], str[i + 1], ctx->in_double_quotes,
		ctx->in_single_quotes) == 1)
		ctx->result->value = char_append(ctx->result->value, str[i]);
}

t_expand_result	expand_token(char *str, char **env, int exit_status)
{
	t_expand_result		result;
	t_expand_context	ctx;
	int					i;

	init_expand_vars(&result, &i);
	init_expand_context(&ctx, &result, exit_status);
	ctx.str = str;
	ctx.env = env;
	ctx.i = &i;
	while (str[++i])
	{
		if (handle_quotes_expander(str[i], &ctx.in_single_quotes,
			&ctx.in_double_quotes) == 1)
		{
			
		}
		else if (should_expand(str, i, ctx.in_single_quotes))
			expand_char(&ctx);
		else
			handle_non_expand_char(&ctx, str, i);
		ctx.result->prev = str[i];
	}
	result.value = finalize_result(result.value);
	return (result);
}

t_token	*add_new_token(t_token *expanded_tokens, t_token *new_token,
	t_token **last)
{
	if (!expanded_tokens)
        expanded_tokens = new_token;
	else
        (*last)->next = new_token;
    *last = new_token;
    return (expanded_tokens);
}

t_token	*process_heredoc(t_token *current, t_token *expanded_tokens,
	t_token **last)
{
    t_token	*new_token;
	int		tmp;

    new_token = create_token(current->type, current->value, &tmp, -1);
    expanded_tokens = add_new_token(expanded_tokens, new_token, last);
    return (expanded_tokens);
}

t_token *process_word(t_token *current, t_expand_params *params)
{
    t_token         *new_token;
	int				tmp;
    t_expand_result expanded;

    expanded = expand_token(current->value, params->env, params->exit_status);
    if (expanded.create_token == 1)
        new_token = split_word(expanded.value);
    else
        new_token = create_token(TOKEN_WORD, expanded.value, &tmp, -1);
    params->expanded_tokens = add_new_token(params->expanded_tokens,
			new_token, params->last);
    while (new_token && new_token->next)
        new_token = new_token->next;
    *(params->last) = new_token;
    free(expanded.value);
    return (params->expanded_tokens);
}

t_token *expand_tokens(t_token *tokens, char **env, int exit_status)
{
	t_token			*expanded_tokens;
	t_token			*current;
	t_token			*last;
	int				skip_next;
	t_expand_params	params;

	current = tokens;
	expanded_tokens = NULL;
	last = NULL;
	skip_next = 0;
	params.env = env;
	params.exit_status = exit_status;
	params.expanded_tokens = expanded_tokens;
	params.last = &last;
    while (current)
    {
        if (skip_next)
        {
            skip_next = 0;
            expanded_tokens = process_heredoc(current, expanded_tokens, &last);
        }
        else if (current->type == TOKEN_HEREDOC)
            skip_next = 1;
        else if (current->type == TOKEN_WORD)
            expanded_tokens = process_word(current, &params);
        else
            expanded_tokens = process_heredoc(current, expanded_tokens, &last);
        current = current->next;
    }
    return expanded_tokens;
}
