/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:14:45 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/18 15:11:04 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_redirection(t_tokentype type)
{
	if (type == TOKEN_HEREDOC || type == TOKEN_REDIR_APPEND ||
		type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
		type == TOKEN_REDIR_IN_OUT)
		return (1);
	return (0);
}
static int	is_delimiter(const char *line, const char *heredoc_delimiter)
{
	if (ft_strncmp(line, heredoc_delimiter,
		ft_strlen(heredoc_delimiter) + 1) == 0)
		return (1);
	return (0);
}

void	process_heredoc(char *heredoc_delimiter, char **env, int exit_status
	, int is_expand)
{
	char	*result;
	char	*line;
	int		fd;

	fd = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	result = NULL;
	while (1 && fd > 0)
	{
		line = readline("heredoc> ");
		if (line != NULL && is_expand == 0)
			result = expand_token(line, env, exit_status);
		else
			result = ft_strdup(line);
		if (is_delimiter(line, heredoc_delimiter))
		{
			free(line);
			free(result);
			break;
		}
		ft_putendl_fd(result, fd);
		free(line);
		free(result);
	}
	if (fd > 0)
		close(fd);
}

static int	is_invalid_redirection(t_token *token)
{
	return (token == NULL || token->type != TOKEN_WORD);
}

static int	print_syntax_error(void)
{
	ft_putstr_fd(RED"Syntax error: expected filename after redirection"
		" or expected delimiter after heredoc\n" RESET, 2);
	return (0);
}

static int	determine_expansion(t_token *current_token, char *tmp)
{
	if (mns_strcmp(current_token->value, tmp) == 0)
		return (0);
	return (1);
}

char	*expand_for_heredoc(char *heredoc_delimiter)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (heredoc_delimiter[i] != '\0')
	{
		if (heredoc_delimiter[i] == '\'' || heredoc_delimiter[i] == '"')
			i++;
		else if (heredoc_delimiter[i] == '$' && (heredoc_delimiter[i + 1] == '\''
			|| heredoc_delimiter[i + 1] == '"'))
			i++;
		else
		{
			result = char_append(result, heredoc_delimiter[i]);
			i++;
		}
	}
	if (result == NULL)
		return (ft_strdup(""));
	return (result);
}

static int	handle_heredoc(t_token **current_token, char **env, int exit_status)
{
	char	*tmp;
	int		is_expand;

	*current_token = (*current_token)->next;
	if (is_invalid_redirection(*current_token))
		return (print_syntax_error());
	tmp = expand_for_heredoc((*current_token)->value);
	is_expand = determine_expansion(*current_token, tmp);
	process_heredoc(tmp, env, exit_status, is_expand);
	free(tmp);
	return (1);
}

static int	check_redirection(t_token **current_token, char **env, int exit_status)
{
	if ((*current_token)->type == TOKEN_HEREDOC)
		return (handle_heredoc(current_token, env, exit_status));
	*current_token = (*current_token)->next;
	if (is_invalid_redirection(*current_token))
		return (print_syntax_error());
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
