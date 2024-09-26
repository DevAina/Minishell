/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/26 10:39:51 by trarijam         ###   ########.fr       */
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

void	process_line(t_data *data)
{
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
		return ;
	}
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
