/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 16:16:18 by kyork             #+#    #+#             */
/*   Updated: 2018/06/07 16:18:22 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "macho_iter.h"

bool		g_swap_wrong_endian = false;

uint32_t	swap32i(const uint32_t value)
{
    if (g_swap_wrong_endian)
	return (__builtin_bswap32(value));
    return (value);
}

uint32_t	swap32r(const uint32_t *value)
{
    if (g_swap_wrong_endian)
	return (__builtin_bswap32(*value));
    return (*value);
}

uint64_t	swap64i(const uint64_t value)
{
    if (g_swap_wrong_endian)
	return (__builtin_bswap64(value));
    return (value);
}

uint64_t	swap64r(const uint64_t *value)
{
    if (g_swap_wrong_endian)
	return (__builtin_bswap64(*value));
    return (*value);
}
