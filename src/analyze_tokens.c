/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:14:45 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/12 10:02:53 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redirection(t_tokentype type)
{
	if (type == TOKEN_HEREDOC || type == TOKEN_REDIR_APPEND ||
		type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
		type == TOKEN_REDIR_IN_OUT)
		return (1);
	return (0);
}

char *expand_line(char *str, char **env, int exit_status)
{
    char	*result;
    char	*tmp;
    int		i;

	tmp = NULL;
	result = NULL;
	i = 0;
    while (str[i])
    {
		if (str[i] == '$' &&
			(ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
        {
            tmp = expand_special_char(str, env, &i, exit_status);
            result = str_append(result, tmp);
        }
        else
            result = char_append(result, str[i]);
        i++;
    }
    return (result);
}

void	process_heredoc(char *heredoc_delimiter, char **env, int exit_status)
{
	char	*result;
	char	*line;
	int		fd;

	fd = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	result = NULL;
	while (1 && fd > 0)
	{
		line = readline("heredoc> ");
		if (line != NULL)
			result = expand_line(line, env, exit_status);
		if (ft_strncmp(line, heredoc_delimiter,
			ft_strlen(heredoc_delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(result, fd);
		free(line);
		free(result);
	}
	if (fd > 0)
		close(fd);
}

static int	check_redirection(t_token **current_token, char **env, int exit_status)
{
	if ((*current_token)->type == TOKEN_HEREDOC)
	{
		*current_token = (*current_token)->next;
		if (*current_token == NULL || (*current_token)->type != TOKEN_WORD)
		{
			ft_putstr_fd(RED"Syntax error: expected filename after redirection\n"
				RESET, 2);
			return (0);
		}
		process_heredoc((*current_token)->value, env, exit_status);
		return (1);
	}
	*current_token = (*current_token)->next;
	if (*current_token == NULL || (*current_token)->type != TOKEN_WORD)
	{
		ft_putstr_fd(RED"Syntax error: expected filename after redirection\n"
			RESET, 2);
		return (0);
	}
	return (1);
}

int	check_pipe(t_token *current_token, int	arg_count)
{
	if (current_token->next == NULL || arg_count == 0 ||
		current_token->next->type == TOKEN_EOF)
	{
		ft_putstr_fd(RED"Syntax error: expected command after pipe.\n"
			RESET, 2);
		return (0);
	}
	return (1);
}

int	analyze_tokens(t_token *tokens, char **env, int exit_status)
{
	t_token	*current;
	int		arg_count;

	current = tokens;
	arg_count = 0;
	while (current != NULL && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			arg_count++;
		if (is_redirection(current->type))
		{
			if (check_redirection(&current, env, exit_status) == 0)
				return (0);
		}
		if (current->type == TOKEN_PIPE)
		{
			if (check_pipe(current, arg_count) == 0)
				return (0);
			arg_count = 0;
		}
		current = current->next;
	}
	if (arg_count == 0)
		return (0);
	return (1);
}
