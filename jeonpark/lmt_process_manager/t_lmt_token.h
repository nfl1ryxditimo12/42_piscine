/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lmt_token.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeonpark <jeonpark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 16:54:16 by jeonpark          #+#    #+#             */
/*   Updated: 2021/07/08 19:21:45 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_LMT_TOKEN_H
# define T_LMT_TOKEN_H

# include "lmt_process_manager_typedef.h"

typedef struct s_lmt_token
{
	int	type;
	t_lmt_string_array	*array;
	struct s_lmt_token	*next;
}	t_lmt_token;

//	type function
t_lmt_token	*lmt_token_new(int type);
void	lmt_token_free(t_lmt_token *p_token);
//
//	method function
void	lmt_token_append_token(t_lmt_token *lhs, t_lmt_token *rhs);

#endif
