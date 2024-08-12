/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 10:03:38 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/12 12:15:50 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int special_char(char c)
{
    if (c == ' ' || c == '>' || c == '<' || c == '|' || c == '\t')
        return (1);
    return (0);
}

int mns_is_space(char c)
{
    if ((c >= 9 && c <= 13) || c == ' ')
        return (1);
    return (0);
}

t_token *create_token(t_tokentype type, char *value, int *index)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (token == NULL)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    token->next = NULL;
    *index += ft_strlen(value);
    return (token);
}

t_token *handle_quote(char *input, int *index)
{
    t_token *token;
    char    quote;
    int     start;
    int     end;
    char    *value;

    quote = input[*index];
    *index += 1;
    start = *index;
    end = start;
    while (input[end] != '\0' && input[end] != '\'')
        end++;
    if (input[end] != quote)
    {
        //gestion d'erreur
        return (NULL);
    }
    value = (char *)malloc(sizeof(char ) * (end - start + 2));
    ft_strlcpy(value, input, (end - start + 2));
    if (quote == '\'')
        token = create_token(TOKEN_QUOTE, value, index);
    else
        token = create_token(TOKEN_DOUBLE_QUOTE, value, index);
    free(value);
    return (token);
}

t_token *create_token_word(char *input, int *index)
{
    int start;
    int end;

    start = *index;
    end = start;
    while (input[end] != '\0' && special_char(input[end]) == 1)
        end++;
}

t_token *get_next_token(char *input, int *index)
{
    if (input[*index] == '|')
        return (create_token(TOKEN_PIPE, "|", index));
    else if (input[*index] == '>')
    {
        if (input[*index + 1] == '>')
            return (create_token(TOKEN_REDIR_APPEND, ">>", index));
        return (create_token(TOKEN_REDIR_OUT, ">", index));
    }
    else if (input[*index] == '<')
    {
        if (input[*index + 1] = '<')
            return (create_token(TOKEN_HEREDOC, "<<", index));
        return (create_token(TOKEN_REDIR_IN, "<", index));
    }
    else if (input[*index] == '"' || input[*index] == '\'')
        return (handle_quote(input, index));
    else
        return (create_token_word(input, index));
}

t_token *tokenizer(char *input)
{
    t_token *head;
    t_token *current;
    t_token *new_token;
    size_t  *len_input;
    size_t  i;

    head = NULL;
    i = 0;
    len_input = ft_strlen(input);
    while (i < len_input)
    {
        if (mns_is_space(input[i]))
        {
            i++;
            continue;
        }
        new_token = get_next_token(input, &i);
        if (head == NULL)
        {
            head = new_token;
            current = new_token;
        }
        else
        {
            current->next = new_token;
            current = new_token;
        }
    }

}