/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 08:44:10 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/19 08:03:46 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define RESET "\033[0m"

/****structure for token**/
/*
    Commandes et arguments: ls, echo, file1.txt, etc.
    Opérateurs de redirection: <, >, <<, >>
    Pipes: |
    Variables d'environnement: $HOME, $VAR
    Quotes: ", '
*/
typedef enum e_tokentype
{
    TOKEN_ENV_VAR,
    TOKEN_WORD,   
    TOKEN_PIPE,    
    TOKEN_REDIR_IN, 
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,  
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMICOLON,
    TOKEN_NEWLINE,
    TOKEN_EOF      
}   t_tokentype;

typedef struct	s_token
{
    t_tokentype       type;
    char            *value;
    struct s_token    *next;
} t_token;

/*structure for AST*/
typedef enum    e_ast_node_type
{
    AST_COMMAND = 0,
    AST_PIPE = 1,
    AST_REDIR_IN = 2,
    AST_REDIR_OUT = 3,
    AST_REDIR_APPEND = 4,
    AST_REDIR_HERDOC = 5
} t_ast_node_type;

typedef struct s_ast_node
{
    t_ast_node_type     type;
    char                **args;
    char                *input_file;
    char                *output_file;
    //(0 redirection simple > (le fichier sera ecraser), 1 redirection double >> (La sortie sera ajoutée à la fin du fichier))
    int                 append_output;
    char                *heredoc_delimiter;
    struct s_ast_node   *left;
    struct s_ast_node   *right;
} t_ast_node;

/***built_in**/
int		mns_cd(char *path);

/******utils********/
int		mns_strcmp(char *s1, char *s2);
t_list	*get_env_lst(char **envp);
void	free_env_lst(t_list *env_lst);

/**********AST*******/

t_ast_node	*parse(t_token *tokens);
void        free_token(t_token *token);
t_token     *lexer(char *input);
void        free_ast(t_ast_node **node);

//utils for exec
char	**find_path_list(char **env);
char	*find_path(char **path_list, char *cmd);
void	free_split(char **str);
void	exec_cmd(char **envp, char **cmd);
void	executor(char **envp, t_ast_node *ast);
#endif
