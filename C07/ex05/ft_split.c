/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 12:54:43 by seonkim           #+#    #+#             */
/*   Updated: 2021/03/03 16:56:07 by seonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

int	string_sep(char *str, char *charset)
{
	while (*charset)
		if (*str == *charset++)
			return (1);
	return (0);
}

int	string_len(char *str, char *charset)
{
	int i;

	i = 0;
	while (str[i] && !string_sep(str + i, charset))
		i++;
	return (i);
}

int	total_size(char *str, char *charset)
{
	int i;
	int w;

	w = 0;
	while (*str)
	{
		while (*str && string_sep(str, charset))
			str++;
		i = string_len(str, charset);
		str += i;
		if (i)
			w++;
	}
	return (w);
}

char	*string_cpy(char *str, int len)
{
	char *dest;

	if (!(dest = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	dest[len] = '\0';
	while (len--)
		dest[len] = str[len];
	return (dest);
}

char	**ft_split(char *str, char *charset)
{
	char **arr;
	int size;
	int i;
	int n;

	size = total_size(str, charset);
	i = -1;
	if(!(arr = malloc(sizeof(char *) * (size + 1))))
		return (NULL);
	while (++i < size)
	{
		while (*str && string_sep(str, charset))
			str++;
		n = string_len(str, charset);
		arr[i] = string_cpy(str, n);
		if (!arr[i])
			return (NULL);
		str += n;
		printf("%d\n", string_len(arr[i], charset));
	}
	arr[i] = 0;
	return (arr);
}

int main()
{
	char **ptr = ft_split("helloww--world--hihi", "--");
	int i;

	i = 0;
	while (i < 3)
		printf("%s\n", ptr[i++]);
}