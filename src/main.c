/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:09:27 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/20 14:09:29 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

/*int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)envp;
	(void)argv;
	char	**split;
	int		i;
	char	dir[1024];
	char	*line;

	while (1)
	{
		getcwd(dir, sizeof(dir));
		ft_putstr_fd(getenv("USER"), 1);
		ft_putstr_fd(dir, 1);
		line = readline("$ ");
		if (line == NULL)
			break ;
		if (*line == '\0')
		{
			free(line);
			break ;
		}
		split = ft_split(line, ' ');
		if (ft_strncmp(split[0], "cd", 2) == 0)
			mns_cd(split[1]);
		i = 0;
		while (split[i] != NULL)
		{
			free(split[i]);
			i++;
		}
		free(split);
		free(line);
	}
	return (0);
}*/

void print_ast_node(t_ast_node *node, int depth)
{
	int	i;

    if (!node) return;

    // Indentation pour la structure arborescente
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Affichage selon le type de nœud
    switch (node->type) {
        case AST_COMMAND:
            printf("COMMAND: ");
            if (node->args) {
                for (i = 0; node->args[i]; i++) {
                    printf("%s ", node->args[i]);
                }
            }
            printf("\n");
            if (node->input_file)
                printf("  Input: %s\n", node->input_file);
            if (node->output_file)
                printf("  Output: %s (Append: %s)\n", node->output_file, node->append_output ? "Yes" : "No");
            if (node->heredoc_delimiter)
                printf("  Heredoc: %s\n", node->heredoc_delimiter);
            break;
        case AST_PIPE:
            printf("PIPE\n");
            break;
        case AST_REDIR_IN:
            printf("REDIRECTION IN\n");
			if (node->args) 
			{
				for (int i = 0; i < depth + 13; i++)
        			printf("  ");
                for (i = 0; node->args[i]; i++) {
                    printf("%s ", node->args[i]);
                }
            }
            printf("\n");
            if (node->input_file)
                printf("  Input: %s\n", node->input_file);
            if (node->output_file)
                printf("  Output: %s (Append: %s)\n", node->output_file, node->append_output ? "Yes" : "No");
            if (node->heredoc_delimiter)
                printf("  Heredoc: %s\n", node->heredoc_delimiter);
            break;
		case AST_REDIR_OUT:
            printf("REDIRECTION OUT\n");
			if (node->args) 
			{
				for (int i = 0; i < depth + 13; i++)
        			printf("  ");
                for (i = 0; node->args[i]; i++) {
                    printf("%s ", node->args[i]);
                }
            }
            printf("\n");
            if (node->input_file)
                printf("  Input: %s\n", node->input_file);
            if (node->output_file)
                printf("  Output: %s (Append: %s)\n", node->output_file, node->append_output ? "Yes" : "No");
            if (node->heredoc_delimiter)
                printf("  Heredoc: %s\n", node->heredoc_delimiter);
            break;
		case AST_REDIR_APPEND:
            printf("REDIRECTION APPEND\n");
			if (node->args) 
			{
				for (int i = 0; i < depth + 13; i++)
        			printf("  ");
                for (i = 0; node->args[i]; i++) {
                    printf("%s ", node->args[i]);
                }
            }
            printf("\n");
            if (node->input_file)
                printf("  Input: %s\n", node->input_file);
            if (node->output_file)
                printf("  Output: %s (Append: %s)\n", node->output_file, node->append_output ? "Yes" : "No");
            if (node->heredoc_delimiter)
                printf("  Heredoc: %s\n", node->heredoc_delimiter);
            break;
		case AST_REDIR_HERDOC:
            printf("REDIRECTION HEREDOC\n");
			if (node->args) 
			{
				for (int i = 0; i < depth + 13; i++)
        			printf("  ");
                for (i = 0; node->args[i]; i++) {
                    printf("%s ", node->args[i]);
                }
            }
            printf("\n");
            if (node->input_file)
                printf("  Input: %s\n", node->input_file);
            if (node->output_file)
                printf("  Output: %s (Append: %s)\n", node->output_file, node->append_output ? "Yes" : "No");
            if (node->heredoc_delimiter)
                printf("  Heredoc: %s\n", node->heredoc_delimiter);
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

int main(int argc, char **argv, char **envp)
{
    (void)envp;
	char	*line;
	t_token	*token;
	t_ast_node	*ast;

	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (*line == '\0')
		{
			free(line);
			break ;
		}
		token = lexer(line);
		ast = parse(token);
		free_token(token);
//		exec_cmd(envp, ast->args, -1, NULL);
		if (fork() == 0)
		{
			executor(envp, ast);
			free_ast(&ast);
			exit(0);
		}
		//print_ast(ast, 0);
		wait(NULL);
		free_ast(&ast);
		free(line);
	}
	return (0);
}
