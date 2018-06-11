/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_commands.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 13:26:38 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 16:13:45 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOAD_COMMANDS_H
# define LOAD_COMMANDS_H

# include "macho_iter.h"

typedef struct		s_debug_cmd_data {
	t_mfile		*file;
}					t_debug_cmd_data;

typedef struct		s_loadcmd_name {
	const char	*name;
	uint32_t	type;
	bool		handled;
}					t_loadcmd_name;

void				debug_loadcmd(void *d, uint32_t type,
			const void *cmd_body, size_t cmd_size);
void				debug_segment(void *d, bool is_64,
			const void *cmd_body, size_t cmd_size);
void				debug_section(void *d, bool is_64,
			const void *section_command);

void				debug_segment_64(void *d,
			const void *cmd_body, size_t cmd_size);
void				debug_segment_32(void *d,
			const void *cmd_body, size_t cmd_size);

void				lc_stab(void *d, const void *body, size_t size);

#endif
