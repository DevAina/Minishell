/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/16 18:57:04 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <complex.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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

void print_ast_node(t_ast_node *node, int depth)
{
	int	i;

    if (!node) return;

    // Indentation pour la structure arborescente
    for (i = 0; i < depth; i++)
        printf("  ");

    // Affichage selon le type de nœud
    switch (node->type) {
        case AST_COMMAND:
            printf("COMMAND: ");
            if (node->args)
            {
                for (i = 0; node->args[i]; i++)
                    printf("%s ", node->args[i]);
            }
            printf("\n");
            if (node->assignement)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("assignement: ");
                for (i = 0; node->assignement[i]; i++)
                    printf("%s ", node->assignement[i]);
                printf("\n");
            }
            if (node->redirection)
			{
				for (i = 0; node->redirection[i].target; i++)
                {
					if (node->redirection[i].type_redirection == REDIRECTION_IN)
					{
						for (int j = 0; j < depth; j++)
                    		printf("  ");
						printf("Input file: %s\n", node->redirection[i].target);
					}
					if (node->redirection[i].type_redirection == REDIRECTION_OUT)
					{
						for (int j = 0; j < depth; j++)
                    		printf("  ");
						printf("Output file: %s\n", node->redirection[i].target);
					}
					if (node->redirection[i].type_redirection == REDIRECTION_HEREDOC)
					{
						for (int j = 0; j < depth; j++)
                    		printf("  ");
						printf("Delimiter heredoc: %s\n", node->redirection[i].target);
					}
					if (node->redirection[i].type_redirection == REDIRECTION_APPEND)
					{
						for (int j = 0; j < depth; j++)
                    		printf("  ");
						printf("Append file: %s\n", node->redirection[i].target);
					}
				}
                printf("\n");
			}
            break;
        case AST_PIPE:
            printf("PIPE\n");
            break;
        default:
            printf("UNKNOWN NODE TYPE\n");
    }
}

void print_ast(t_ast_node *root, int depth)
{
    if (!root)
		return;

    print_ast_node(root, depth);

    if (root->left) {
        print_ast(root->left, depth + 1);
    }
    if (root->right) {
        print_ast(root->right, depth + 1);
    }
}

void	get_history(int fd)
{
	char	*line;

	if (fd <= 0)
		return ;
	while (1)
	{
		line = get_next_line(fd);
		if (line != NULL)
		{
			line[ft_strlen(line) - 1] = '\0';
			add_history(line);
			free(line);
		}
		else
			break ;
	}
}

void	setup_signals(t_data *data)
{
	sigemptyset(&data->sa_sigquit.sa_mask);
	sigemptyset(&data->sa.sa_mask);
	sigemptyset(&data->sa_ignore.sa_mask);
	data->sa.sa_handler = handler_sigint;
	data->sa.sa_flags = 0;
	sigaction(SIGINT, &data->sa, NULL);
	data->sa_ignore.sa_handler = SIG_IGN;
	data->sa_sigquit.sa_flags = 0;
	data->sa_ignore.sa_flags = 0;
	data->sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data->sa_sigquit, NULL);
}

void	init_data(t_data *data, char **env)
{
	data->line = NULL;
	data->envp = cpy_env(env);
	data->token = NULL;
	data->ast = NULL;
	data->fd_tmp = -1;
	data->hist_fd = open(".history_file", O_RDWR
		| O_CREAT | O_APPEND, 0777);
	if (data->hist_fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	setup_signals(data);
}

void	handle_built_in_cmd(t_data *data, t_ast_node *ast, char ***envp)
{
	if (ast->redirection)
		data->fd_tmp = check_redirection_exec(ast);
	if (ft_strncmp(ast->args[0], "cd", 3) == 0)
		g_exit_status = mns_cd(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "export", 7) == 0)
		g_exit_status = ft_export(ast->args, ast->assignement, envp);
	else if (ft_strncmp(ast->args[0], "unset", 6) == 0)
		g_exit_status = ft_unset(ast->args, envp);
	else if (ft_strncmp(ast->args[0], "exit", 5) == 0)
		g_exit_status = ft_exit(ast->args, ast, *envp);
	dup2(data->fd_tmp, STDOUT_FILENO);
	close(data->fd_tmp);
}

void execute_fork_cmd(t_data *data, char **envp, t_ast_node *ast)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		executor(envp, ast);
		free_ast(&ast);
		free_split(envp);
		exit(EXIT_FAILURE);
	}
	else
	{
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
}

void process_line(t_data *data)
{
	add_history(data->line);
	ft_putendl_fd(data->line, data->hist_fd);
	data->token = lexer(data->line);
	if (analyze_tokens(data->token, data->envp, g_exit_status) == 0)
	{
		unlink(".tmp");
		free_token(data->token);
		return ;
	}
	expand_tokens(data->token, data->envp, g_exit_status);
	data->ast = parse(data->token);
	free_token(data->token);
	if (data->ast->args[0] == NULL)
		return ;
	if (data->ast->type == AST_COMMAND
		&& (ft_strncmp(data->ast->args[0], "cd", 3) == 0
		|| ft_strncmp(data->ast->args[0], "export", 7) == 0
		|| ft_strncmp(data->ast->args[0], "unset", 6) == 0
		|| ft_strncmp(data->ast->args[0], "exit", 5) == 0))
		handle_built_in_cmd(data, data->ast, &data->envp);
	else
		execute_fork_cmd(data, data->envp, data->ast);
}

int main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argc;
	(void)argv;
	init_data(&data, env);
	while (1)
	{
		data.line = readline(YELLOW"minishell$ "RESET);
		if (data.line == NULL)
		{
			ft_putendl_fd(CYAN"Exit"RESET, 1);
			break;
		}
		if (*data.line == '\0')
		{
			free(data.line);
			continue ; 
		}
		process_line(&data);
		free_ast(&data.ast);
		unlink(".tmp");
		free(data.line);
	}
	rl_clear_history();
	free_split(data.envp);
	return (0);
}
