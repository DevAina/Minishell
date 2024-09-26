/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: traveloa <traveloa@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 07:17:56 by traveloa          #+#    #+#             */
/*   Updated: 2024/09/26 07:22:36 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_n_exec_built_in1(char **cmd, char **env, t_ast_node *ast)
{
	if (ft_strncmp(cmd[0], "export", 7) == 0)
	{
		ft_export(cmd, ast->assignement, &env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
	{
		mns_cd(cmd, &env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
	{
		ft_unset(cmd, &env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
	{
		ft_env(env, cmd);
		return (1);
	}
	return (0);
}

int	check_n_exec_built_in(char **cmd, char **env, t_ast_node *ast)
{
	if (ft_strncmp(cmd[0], "pwd", 4) == 0)
	{
		ft_pwd(cmd, env);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "echo", 5) == 0)
	{
		ft_echo(cmd);
		return (1);
	}
	else if (ft_strncmp(cmd[0], "exit", 5) == 0)
	{
		ft_exit(cmd, ast, env);
		return (1);
	}
	return (0);
}

char	**check_void_cmd(char **cmd, char **envp, t_ast_node *ast)
{
	char	**tmp;
	int		i;

	if (cmd == NULL)
		return (NULL);
	i = 0;
	tmp = cmd;
	while (ft_strlen(tmp[i]) == 0 && tmp[i] != NULL)
		i++;
	if (tmp[i] == NULL)
	{
		free_split(envp);
		free_ast(&ast);
		exit(EXIT_SUCCESS);
	}
	tmp += i;
	return (tmp);
}

void	execute(t_ast_node *ast, char **envp, char **cmd, int *flag)
{
	char	**path_list;
	char	*path;

	(void)ast;
	path_list = find_path_list(envp);
	path = find_path(path_list, cmd[0]);
	free_split(path_list);
	if (path == NULL)
	{
		perror(cmd[0]);
		*flag = 127;
		return ;
	}
	if (execve(path, cmd, envp) == -1)
	{
		perror(cmd[0]);
		*flag = 126;
		return ;
	}
}
