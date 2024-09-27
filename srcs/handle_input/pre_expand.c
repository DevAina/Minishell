/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:24:19 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/27 11:30:22 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*expand_line(char *line, char **env, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	tmp = NULL;
	init_for_expand_token(&i, &in_single_quotes, &in_double_quotes, &result);
	while (line[++i])
	{
		if (line[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (line[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (should_expand(line, i, in_single_quotes))
		{
			tmp = expand_special_char(line, env, &i, exit_status);
			result = str_append(result, tmp);
			free(tmp);
		}
		else if (is_ignored_dollar(line[i], line[i + 1],
				in_double_quotes, in_single_quotes) == 1)
			result = char_append(result, line[i]);
	}
	return (finalize_result(result));
}
