/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_commands.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 13:26:38 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 14:26:01 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOAD_COMMANDS_H
# define LOAD_COMMANDS_H

# include "macho_iter.h"

typedef struct		s_debug_cmd_data {
	t_mfile		*file;
}					t_debug_cmd_data;

void				debug_loadcmd(void *d, uint32_t type,
			const void *cmd_body, size_t cmd_size);
void				debug_segment(void *d, bool is_64,
			const void *cmd_body, size_t cmd_size);
void				debug_section(void *d, bool is_64,
			const void *section_command);

#endif
