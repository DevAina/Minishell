/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/04 11:41:54 by trarijam         ###   ########.fr       */
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
            if (node->input)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("input file: ");
                for (i = 0; node->input[i].target; i++)
                    printf("%s fd: %d", node->input[i].target, node->input[i].fd);
                printf("\n");
            }
            if (node->assignement)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("assignement: ");
                for (i = 0; node->assignement[i]; i++)
                    printf("%s ", node->assignement[i]);
                printf("\n");
            }
            if (node->output)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("Output file: ");
                for (i = 0; node->output[i].target; i++)
                    printf("%s fd: %d", node->output[i].target, node->output[i].fd);
                printf("\n");
            }
            if (node->heredoc)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("hredeoc defilimiter: ");
                for (i = 0; node->heredoc[i].target; i++)
                    printf("%s fd: %d", node->heredoc[i].target, node->heredoc[i].fd);
                printf("\n");
            }
            if (node->output_append)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("output append : ");
                for (i = 0; node->output_append[i].target; i++)
                    printf("%s fd: %d", node->output_append[i].target, node->output_append[i].fd);
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
	int					status;
	char				**envp;
	t_token				*token;
	t_ast_node			*ast;
	pid_t				pid;
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
			break;
		if (*line == '\0')
		{
			free(line);
			continue ; 
		}
		token = lexer(line);
        if (analyze_tokens(token) == 0)
        {
            free_token(token);
            free(line);
            continue;
        }
        expand_tokens(token, envp, g_exit_status);
		ast = parse(token);
		free_token(token);
		if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "cd", 3) == 0)
			mns_cd(ast->args, &envp);
		else if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "export", 7) == 0)
			ft_export(ast->args, ast->assignement, &envp);
		else if (ast->type == AST_COMMAND && ft_strncmp(ast->args[0], "unset", 6) == 0)
			ft_unset(ast->args, &envp);
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
				exit(0);
			}
		}
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
		free_ast(&ast);
		if (line && *line)
		{
			add_history(line);
			ft_putendl_fd(line, hist_fd);
		}
		free(line);
	}
	rl_clear_history();
	free_split(envp);
	return (0);
}
