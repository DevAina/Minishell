/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:30:35 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/21 15:27:48 by trarijam         ###   ########.fr       */
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
            if (node->input_file)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("  Input: ");
                for (i = 0; node->input_file[i]; i++)
                    printf("%s ", node->input_file[i]);
                printf("\n");
            }
            if (node->output_file)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("Output file: ");
                for (i = 0; node->output_file[i]; i++)
                    printf("%s ", node->output_file[i]);
                printf("\n");
            }
            if (node->heredoc_delimiter)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("hredeoc defilimiter: ");
                for (i = 0; node->heredoc_delimiter[i]; i++)
                    printf("%s ", node->heredoc_delimiter[i]);
                printf("\n");
            }
            if (node->output_append)
            {
                for (i = 0; i < depth; i++)
                    printf("  ");
                printf("output append : ");
                for (i = 0; node->output_append[i]; i++)
                    printf("%s ", node->output_append[i]);
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

int main(int argc, char **argv, char **envp)
{
    //(void)envp;
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
        expand_tokens(token, envp);
		ast = parse(token);
		//free_token(token);
//		exec_cmd(envp, ast->args, -1, NULL);
		/*if (fork() == 0)
		{
			executor(envp, ast);
			free_ast(&ast);
			exit(0);
		}*/
		print_ast(ast, 0);
        free_token(token);
		//wait(NULL);
		free_ast(&ast);
		free(line);
	}
	return (0);
}
