/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho-segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 14:06:45 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 14:17:50 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "macho_iter.h"

void			ft_mh_iter_segment32(t_iter *iter, const void *cmd)
{
	const struct segment_command	*c;
	const struct section			*sect;
	const struct section			*enditer;

	c = cmd;
	iter->cb->seg(iter->cb->data, false, cmd, swap32i(c->cmdsize));
	enditer = (void*)((char*)cmd + swap32i(c->cmdsize));
	sect = (void*)(c + 1);
}

void			ft_mh_iter_segment64(t_iter *iter, const void *cmd)
{
	const struct segment_command_64	*c;
	const struct section_64			*sect;
	const struct section_64			*enditer;
	int								i;

	c = cmd;
	if (iter->cb->seg)
		iter->cb->seg(iter->cb->data, true, cmd, swap32i(c->cmdsize));
	enditer = (void*)((char*)cmd + swap32i(c->cmdsize));
	sect = (void*)(c + 1);
	i = 0;
	while (sect < enditer && i < c->nsects)
	{
		if (iter->cb->sec)
			iter->cb->sec(iter->cb->data, true, sect);
		sect++;
		i++;
	}
}
