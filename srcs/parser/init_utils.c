/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:36:37 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/26 10:35:51 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*init_node(t_ast_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->assignement = NULL;
	node->redirection = NULL;
	return (node);
}

void	init_args_input_output_file(t_ast_node **cmd, int *count)
{
	if (count[ARG_COUNT] != 0)
		(*cmd)->args = (char **)ft_calloc(count[ARG_COUNT] + 1,
				sizeof(char *));
	if (count[REDIR_COUNT] != 0)
		(*cmd)->redirection = (t_redirection *)ft_calloc(count[REDIR_COUNT] + 1,
				sizeof(t_redirection));
	if (count[ASSIGNEMENT_COUNT] != 0)
		(*cmd)->assignement = (char **)ft_calloc(count[ASSIGNEMENT_COUNT] + 1,
				sizeof(char *));
}
