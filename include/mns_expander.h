/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mns_expander.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:03:22 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/25 16:48:15 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MNS_EXPANDER_H
# define MNS_EXPANDER_H
# include "mns_lexer.h"

int		get_var_name_length(char *str);
char	*get_env_value(char *var, char **env);
char	*str_append(char *str, char *append);
char	*char_append(char *str, char c);
char	*expand_env_var(char *var_name, char **env);
void	handle_quotes(char c, int *in_single_quotes, int *in_double_quotes);
char	*expand_special_char(char *str, char **env, int *i, int exit_status);
int		should_expand(char *str, int i, int in_single_quotes);
char	*expand_token(char *str, char **env, int exit_status);
void	expand_tokens(t_token *tokens, char **env, int exit_status);

#endif