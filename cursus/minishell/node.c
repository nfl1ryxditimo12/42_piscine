/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 16:12:54 by seonkim           #+#    #+#             */
/*   Updated: 2021/10/24 17:56:52 by seonkim          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*node_init(void)
{
	t_token	*node;
	int		i;

	i = -1;
	node = lmt_alloc(sizeof(t_token));
	if (!node)
		print_err(MEM_ERR, 0);
	while (++i < 101)
	{
		node->token[i] = NULL;
		if (i < 100)
			node->space[i] = 0;
	}
	node->cmd_dir = 0;
	node->size = 0;
	node->pre = NULL;
	node->next = NULL;
	return (node);
}

void	node_push(t_handler *hand)
{
	t_token	*new;
	t_token	*old;

	new = node_init();
	if (!hand->token_size)
	{
		hand->line = new;
		hand->top = hand->line;
	}
	else
	{
		old = hand->line;
		old->next = new;
		new->pre = old;
		hand->line = new;
	}
	hand->line->type = -1;
	hand->token_size++;
}
