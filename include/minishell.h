/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 08:44:10 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/12 11:58:54 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../libft/get_next_line/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define RESET "\033[0m"
# define REDIR_COUNT 0
# define ARG_COUNT 1
# define ASSIGNEMENT_COUNT 2

typedef struct s_ast_node		t_ast_node;
typedef enum e_tokentype		t_tokentype;
typedef struct s_token			t_token;
typedef struct s_redirection	t_redirection;
typedef enum e_ast_node_type	t_ast_node_type;
typedef struct s_ast_node		t_ast_node;
typedef enum e_type_redirection	t_type_redirection;

/****structure for token**/
/*
    Commandes et arguments: ls, echo, file1.txt, etc.
    Opérateurs de redirection: <, >, <<, >>
    Pipes: |
    Variables d'environnement: $HOME, $VAR
    Quotes: ", '
*/
enum e_tokentype
{
    TOKEN_WORD,
    TOKEN_ASSIGNEMENT,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_IN_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_NEWLINE,
    TOKEN_EOF
};

struct	s_token
{
    t_tokentype		type;
    char			*value;
    int				fd;
    struct s_token	*next;
};

/*structure for AST*/
enum    e_ast_node_type
{
    AST_COMMAND = 0,
    AST_PIPE = 1
};

enum	e_type_redirection
{
	REDIRECTION_IN = 0,
	REDIRECTION_OUT = 1,
	REDIRECTION_APPEND = 2,
	REDIRECTION_HEREDOC = 3,
};


struct s_redirection
{
	t_type_redirection		type_redirection;
	int						fd;
	char					*target;
};


struct s_ast_node
{
    t_ast_node_type     type;
    char                **args;
    char                **assignement;
	t_redirection		*redirection;
    struct s_ast_node   *left;
    struct s_ast_node   *right;
};

/***built_in**/
int		mns_cd(char **cmd, char ***env);

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
void	exec_cmd(char **envp, char **cmd, t_ast_node *ast);
void	executor(char **envp, t_ast_node *ast);
/*****expander*******/
int		get_var_name_length(char *str);
char	*get_env_value(char *var, char **env);
char	*str_append(char *str, char *append);
char	*char_append(char *str, char c);
char	*expand_env_var(char *var_name, char **env);
char	*expand_special_char(char *str, char **env, int *i, int exit_status);
char	*expand_token(char *str, char **env, int exit_status);
void	expand_tokens(t_token *tokens, char **env, int exit_status);

/********Analyze tokens********/
int		analyze_tokens(t_token *tokens, char **env, int exit_status);

//built in cmd
int		ft_pwd(char **args, char **env);
int		ft_echo(char **args);
int		ft_env(char **env);
int		ft_export(char **cmd, char **assignement, char ***env);
char	**cpy_env(char **env);
t_list	*get_env_lst(char **envp);
char	**list_to_tab(t_list *env_lst);
int		ft_unset(char **cmd, char ***env);
int		ft_exit(char **cmd);
/********/
void    handler_sigint(int sig);
void	remove_one(t_list **env_lst, char *to_remove);

#endif
