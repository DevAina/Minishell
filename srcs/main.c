/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/27 10:54:33 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

void	handler_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	handle_built_in_cmd(t_data *data, t_ast_node *ast, char ***envp)
{
	data->fd_tmp = dup(STDOUT_FILENO);
	close(data->fd_tmp);
	if (ast->redirection)
		check_redirection_exec(ast, *envp);
	if (ft_strncmp(ast->args[0], "cd", 3) == 0)
		g_exit_status = mns_cd(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "export", 7) == 0)
		g_exit_status = ft_export(ast->args, ast->assignement, envp);
	else if (ft_strncmp(ast->args[0], "unset", 6) == 0)
		g_exit_status = ft_unset(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "exit", 5) == 0)
		g_exit_status = ft_exit(ast->args, ast, *envp, 1);
	dup2(data->fd_tmp, STDOUT_FILENO);
	close(data->fd_tmp);
}

void	wait_child_process(t_data *data)
{
	int	status;

	sigaction(SIGINT, &data->sa_ignore, NULL);
	wait(&status);
	sigaction(SIGINT, &data->sa, NULL);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		write(1, "\n", 1);
		g_exit_status = 128 + WTERMSIG(status);
	}
}

static void	init_for_expand_token(int *i, int *in_single_quotes,
	int	*in_double_quotes, char **result)
{
	*result = NULL;
	*i = -1;
	*in_single_quotes = 0;
	*in_double_quotes = 0;
}

static	char	*finalize_result(char *result)
{
	if (result == NULL)
		return (ft_strdup(""));
	return (result);
}

static int	is_ignored_dollar(char current, char next,
	int in_double_quotes, int in_single_quotes)
{
	if (current == '$' && (next == '\'' || next == '"')
		&& (in_double_quotes || in_single_quotes))
		return (1);
	if (current == '$' && (next == '\'' || next == '"'))
		return (0);
	return (1);
}

char	*expand_line(char *line, char **env, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	tmp = NULL;
	init_for_expand_token(&i, &in_single_quotes, &in_double_quotes, &result);
	while (line[++i])
	{
		if (line[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (line[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (should_expand(line, i, in_single_quotes))
		{
			tmp = expand_special_char(line, env, &i, exit_status);
			result = str_append(result, tmp);
			free(tmp);
		}
		else if (is_ignored_dollar(line[i], line[i + 1],
			in_double_quotes, in_single_quotes) == 1)
			result = char_append(result, line[i]);
	}
	return (finalize_result(result));
}

void	process_line(t_data *data)
{
	char	*line_expanded;

	uptdate_history(data);
	line_expanded = expand_line(data->line, data->envp, g_exit_status);
	data->token = lexer(line_expanded);
	if (data->token == NULL)
	{
		data->ast = NULL;
		return ;
	}
	if (analyze_tokens(data->token, data->envp, g_exit_status) == 0)
	{
		unlink(".tmp");
		free(line_expanded);
		free_token(data->token);
		return ;
	}
	free(line_expanded);
	expand_tokens(data->token, data->envp, g_exit_status);
	data->ast = parse(data->token);
	free_token(data->token);
	if (data->ast->type == AST_COMMAND && data->ast->args != NULL
		&& data->ast->args[0] == NULL)
		return ;
	if (data->ast->type == AST_COMMAND && data->ast->args != NULL
		&& check_built_in(data->ast) == 1)
		handle_built_in_cmd(data, data->ast, &data->envp);
	else
		execute_fork_cmd(data, data->envp, data->ast);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argc;
	(void)argv;
	init_data(&data, env);
	ft_putstr_fd("\033]2;667\007", STDIN_FILENO);
	while (1)
	{
		data.line = readline("minishell$ ");
		if (data.line == NULL)
		{
			ft_putendl_fd(CYAN"Exit"RESET, 1);
			break ;
		}
		if (check_eof(data.line) == 1)
			continue ;
		process_line(&data);
		free_ast(&data.ast);
		unlink(".tmp");
		free(data.line);
		data.ast = NULL;
	}
	rl_clear_history();
	free_split(data.envp);
	return (0);
}
