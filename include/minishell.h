/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 08:44:10 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/12 11:53:52 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>

/*
    Commandes et arguments: ls, echo, file1.txt, etc.
    Opérateurs de redirection: <, >, <<, >>
    Pipes: |
    Variables d'environnement: $HOME, $VAR
    Quotes: ", '
    newline: \n
*/
typedef enum    e_tokentype
{
	TOKEN_WORD,
	TOKEN_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_PIPE,
	TOKEN_NEWLINE,
	TOKEN_EOF
}   t_tokentype;

typedef struct  s_token
{
	t_tokentype		type;
	char			*value;
	struct s_token	*next;
}   t_token;

/***built_in**/
int	mns_cd(char *path);

/******utils********/
int		mns_strcmp(char *s1, char *s2);
t_list	*get_env_lst(char **envp);

#endif