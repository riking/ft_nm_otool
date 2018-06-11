/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 15:49:55 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 14:32:00 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "macho_iter.h"
#include "load_commands.h"
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

int		main(int argc, char **argv)
{
    t_mfile				memfile;
    t_iter				it;
    t_callbacks			cbs;
	t_debug_cmd_data	data;
    int					errnum;

    ft_set_progname(argv[0]);
    if (argc != 2)
		exit(2);
    errnum = ft_mh_mfile_open(&memfile, argv[1]);
    if (0 != errnum)
		fatal_error(errnum, argv[1]);
	data.file = &memfile;
    cbs = (t_callbacks){&data, 0, &debug_loadcmd, &debug_segment,
		&debug_section, 0};
    errnum = ft_mh_iter_start(&it, &memfile, &cbs);
    if (0 != errnum)
		fatal_error(errnum, argv[1]);
    while (0 == errnum)
		errnum = ft_mh_iter_next(&it);
    if (errnum != -1)
		fatal_error(errnum, argv[1]);
    return (0);
}
