/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:09:13 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/14 11:12:24 by trarijam         ###   ########.fr       */
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

int	process_heredoc_redir(char *heredoc_delimiter, char **env, int exit_status
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
		if (line == NULL)
		{
			if (g_sigint_received)
			{
				if (result != NULL)
				{
					//free(result);
					close(fd);
				}
				printf("Misy olona ato\n");
				return (130);
			}
			return (-2);
		}
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
	return (1);
}

int	heredoc(t_data *data, int exit_status)
{
	char	*tmp_value;
	pid_t	pid_child;
	int		is_expand;
	t_token	*tmp_token;
	t_token	*token;
	int		status;
	struct sigaction sa;

	pid_child = fork();
	if (pid_child == 0)
	{
		sigemptyset(&sa.sa_mask);
		status = 1;
		sa.sa_flags = 0;
		sa.sa_handler = handler_sigint_heredoc;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
		token = lexer(data->line);
		if (token == NULL)
			exit (status);
		tmp_token = token;
		while (tmp_token)
		{
			if (tmp_token->type == TOKEN_HEREDOC)
			{
				tmp_token = tmp_token->next;
				if (is_invalid_redirection(tmp_token))
					return (status);
				tmp_value = expand_for_heredoc(tmp_token->value);
				is_expand = determine_expansion(tmp_token, tmp_value);
				status = process_heredoc_redir(tmp_value, data->envp, exit_status, is_expand);
				free(tmp_value);
			}
			tmp_token = tmp_token->next;
		}
		free_token(token);
		free_split(data->envp);
		free(data->line);
		exit (status);
	}
	else
	{
		sigaction(SIGINT, &data->sa_ignore, NULL);
		wait(&status);
		sigaction(SIGINT, &data->sa, NULL);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == -130)
			{
				return (-130);
			}
			else
				return (WEXITSTATUS(status));
		}
		return (0);
	}	
}