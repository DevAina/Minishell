/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:42:33 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/08 10:59:50 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_token_split(t_token **token, t_token **last,
	t_token *new_token)
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

static t_token	*split_word(char *word)
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

t_token	*process_word(t_token *current, t_expand_params *params)
{
	t_token			*new_token;
	int				tmp;
	t_expand_result	expanded;

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
