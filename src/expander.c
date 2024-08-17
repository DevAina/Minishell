/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 20:58:23 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/17 16:15:01 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int get_var_name_length(char *str)
{
	int i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
    	i++;
    return (i);
}

char *get_env_value(char *var, char **env)
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

char *str_append(char *str, char *append)
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

char *char_append(char *str, char c)
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


char *expand_env_var(char *var_name, char **env)
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
		return (ft_strdup(""));
    return (value);
}

char *expand_token(char *str, char **env)
{
    char	*result;
    int		i;
    int		in_single_quotes;
	char	*var_value;

    i = 0;
    in_single_quotes = 0;
    result = NULL;
    while (str[i])
    {
        if (str[i] == '\'')
            in_single_quotes = !in_single_quotes;
        else if (str[i] == '$' && !in_single_quotes)
        {
            var_value = expand_env_var(str + i + 1, env);
            result = str_append(result, var_value);
            free(var_value);
            i += get_var_name_length(str + i + 1);
        }
        else
            result = char_append(result, str[i]);
        i++;
    }
    return (result);
}

void expand_tokens(t_token *tokens, char **env)
{
    t_token *current;
    char    *expanded;

    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_WORD)
        {
            expanded = expand_token(current->value, env);
            free(current->value);
            current->value = expanded;
        }
        current = current->next;
    }
}
