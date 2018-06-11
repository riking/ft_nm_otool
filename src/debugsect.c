/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugsect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 14:41:58 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 16:06:06 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "load_commands.h"

#include <ft_printf.h>

#define ENTRY_LC(x, name) [x] = {#name, x, false}

static const t_loadcmd_name	g_section_types[] = {
	ENTRY_LC(S_REGULAR, "regular"),
};

static void					section_flags(uint32_t f, uint32_t r1,
		uint32_t r2)
{
	ft_printf("    flags:\ttype: %d other: %#x\n", f & SECTION_TYPE,
			f & SECTION_ATTRIBUTES);
	if ((f & SECTION_TYPE) >= S_NON_LAZY_SYMBOL_POINTERS && (f & SECTION_TYPE) <= S_LAZY_DYLIB_SYMBOL_POINTERS)
		ft_printf("      index: %d\tsize:%d\n", r1, r2);
	else if (r1 != 0 || r2 != 0)
		ft_printf("      ex1: %d\tex2:%d\n", r1, r2);
	(void)g_section_types;
}

static void					debug_section_64(void *d,
					const void *section_command)
{
	const struct section_64		*cmd;
	t_debug_cmd_data			*data;

	cmd = section_command;
	data = d;
	ft_printf("  Section %.16s (%.16s)\n", cmd->sectname, cmd->segname);
	ft_printf("    vmaddr: %#llx\tsize: %#llx\n", swap64i(cmd->addr),
			swap64i(cmd->size));
	ft_printf("    fileoff: %d\talign: %d\n", swap32i(cmd->offset),
			swap32i(cmd->align));
	ft_printf("    nreloc: %d\treloff: %#x\n", swap32i(cmd->nreloc),
			swap32i(cmd->reloff));
	section_flags(swap32i(cmd->flags), swap32i(cmd->reserved1),
			swap32i(cmd->reserved2));
}

void						debug_section(void *d, bool is_64,
					const void *section_command)
{
	if (is_64)
		debug_section_64(d, section_command);
	else
		(void)d;// TODO
}
