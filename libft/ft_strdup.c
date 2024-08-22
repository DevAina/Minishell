/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <trarijam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:51:08 by trarijam          #+#    #+#             */
/*   Updated: 2024/08/22 15:24:09 by trarijam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*new_str;

	new_str = (char *)malloc(ft_strlen(src) + 1);
	if (new_str == NULL)
		return (NULL);
	ft_strlcpy(new_str, src, ft_strlen(src) + 1);
	return (new_str);
}
