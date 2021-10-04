/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lmt_process_manager_method.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeonpark <jeonpark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 12:18:19 by jeonpark          #+#    #+#             */
/*   Updated: 2021/07/16 16:41:30 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "t_lmt_process_manager.h"

//	stddef: NULL

//	token_sublist 의 범위 안에 있는 token 들로
//	process 를 구성하고 실행한다
//
//	- 쓰임:
//	lmt_process_manager_execute() 안에서,
//	또는 type 이 subshell 인 lmt_process 를 execute 할 때 호출한다
//
//	- 반환값:
//	프로세스가 실행된 후 반환된 값
int	lmt_process_manager_execute_token_sublist(t_handler *p_handler, t_lmt_token_sublist *token_sublist)
{
	t_lmt_process_list	*process_list;
	int	exit_code;

	process_list = lmt_process_list_new();
	lmt_process_list_set_by_token_sublist(process_list, token_sublist);
	exit_code = lmt_process_list_execute(process_list, p_handler);
	lmt_process_list_free(process_list);
	return (exit_code);
}

//	파싱이 끝난 p_handler 를 읽어 자식 프로세스 생성, 리다이렉션 적용,
//	'|', '&&', '||' '()' 에 따라 적절히 자식 프로세스를 생성하고
//	command 를 실행하는 함수를 호출한다
//
//	- 쓰임:
//	파싱이 끝난 후 이 함수를 호출하면 된다
//
//	- 반환값:
//	프로세스가 실행된 후 반환된 값
void	lmt_process_manager_execute(t_handler *p_handler)
{
	t_lmt_token_sublist	*token_sublist;
	int	exit_code;

	token_sublist = lmt_token_sublist_new(p_handler->top, NULL);
	exit_code = lmt_process_manager_execute_token_sublist(p_handler, token_sublist);
	lmt_token_sublist_free(token_sublist);
	//	p_handler 에 exit_code 관련된 변수값 설정하기
}
