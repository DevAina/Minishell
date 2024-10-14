/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/14 16:02:51 by trarijam         ###   ########.fr       */
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

void	handle_built_in_cmd(t_ast_node *ast, char ***envp)
{
	int		fd_in;
	int		fd_out;

	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (ast->redirection)
		check_redirection_exec(ast, *envp);
	if (ft_strncmp(ast->args[0], "cd", 3) == 0)
		g_exit_status = mns_cd(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "export", 7) == 0)
		g_exit_status = ft_export(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "unset", 6) == 0)
		g_exit_status = ft_unset(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "exit", 5) == 0)
		g_exit_status = ft_exit(ast->args, ast, *envp, 1);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
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

void	process_line(t_data *data)
{
	t_token		*expanded;

	uptdate_history(data);
	data->token = lexer(data->line);
	if (data->token == NULL)
	{
		data->ast = NULL;
		return ;
	}
	if (analyze_tokens(data->token, data->envp, g_exit_status) == 0)
	{
		unlink(".tmp");
		free_token(data->token);
		g_exit_status = 2;
		return ;
	}
	expanded = expand_tokens(data->token, data->envp, g_exit_status);
	free_token(data->token);
	data->ast = parse(expanded);
	free_token(expanded);
	if (data->ast->type == AST_COMMAND && data->ast->args != NULL
		&& data->ast->args[0] == NULL)
		return ;
	handle_ast(data);
}

int	handle_state(int state)
{
	if (state == 130)
	{
		unlink(".tmp");
		g_exit_status = 130;
		return (1);
	}
	else if (state == 166)
	{
		unlink(".tmp");
		return (1);
	}
	return (0);
}

int	process_input(t_data *data)
{
	int state;

	state = heredoc(data, g_exit_status);
	if (handle_state(state))
		return (1);
	if (check_eof(data->line) == 1)
		return (1);
	process_line(data);
	return (0);
}

void	cleanup_data(t_data *data)
{
	free_ast(&data->ast);
	unlink(".tmp");
	free(data->line);
	data->ast = NULL;
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
		if (process_input(&data) == 1)
			continue;
		cleanup_data(&data);
	}
	rl_clear_history();
	free_split(data.envp);
	return (0);
}
