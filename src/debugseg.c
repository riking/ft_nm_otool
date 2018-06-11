/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 13:30:06 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 14:27:59 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "macho_iter.h"
#include "load_commands.h"

#include <ft_printf.h>

static void		seg_print_flags(uint32_t flags)
{
	ft_printf("  flags:");
	if (flags & SG_HIGHVM)
		ft_printf(" highvm");
	if (flags & SG_FVMLIB)
		ft_printf(" fvmlib");
	if (flags & SG_NORELOC)
		ft_printf(" noreloc");
	if (flags & SG_PROTECTED_VERSION_1)
		ft_printf(" protv1");
	ft_printf("\n");
}

void			debug_segment_32(void *d, const void *body, size_t cmd_size)
{
	const struct segment_command_64	*cmd;
	t_debug_cmd_data				*data;

	data = d;
	cmd = body;
	ft_printf("Section type LC_SEGMENT: %zd bytes\n", cmd_size);
	ft_printf("  segment name: %.16s\n", cmd->segname);
	ft_printf("  vmaddr: %lld\tvmsize: %lld\n", swap64i(cmd->vmaddr),
			swap64i(cmd->vmsize));
	ft_printf("  fileoff: %lld\tfilesize: %lld\n", swap64i(cmd->fileoff),
			swap64i(cmd->filesize));
	ft_printf("  maxprot: %llx\tinitprot: %llx\tsections: %d\n",
			swap64i(cmd->maxprot), swap64i(cmd->initprot),
			swap32i(cmd->nsects));
	seg_print_flags(swap32i(cmd->flags));
	(void)data;
}

void			debug_segment_64(void *d, const void *body, size_t cmd_size)
{
	const struct segment_command_64	*cmd;
	t_debug_cmd_data				*data;

	data = d;
	cmd = body;
	ft_printf("Section type LC_SEGMENT_64: %zd bytes\n", cmd_size);
	ft_printf("  segment name: %.16s\n", cmd->segname);
	ft_printf("  vmaddr: %lld\tvmsize: %lld\n", swap64i(cmd->vmaddr),
			swap64i(cmd->vmsize));
	ft_printf("  fileoff: %lld\tfilesize: %lld\n", swap64i(cmd->fileoff),
			swap64i(cmd->filesize));
	ft_printf("  maxprot: %llx\tinitprot: %llx\tsections: %d\n",
			swap64i(cmd->maxprot), swap64i(cmd->initprot),
			swap32i(cmd->nsects));
	seg_print_flags(swap32i(cmd->flags));
	(void)data;
}

void			debug_segment(void *d, bool is_64,
		const void *cmd_body, size_t cmd_size)
{
	if (is_64)
		debug_segment_64(d, cmd_body, cmd_size);
	else
		debug_segment_32(d, cmd_body, cmd_size);
}
