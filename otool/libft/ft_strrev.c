/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 17:16:53 by bhamidi           #+#    #+#             */
/*   Updated: 2018/02/11 15:49:04 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strrev(char *src)
{
	char	c;
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_strlen(src) - 1;
	while (i < j)
	{
		c = src[i];
		src[i] = src[j];
		src[j] = c;
		i++;
		j--;
	}
	return (src);
}
