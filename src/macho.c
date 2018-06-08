/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 12:48:35 by kyork             #+#    #+#             */
/*   Updated: 2018/06/07 16:15:55 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libkern/OSByteOrder.h>

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "macho_iter.h"

static int		check_magic(t_iter *iter, t_mfile *f)
{
	const struct mach_header_64	*hdr;
	uint32_t					magic;

	hdr = (struct mach_header_64*)f->ptr;
	if (hdr->magic == MH_CIGAM_64 || hdr->magic == MH_CIGAM)
		g_swap_wrong_endian = true;
	magic = swap32i(hdr->magic);
	if (magic == MH_MAGIC_64)
	{
		f->is_64 = true;
		iter->cmd_count = swap32i(hdr->ncmds);
		iter->cur_command = (void*)(hdr + 1);
		return (1);
	}
	if (magic == MH_MAGIC)
	{
		iter->cmd_count = swap32i(hdr->ncmds);
		iter->cur_command = (void*)(((const struct mach_header*)hdr) + 1);
		return (1);
	}
	return (0);
}

int				ft_mh_iter_start(t_iter *iter, t_mfile *file,
		const t_callbacks *cbs)
{
	iter->file = file;
	iter->cb = cbs;
	iter->cmd_cur = 0;
	iter->cmd_count = 0;
	if (!check_magic(iter, file))
		return (ERR_NOT_OBJ_FILE);
	if (cbs->hdr)
		cbs->hdr(cbs->data, (const struct mach_header_64*)file->ptr);
	return (0);
}

int				ft_mh_iter_next(t_iter *iter)
{
	const struct load_command	*cmd;

	if (iter->cmd_cur >= iter->cmd_count)
		return (-1);
	if ((((char*)iter->cur_command) - iter->file->ptr) > (ptrdiff_t)iter->file->size)
		return (ERR_TRUNC);
	cmd = iter->cur_command;
	iter->cur_command = (void*)(((char*)cmd) + swap32i(cmd->cmdsize));
	iter->cmd_cur++;
	iter->cb->cmd(iter->cb->data, swap32i(cmd->cmd),
			cmd, swap32i(cmd->cmdsize));
	return (0);
}

int				ft_mh_mfile_open(t_mfile *out, char *path)
{
	struct stat		st;

	out->fd = open(path, O_RDONLY, 0644);
	if (out->fd < 0)
		return (ERR_STDLIB_OPEN | (errno));
	if (0 != fstat(out->fd, &st))
		return (ERR_STDLIB_FSTAT | (errno));
	out->size = st.st_size;
	out->ptr = mmap(NULL, out->size, PROT_READ, MAP_FILE | MAP_PRIVATE,
			out->fd, 0);
	if (out->ptr == NULL || out->ptr == MAP_FAILED)
		return (ERR_STDLIB_MMAP | (errno));
	out->is_64 = false;
	return (0);
}
