/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:09:13 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/14 15:58:27 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_sigint_received = 0;

void	handler_sigint_heredoc(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	write(1, "\n", 1);
	close(STDIN_FILENO);
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

int	init_heredoc(char **line, int fd)
{
	*line = readline("heredoc> ");
	if (*line == NULL)
	{
		if (g_sigint_received)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		return (-2);
	}
	return (1);
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

int	process_heredoc_redir(char *heredoc_delimiter, char **env, int exit_status,
	int is_expand)
{
	char	*line;
	char	*result;
	int		fd;

	fd = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	line = NULL;
	result = NULL;
	while (1 && fd > 0)
	{
		if (init_heredoc(&line, fd) < 0)
		{
			if (g_sigint_received)
				return (130);
			return (166);
		}
		result = get_result(line, is_expand, env, exit_status);	
		if (check_delimiter(heredoc_delimiter, result, line))
			break ;
		ft_putendl_fd(result, fd);
		free(line);
		free(result);
	}
	close(fd);
	return (1);
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

int	process_tokens_heredoc(t_token *token, t_data *data, int exit_status)
{
	int	status;
	t_token	*tmp_token;

	status = 0;
	tmp_token = token;
	while (tmp_token)
	{
		if (tmp_token->type == TOKEN_HEREDOC)
			status = process_heredoc_token(&tmp_token, data, exit_status);
		if (status == -2)
			break;
		tmp_token = tmp_token->next;
	}
	return (status);
}

void	setup_signals_heredoc(struct sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sa->sa_handler = handler_sigint_heredoc;
	sigaction(SIGINT, sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_child_process(t_data *data, int exit_status)
{
	struct sigaction sa;
	t_token			*token;
	int				status;

	g_sigint_received = 0;
	setup_signals_heredoc(&sa);
	status = 1;
	token = lexer(data->line);
	if (token == NULL)
		exit(status);
	status = process_tokens_heredoc(token, data, exit_status);
	free_token(token);
	free_split(data->envp);
	free(data->line);
	exit(status);
}

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

int	heredoc(t_data *data, int exit_status)
{
	pid_t	pid_child;

	pid_child = fork();
	if (pid_child == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (pid_child == 0)
		handle_child_process(data, exit_status);
	else
		return (wait_process(data));
	return (0);
}