/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 09:49:05 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/16 09:35:40 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token != NULL)
	{
		printf("token value: %s\n", token->value);
		tmp = token;
		token = token->next;
		free(tmp->value);
		free(tmp);
	}
}

t_token	*create_token(t_tokentype type, const char *value, int *index)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	*index += ft_strlen(value);
	return (token);
}

int	mns_is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int is_special_char(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == ' ')
		return (1);
	return (0);
}

static void	block_if_for_get_len_word(char *input, int index, 
	int	*in_quotes, char *type_quotes)
{
	if (!*in_quotes)
	{
		*in_quotes = 1;
		*type_quotes = input[index];
	}
	else if (input[index] == *type_quotes)
	{
		*in_quotes = 0;
		*type_quotes = 0;
	}
}

int	get_len_word(char *input, int *index)
{
	int		start;
	int		end;
	int		in_quotes;
	char	type_quotes;

	in_quotes = 0;
	type_quotes = 0;
	start = *index;
	end = start;
	while (input[end] != '\0')
	{
		if (!in_quotes && is_special_char(input[end]) == 1)
			break ;
		if (input[end] == '\'' || input[end] == '"')
			block_if_for_get_len_word(input, end, &in_quotes, &type_quotes);
		end++;
	}
	return (end - start + 1);
}

t_token	*get_token_word(char *input, int *index)
{
	t_token	*token;
	char	*word;
	int		len;

	len = get_len_word(input, index);
	word = malloc(sizeof(char) * len);
	ft_strlcpy(word, input + *index, len);
	if (word == NULL)
		return (NULL);
	token = create_token(TOKEN_WORD, word, index);
	free(word);
	return (token);
}

t_token	*get_next_token(char *input, int *index)
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
		if (input[*index + 1] == '<')
			return (create_token(TOKEN_HEREDOC, "<<", index));
		return (create_token(TOKEN_REDIR_IN, "<", index));
	}
	else if (input[*index] == '\n')
		return (create_token(TOKEN_NEWLINE, "\n", index));
	else if (input[*index] == '\0')
		return (create_token(TOKEN_NEWLINE, "\0", index));
	else
		return (get_token_word(input, index));
}

void	block_if_for_lexer(t_token **current, t_token **head
	, t_token *new_token)
{
	if (*head == NULL)
	{
		*head = new_token;
		*current = new_token;
	}
	else
	{
		(*current)->next = new_token;
		*current = new_token;
	}
}

/**Function for analyzing the input**/
t_token	*lexer(char *input)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;
	int		len;

	head = NULL;
	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		if (mns_is_space(input[i]))
		{
			i++;
			continue ;
		}
		new_token = get_next_token(input, &i);
		if (new_token == NULL)
			break ;
		block_if_for_lexer(&current, &head, new_token);
	}
	if (current == NULL)
		return (head);
	current->next = create_token(TOKEN_EOF, "\0", &i);
	return (head);
}
