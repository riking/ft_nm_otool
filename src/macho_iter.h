/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_iter.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 13:15:48 by kyork             #+#    #+#             */
/*   Updated: 2018/06/11 14:29:23 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACHO_ITER_H
# define MACHO_ITER_H

# include <mach-o/loader.h>
# include <sys/types.h>
# include <stdbool.h>

typedef struct	s_mfile {
	const char	*ptr;
	size_t		size;
	int			fd;

	bool		is_64;
}				t_mfile;

/*
** not thread safe due to global here
*/

extern bool		g_swap_wrong_endian;

uint32_t		swap32i(const uint32_t value);
uint32_t		swap32r(const uint32_t *value);
uint64_t		swap64i(const uint64_t value);
uint64_t		swap64r(const uint64_t *value);

typedef void	(*t_callback_hdr)(void *d,
					const struct mach_header_64 *hdr);
typedef void	(*t_callback_cmd)(void *d, uint32_t type,
					const void *cmd_body, size_t cmd_size);
typedef void	(*t_callback_error)(void *d,
					int errnum, void *extra, size_t size);
typedef void	(*t_callback_segment)(void *d, bool is_64,
					const void *segment_command, size_t cmd_size);
typedef void	(*t_callback_section)(void *d, bool is_64,
					const void *section_command);

#define ERRNUM_CUSTOM		0x10000000
#define ERRNUM_STDLIB_MASK	0x0F000000

#define ERR_NOT_OBJ_FILE	(ERRNUM_CUSTOM | 0x1)
#define ERR_BAD_ALIGN		(ERRNUM_CUSTOM | 0x2)
#define ERR_TRUNC			(ERRNUM_CUSTOM | 0x3)

#define ERR_STDLIB_OPEN		(0x01000000)
#define ERR_STDLIB_CLOSE	(0x02000000)
#define ERR_STDLIB_MMAP		(0x03000000)
#define ERR_STDLIB_MUNMAP	(0x04000000)
#define ERR_STDLIB_WRITE	(0x05000000)
#define ERR_STDLIB_FSTAT	(0x06000000)

typedef struct	s_callbacks {
	void						*data;
	t_callback_hdr				hdr;
	t_callback_cmd				cmd;
	t_callback_segment			seg;
	t_callback_section			sec;
	t_callback_error			err;
}				t_callbacks;

typedef struct	s_iter {
	t_mfile						*file;
	const struct load_command	*cur_command;
	const t_callbacks			*cb;
	uint32_t					cmd_count;
	uint32_t					cmd_cur;
}				t_iter;

int				ft_mh_mfile_open(t_mfile *out, char *path);
int				ft_mh_iter_start(t_iter *iter, t_mfile *file,
					const t_callbacks *cbs);
int				ft_mh_iter_next(t_iter *iter);

void			ft_mh_iter_segment32(t_iter *iter, const void *cmd);
void			ft_mh_iter_segment64(t_iter *iter, const void *cmd);

/*
** return codes from the above three functions are 0 on success
** ft_mh_iter_next returns 0 on success, -1 on EOF
*/

#endif
