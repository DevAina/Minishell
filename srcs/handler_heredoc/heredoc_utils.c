/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:45:04 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/15 08:47:51 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	wait_process(t_data *data)
{
	int	status;

	sigaction(SIGINT, &data->sa_ignore, NULL);
	wait(&status);
	sigaction(SIGINT, &data->sa, NULL);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
			return (130);
		else
			return (WEXITSTATUS(status));
	}
	return (0);
}

void	setup_signals_heredoc(struct sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sa->sa_handler = handler_sigint_heredoc;
	sigaction(SIGINT, sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	process_heredoc_token(t_token **current, t_data *data, int exit_status)
{
	char	*tmp_value;
	int		status;
	int		is_expand;

	tmp_value = NULL;
	*current = (*current)->next;
	if (is_invalid_redirection(*current))
		return (-2);
	tmp_value = expand_for_heredoc((*current)->value);
	is_expand = determine_expansion(*current, tmp_value);
	status = process_heredoc_redir(tmp_value, data->envp, exit_status, is_expand);
	free(tmp_value);
	return (status);
}

char	*get_result(char *line, int is_expand, char **env, int exit_status)
{
	char	*result;

	result = NULL;
	if (line != NULL && is_expand == 0)
		result = special_expand_char(line, env, exit_status);
	else
		result = ft_strdup(line);
	return (result);
}

int	check_delimiter(char *heredoc_delimiter, char *result, char *line)
{
	if (is_delimiter(heredoc_delimiter, result))
	{
		free(line);
		if (result != NULL)
			free(result);
		return (1);
	}
	return (0);
}