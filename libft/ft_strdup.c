/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trarijam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:51:08 by trarijam          #+#    #+#             */
/*   Updated: 2024/02/21 10:56:34 by trarijam         ###   ########.fr       */
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
