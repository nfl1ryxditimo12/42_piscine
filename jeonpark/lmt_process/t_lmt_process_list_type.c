/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lmt_process_list_type.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeonpark <jeonpark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 12:04:03 by jeonpark          #+#    #+#             */
/*   Updated: 2021/07/04 21:51:33 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_lmt_process_list.h"
#include "t_lmt_process.h"
#include "lmt_util.h"

static t_lmt_process_list	*lmt_process_list_alloc(void)
{
	return (lmt_alloc(sizeof(t_lmt_process_list)));
}

static void	lmt_process_list_init(t_lmt_process_list *list)
{
	list->p_dummy = lmt_process_new();
	list->p_bottom = list->p_dummy;
}

t_lmt_process_list	*lmt_process_list_new(void)
{
	t_lmt_process_list	*list;

	list = lmt_process_list_alloc();
	lmt_process_list_init(list);
	return (list);
}

void	lmt_process_list_free(t_lmt_process *list)
{
	t_lmt_process	*iterator;
	t_lmt_process	*next;

	iterator = list;
	while (iterator != NULL)
	{
		next = iterator->next;
		lmt_process_free(iterator);
		iterator = next;
	}
	free(list);
}
