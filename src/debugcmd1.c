/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugcmd1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 15:55:08 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 16:14:11 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "load_commands.h"

#include <libft.h>
#include <ft_printf.h>

#define ENTRY_LC(x) [x & ~LC_REQ_DYLD] = {#x, x, false}
#define ENTRY_LCX(x) [x & ~LC_REQ_DYLD] = {#x, x, true}

static const t_loadcmd_name	g_loadcmd_names[] = {
	{(char*)0, 0, false},
	ENTRY_LCX(LC_SEGMENT),
	ENTRY_LC(LC_SYMTAB),
	ENTRY_LC(LC_SYMSEG),
	ENTRY_LC(LC_THREAD),
	ENTRY_LC(LC_UNIXTHREAD),
	ENTRY_LC(LC_LOADFVMLIB),
	ENTRY_LC(LC_IDFVMLIB),
	ENTRY_LC(LC_IDENT),
	ENTRY_LC(LC_FVMFILE),
	ENTRY_LC(LC_PREPAGE),
	ENTRY_LC(LC_DYSYMTAB),
	ENTRY_LC(LC_LOAD_DYLIB),
	ENTRY_LC(LC_ID_DYLIB),
	ENTRY_LC(LC_LOAD_DYLINKER),
	ENTRY_LC(LC_ID_DYLINKER),
	ENTRY_LC(LC_PREBOUND_DYLIB),
	ENTRY_LC(LC_ROUTINES),
	ENTRY_LC(LC_SUB_FRAMEWORK),
	ENTRY_LC(LC_SUB_UMBRELLA),
	ENTRY_LC(LC_SUB_CLIENT),
	ENTRY_LC(LC_SUB_LIBRARY),
	ENTRY_LC(LC_TWOLEVEL_HINTS),
	ENTRY_LC(LC_PREBIND_CKSUM),
	ENTRY_LC(LC_LOAD_WEAK_DYLIB),
	ENTRY_LCX(LC_SEGMENT_64),
	ENTRY_LC(LC_ROUTINES_64),
	ENTRY_LC(LC_UUID),
	ENTRY_LC(LC_RPATH),
	ENTRY_LC(LC_CODE_SIGNATURE),
	ENTRY_LC(LC_SEGMENT_SPLIT_INFO),
	ENTRY_LC(LC_REEXPORT_DYLIB),
	ENTRY_LC(LC_LAZY_LOAD_DYLIB),
	ENTRY_LC(LC_ENCRYPTION_INFO),
	ENTRY_LC(LC_DYLD_INFO),
	ENTRY_LC(LC_LOAD_UPWARD_DYLIB),
	ENTRY_LC(LC_VERSION_MIN_MACOSX),
	ENTRY_LC(LC_VERSION_MIN_IPHONEOS),
	ENTRY_LC(LC_FUNCTION_STARTS),
	ENTRY_LC(LC_DYLD_ENVIRONMENT),
	ENTRY_LC(LC_MAIN),
	ENTRY_LC(LC_DATA_IN_CODE),
	ENTRY_LC(LC_SOURCE_VERSION),
	ENTRY_LC(LC_DYLIB_CODE_SIGN_DRS),
	ENTRY_LC(LC_ENCRYPTION_INFO_64),
	ENTRY_LC(LC_LINKER_OPTION),
	ENTRY_LC(LC_LINKER_OPTIMIZATION_HINT),
	ENTRY_LC(LC_VERSION_MIN_TVOS),
	ENTRY_LC(LC_VERSION_MIN_WATCHOS),
	ENTRY_LC(LC_NOTE),
	ENTRY_LC(LC_BUILD_VERSION),
};

/*
** special notes: LC_DYLD_INFO_ONLY is LC_DYLD_INFO | LC_REQ_DYLD
*/

void						debug_loadcmd(void *d, uint32_t type,
					const void *cmd_body, size_t cmd_size)
{
	t_debug_cmd_data	*data;
	uint32_t			typ_s;

	data = d;
	(void)cmd_body;
	typ_s = type & ~LC_REQ_DYLD;
	if (typ_s < ARRAYLEN(g_loadcmd_names))
	{
		if (!g_loadcmd_names[typ_s].handled)
		{
			ft_printf("Loadercmd %3x (%s)%s: %zd bytes\n", typ_s,
					g_loadcmd_names[typ_s].name,
					(type & LC_REQ_DYLD) ? " (req_dyld)" : "", cmd_size);
		}
		if (typ_s == LC_SYMTAB)
			lc_stab(d, cmd_body, cmd_size);
	}
	else
	{
		ft_printf("Loadercmd %3x (UNKNOWN)%s: %zd bytes\n", typ_s,
				(type & LC_REQ_DYLD) ? " (req_dyld)" : "", cmd_size);
	}
}

