/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handling.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:55:11 by trarijam          #+#    #+#             */
/*   Updated: 2024/09/25 14:02:13 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_fd(char *input, int *index)
{
	int	fd;

	fd = -1;
	if (ft_isdigit(input[*index]) && (input[*index + 1] == '>'
			|| input[*index + 1] == '<'))
	{
		fd = ft_atoi(input + *index);
		*index += 1;
	}
	return (fd);
}
