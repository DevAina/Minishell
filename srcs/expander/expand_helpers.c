/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:12:59 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/26 10:35:13 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes(char c, int *in_single_quotes, int *in_double_quotes)
{
	if (c == '\'' && !(*in_double_quotes))
		*in_single_quotes = !(*in_single_quotes);
	else if (c == '"' && !(*in_single_quotes))
		*in_double_quotes = !(*in_double_quotes);
}

char	*expand_special_char(char *str, char **env, int *i, int exit_status)
{
	char	*result;
	char	*var_value;

	result = NULL;
	var_value = NULL;
	if (str[*i + 1] == '?')
	{
		result = ft_itoa(exit_status);
		(*i)++;
	}
	else
	{
		var_value = expand_env_var(str + *i + 1, env);
		result = ft_strdup(var_value);
		*i += get_var_name_length(str + *i + 1);
	}
	return (result);
}

int	should_expand(char *str, int i, int in_single_quotes)
{
	return (str[i] == '$' && !in_single_quotes
		&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'));
}
