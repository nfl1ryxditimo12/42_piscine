/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lmt_string.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:02:52 by jeonpark          #+#    #+#             */
/*   Updated: 2021/10/26 21:35:28 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_LMT_STRING_H
# define T_LMT_STRING_H

# include <stddef.h>

//	lmt_alloc(), free(), lmt_unsafe_strchr()

typedef struct s_lmt_string
{
	char	*value;
	size_t	count;
}	t_lmt_string;

void			lmt_string_init(t_lmt_string *string, const char *value);
t_lmt_string	*lmt_string_new(const char *value);
void			lmt_string_free(t_lmt_string *p_lmt_string);

t_lmt_string	*lmt_string_split(t_lmt_string *string, char *separator);
t_lmt_string	*lmt_string_split_first(t_lmt_string *string, size_t count);
t_lmt_string	*lmt_string_split_last(t_lmt_string *string, size_t count);
void			lmt_string_append_string(
					t_lmt_string *string, const char *word);
void			lmt_string_append_lmt_string(
					t_lmt_string *string, t_lmt_string *word);

#endif
