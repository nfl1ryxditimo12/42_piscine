/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_combn.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 17:54:44 by seonkim           #+#    #+#             */
/*   Updated: 2021/03/14 15:41:34 by seonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	g_len;
char	g_nbr[10];

void	rec(int prev, int n)
{
	int i;

	if (n == glen)
	{
		
	}
}

void	print_last(int i)
{
	
}

void	ft_print_combn(int n)
{
	int i;

	i = 0;
	while (i < 10 - n)
	{
		g_nbr[0] = '0' + i;
		rev(i++, 1);
	}
	print_last(i);
}
