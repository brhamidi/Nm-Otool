/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 16:54:12 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/26 17:29:57 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_natoui(const char *str, size_t n)
{
	size_t	result;
	size_t	i;
	int		is_neg;

	i = 0;
	result = 0;
	is_neg = 0;
	while (i < n && (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
			|| str[i] == '\r' || str[i] == '\f' || str[i] == '\v'))
		i++;
	if (i >= n)
		return (result);
	if (str[i] == '-')
		is_neg = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (i < n && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (is_neg == -1 ? (result * -1) : result);
}

int		ft_atoi(const char *str)
{
	int		result;
	int		i;
	int		is_neg;

	i = 0;
	result = 0;
	is_neg = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
			|| str[i] == '\r' || str[i] == '\f' || str[i] == '\v')
		i++;
	if (str[i] == '-')
		is_neg = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (is_neg == -1 ? (result * -1) : result);
}
