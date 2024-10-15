/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:05:46 by trarijam          #+#    #+#             */
/*   Updated: 2024/10/15 13:55:27 by traveloa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_state(int state)
{
	if (state == 130)
	{
		unlink(".tmp");
		return (1);
	}
	else if (state == 166)
	{
		unlink(".tmp");
		return (1);
	}
	return (0);
}

int	process_input(t_data *data, int exit_status)
{
	int	state;

	state = heredoc(data, exit_status);
	if (handle_state(state))
		return (1);
	if (check_eof(data->line) == 1)
		return (1);
	process_line(data);
	return (0);
}

void	cleanup_data(t_data *data)
{
	free_ast(&data->ast);
	unlink(".tmp");
	free(data->line);
	data->ast = NULL;
}
