/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 13:39:40 by bhamidi           #+#    #+#             */
/*   Updated: 2018/02/11 15:47:15 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dst, const char *src)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(dst);
	while (src[i] != '\0')
	{
		dst[len] = src[i];
		len++;
		i++;
	}
	dst[len] = '\0';
	return (dst);
}
