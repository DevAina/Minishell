/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:32:05 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/12 15:55:49 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_var_name_length(char *str)
{
	int i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
    	i++;
    return (i);
}

char	*get_env_value(char *var, char **env)
{
    int i = 0;
    int var_len;

    var_len = ft_strlen(var);
    while (env[i])
    {
        if (ft_strncmp(env[i], var, var_len) == 0
            && env[i][var_len] == '=')
            return (env[i] + var_len + 1);
        i++;
    }
    return (NULL);
}

char	*str_append(char *str, char *append)
{
    char *result;
    int len1;
    int len2;

	len1 = 0;
	if (str != NULL)
		len1 = ft_strlen(str);
	len2 = ft_strlen(append);
    result = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
    if (str)
        ft_strlcpy(result, str, (len1 + 1));
    ft_strlcpy(result + len1, append, len2 + 1);
    free(str);
    return (result);
}

char	*char_append(char *str, char c)
{
    char	*result;
	int		len;

	len = 0;
	if (str != NULL)
		len = ft_strlen(str);
    result = (char *)malloc(sizeof(char) * (len + 2));
    if (str)
        ft_strlcpy(result, str, (len + 1));
    result[len] = c;
    result[len + 1] = '\0';
    free(str);
    return (result);
}


char	*expand_env_var(char *var_name, char **env)
{
    int		var_len;
    char	*var;
    char	*value;

	var_len = get_var_name_length(var_name);
	var = (char *)malloc(sizeof(char) * (var_len + 1));
	ft_strlcpy(var, var_name, (var_len + 1));
	value = get_env_value(var, env);
    free(var);
	if (value == NULL)
		return (NULL);
    return (value);
}

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

void	init_for_expand_token(int *i, int *in_single_quotes,
	int	*in_double_quotes, char **result)
{
	*result = NULL;
	*i = -1;
	*in_single_quotes = 0;
	*in_double_quotes = 0;	
}


char *expand_token(char *str, char **env, int exit_status)
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
        else if (str[i] == '$' && !in_single_quotes &&
			(ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
        {
            tmp = expand_special_char(str, env, &i, exit_status);
            result = str_append(result, tmp);
			free(tmp);
        }
        else
            result = char_append(result, str[i]);
    }
    return (result);
} 

void expand_tokens(t_token *tokens, char **env, int exit_status)
{
    t_token	*current;
    int     skip_next;
    char    *expanded;

    current = tokens;
    skip_next = 0;
    while (current)
    {
        if (skip_next == 1)
        {
            skip_next = 0;
            current = current->next;
            continue;
        }
        if (current->type == TOKEN_HEREDOC)
            skip_next = 1;
        else if (current->type == TOKEN_WORD ||
			current->type == TOKEN_ASSIGNEMENT)
        {
            expanded = expand_token(current->value, env, exit_status);
            free(current->value);
            current->value = expanded;
        }
        current = current->next;
    }
}
