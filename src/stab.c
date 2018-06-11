/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stab.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 16:08:50 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 16:42:13 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "load_commands.h"

#include <ft_printf.h>

#include <stddef.h>
#include <unistd.h>

#include <nlist.h>
#include <stab.h>

static void	print_hex(const void *addr, size_t size_left)
{
	static const char hex[] = "0123456789abcdef";
	int i;
	const unsigned char *m;

	i = 0;
	m = addr;
	while (i < 16 && size_left-- > 0)
	{
		write(1, &hex[m[i] / 16], 1);
		write(1, &hex[m[i] % 16], 1);
		if (i++ % 2 == 1)
			write(1, " ", 1);
	}
	while (i < 16)
	{
		write(1, "  ", 2);
		if (i++ % 2 == 1)
			write(1, " ", 1);
	}
}

static void	print_chars(const void *addr, size_t size_left)
{
	int i;
	const unsigned char *m;

	i = 0;
	m = addr;
	while (i < 16 && size_left-- > 0)
	{
		if (0x20 <= m[i] && m[i] <= 0x7E)
			write(1, m + i, 1);
		else
			write(1, ".", 1);
		i++;
	}
}

static void	print_memory(const void *addr, size_t size)
{
	while (size >= 16)
	{
		print_hex(addr, size);
		print_chars(addr, size);
		write(1, "\n", 1);
		addr += 16;
		size -= 16;
	}
	if (size > 0)
	{
		print_hex(addr, size);
		print_chars(addr, size);
		write(1, "\n", 1);
	}
}

void			lc_stab(void *d, const void *body, size_t size)
{
	t_debug_cmd_data			*data;
	const struct symtab_command	*cmd;
	uint32_t					i;
	const char					*l;
	const char					*strtab;

	if (size != sizeof(struct symtab_command))
		return ;
	cmd = body;
	data = d;
	ft_printf("Symbol table: off=%d n=%d stroff=%d strsz=%d\n",
			swap32i(cmd->symoff), swap32i(cmd->nsyms), swap32i(cmd->stroff),
			swap32i(cmd->strsize));
	i = 0;
	l = (const char*)(((const char*)data->file->ptr) + swap32i(cmd->symoff));
	strtab = (const char*)(((const char*)data->file->ptr) + swap32i(cmd->stroff));
	print_memory(l, 16 * swap32i(cmd->nsyms));
	while (i < swap32i(cmd->nsyms)) {
		uint32_t val1 = swap32r((const void*)(l + 0));
		uint8_t unk1 = *(l + 4);
		uint8_t unk2 = *(l + 5);
		//uint16_t unk3 = swap16r(l + 6);
		uint64_t symval = swap64r((const void*)(l + 8));
		ft_printf("%016llx %3d %3d %s\n", symval, unk1, unk2, strtab + val1);
		i++;
		l += 16;
	}
}
