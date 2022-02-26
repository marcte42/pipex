/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 12:29:34 by mterkhoy          #+#    #+#             */
/*   Updated: 2022/02/26 15:29:42 by mterkhoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest2;
	unsigned char	*src2;

	dest2 = (unsigned char *)dst;
	src2 = (unsigned char *)src;
	i = 0;
	if (!n || dst == src)
		return (dst);
	while (i < n)
	{
		dest2[i] = src2[i];
		i++;
	}
	return (dst);
}
