/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 08:44:10 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/08 12:34:13 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "mns_struct_define.h"
# include "mns_lexer.h"
# include "mns_expander.h"
# include "mns_parser.h"
# include "mns_check_tokens.h"
# include "../libft/libft.h"
# include "../libft/get_next_line/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

/******env utils*****/
void		handle_built_in_cmd(t_data *data, t_ast_node *ast, char ***envp);
void		handle_ast(t_data *data);
char		**cpy_env(char **env);
char		**list_to_tab(t_list *env_lst);
void		sort_env_lst(t_list	**env_lst);
t_list		*get_env_lst(char **envp);
/***built_in**/
int			mns_cd(char **cmd, char ***env);
/***export utils***/
void		print_export(t_list *env_lst);
void		print_var_value(char *value, int i, t_list *env_lst);
int			check_var_name(char *name);
char		*get_var_name(char *var);
char		*get_var_value(char *var);
/******utils********/
int			mns_strcmp(char *s1, char *s2);
t_list		*get_env_lst(char **envp);
void		free_env_lst(t_list *env_lst);
int			lst_srch_var(t_list *lst, char *var_name);
//utils for exec
int			check_redirection_exec(t_ast_node *ast, char **envp);
char		**find_path_list(char **env);
char		*find_path(char **path_list, char *cmd);
void		free_split(char **str);
void		exec_cmd(char **envp, char **cmd, t_ast_node *ast, int *flag);
void		executor(char **envp, t_ast_node *ast, int *flag);
int			check_n_exec_built_in1(char **cmd, char **env, int *flag);
int			check_n_exec_built_in(char **cmd, char **env,
				t_ast_node *ast, int *flag);
char		**check_void_cmd(char **cmd, char **envp, t_ast_node *ast);
void		execute(t_ast_node *ast, char **envp, char **cmd, int *flag);
//built in cmd
int			ft_pwd(char **args, char **env);
int			ft_echo(char **args);
int			ft_env(char **env, char **cmd);
int			ft_export(char **cmd, char ***env);
char		**cpy_env(char **env);
t_list		*get_env_lst(char **envp);
char		**list_to_tab(t_list *env_lst);
int			ft_unset(char **cmd, char ***env);
int			ft_exit(char **cmd, t_ast_node *ast, char **envp, int flag);
/********/
char		*expand_line(char *line, char **env, int exit_status);
void		handler_sigint(int sig);
int			check_eof(char *str);
void		uptdate_history(t_data *data);
int			check_built_in(t_ast_node *ast);
void		wait_child_process(t_data *data);
void		execute_fork_cmd(t_data *data, char **envp, t_ast_node *ast);
void		init_data(t_data *data, char **env);
void		remove_one(t_list **env_lst, char *to_remove);	
char		*get_var_value(char *var);

#endif
