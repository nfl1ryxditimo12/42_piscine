/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seongsu <seongsu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 17:14:16 by seonkim           #+#    #+#             */
/*   Updated: 2021/02/22 17:22:56 by seongsu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int main()
{
    char b;
    char *ptr;
    int i;

    ptr = &b;
    i = 0;
    while (ptr[i] != '\0')
    {
        write(1, &ptr[i], 1);
        i++;
    }
}