/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/12 13:02:37 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int main(int argc, char **argv, char **env)
{
	char				*line;
	//int					status;
	char				**envp;
	t_token				*token;
	t_ast_node			*ast;
	//pid_t				pid;
	int					hist_fd;
	struct sigaction	sa;
	struct sigaction	sa_sigquit;
	struct sigaction	sa_ignore;

	(void)argc;
	(void)argv;
	sigemptyset(&sa_sigquit.sa_mask);
	sigemptyset(&sa.sa_mask);
	sigemptyset(&sa_ignore.sa_mask);
	sa.sa_handler = handler_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa_ignore.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sa_ignore.sa_flags = 0;
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
	envp = cpy_env(env);
	hist_fd = open(".history_file", O_RDWR
				| O_CREAT | O_APPEND, 0777);
	get_history(hist_fd);
	while (1)
	{
		line = readline(YELLOW"minishell$ "RESET);
		if (line == NULL)
		{
			ft_putendl_fd(CYAN"Exit"RESET, 1);
			break;
		}
		if (*line == '\0')
		{
			free(line);
			continue ; 
		}
		if (line && *line)
		{
			add_history(line);
			ft_putendl_fd(line, hist_fd);
		}
		token = lexer(line);
        if (analyze_tokens(token, envp, g_exit_status) == 0)
        {
			unlink(".tmp");
            free_token(token);
            free(line);
            continue;
        }
        expand_tokens(token, envp, g_exit_status);
		ast = parse(token);
		/*free_token(token);
		if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "cd", 3) == 0)
			g_exit_status = mns_cd(ast->args, &envp);
		else if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "export", 7) == 0)
			g_exit_status = ft_export(ast->args, ast->assignement, &envp);
		else if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "unset", 6) == 0)
			g_exit_status = ft_unset(ast->args, &envp);
		else if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "exit", 5) == 0)
			g_exit_status = ft_exit(ast->args);
		else
		{
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
				sigaction(SIGINT, &sa_ignore, NULL);
				wait(&status);
				sigaction(SIGINT, &sa, NULL);
				if (WIFEXITED(status))
					g_exit_status = WEXITSTATUS(status);
				if (WIFSIGNALED(status))
				{
					write(1, "\n", 1);
					g_exit_status = 128 + WTERMSIG(status);
				}
			}
		}*/
		print_ast(ast, 0);
		free_token(token);
		free_ast(&ast);
		unlink(".tmp");
		free(line);
	}
	rl_clear_history();
	free_split(envp);
	return (0);
}
