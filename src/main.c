/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 15:49:55 by kyork             #+#    #+#             */
/*   Updated: 2018/06/07 16:21:02 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "macho_iter.h"
#include <stdlib.h>
#include <stdio.h>

#include <ft_printf.h>
#include <libft.h>

static const char	*g_custom_errs[] = {
    [ERR_NOT_OBJ_FILE & 0xFF] = "not a Mach-O object file",
    [ERR_BAD_ALIGN & 0xFF] = "invalid entry alignment",
    [ERR_TRUNC & 0xFF] = "unexpected end of file",
};

static const char	*g_op_names[] = {
    [ERR_STDLIB_OPEN >> 24] = "open",
    [ERR_STDLIB_CLOSE >> 24] = "close",
    [ERR_STDLIB_MMAP >> 24] = "mmap",
    [ERR_STDLIB_MUNMAP >> 24] = "munmap",
    [ERR_STDLIB_WRITE >> 24] = "write",
    [ERR_STDLIB_FSTAT >> 24] = "fstat",
};

static void		fatal_error(int errnum, char *file)
{
    const char	*msg;

    if (errnum == -1)
    {
	ft_dprintf(2, "%s: %s: error\n", ft_progname(), file);
    }
    else if ((errnum & ERRNUM_STDLIB_MASK) != 0)
    {
	ft_dprintf(2, "%s: %s: %s: %s\n", ft_progname(), file,
		g_op_names[errnum >> 24],
		sys_errlist[errnum & ~ERRNUM_STDLIB_MASK]);
    }
    else if ((errnum & ERRNUM_CUSTOM) != 0)
    {
	msg = g_custom_errs[errnum & ~ERRNUM_CUSTOM];
	ft_dprintf(2, "%s: %s: %s\n", ft_progname(), file, msg);
    }
    exit(1);
}

static void	cb_cmd(void *data, uint32_t type,
			const void *cmd_body, size_t cmd_size)
{
    (void)data;
    if (type & LC_REQ_DYLD)
	type = type & ~LC_REQ_DYLD;
    ft_printf("Section type %3x: %zd bytes\n", type, cmd_size);
    (void)cmd_body;
}

#define EMPTY_INIT {0,}

int		main(int argc, char **argv)
{
    t_mfile	memfile;
    t_iter	it;
    t_callbacks	cbs;
    int		errnum;

    ft_set_progname(argv[0]);
    if (argc != 2)
	exit(2);
    errnum = ft_mh_mfile_open(&memfile, argv[1]);
    if (0 != errnum)
	fatal_error(errnum, argv[1]);
    cbs = (t_callbacks){0, 0, &cb_cmd, 0};
    cbs.cmd = &cb_cmd;
    errnum = ft_mh_iter_start(&it, &memfile, &cbs);
    if (0 != errnum)
	fatal_error(errnum, argv[1]);
    while (0 == errnum)
	errnum = ft_mh_iter_next(&it);
    if (errnum != -1)
	fatal_error(errnum, argv[1]);
    return (0);
}
