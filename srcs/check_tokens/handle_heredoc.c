/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:55:21 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/26 10:34:48 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*special_expand_char(char *str, char **env, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;

	i = -1;
	tmp = NULL;
	result = NULL;
	while (str[++i])
	{
		if (str[i] == '$'
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '?'))
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

static void	process_heredoc(char *heredoc_delimiter, char **env, int exit_status
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
			result = special_expand_char(line, env, exit_status);
		else
			result = ft_strdup(line);
		if (is_delimiter(line, heredoc_delimiter))
		{
			free(line);
			if (result != NULL)
				free(result);
			break ;
		}
		ft_putendl_fd(result, fd);
		free(line);
		free(result);
	}
	close(fd);
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
		else if (heredoc_delimiter[i] == '$' && (heredoc_delimiter[i + 1]
				== '\'' || heredoc_delimiter[i + 1] == '"'))
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

int	handle_heredoc(t_token **current_token, char **env, int exit_status)
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
