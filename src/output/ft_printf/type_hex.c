/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_hex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycorrupt <ycorrupt@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:33:09 by kokeefe           #+#    #+#             */
/*   Updated: 2020/07/02 22:37:24 by ycorrupt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#define FHASH format->hash
#define FPRECISION format->precision
#define HEXRES buffs[0]
#define HEXSET buffs[1]

/*
**	In any "place" function (dir type_funcs) there is the same sort of array,
**	where:
**	arr[0]	-	no length flags.
**	arr[1]	-	-hh- length flag.
**	arr[2]	-	-h- length flag.
**	arr[3]	-	-l- length flag.
**	arr[4]	-	-z- length flag.
**	arr[5]	-	-j- length flag.
**	arr[6]	-	-ll- length flag.
**	arr[7]	-	-L- length flag.
**	If unsupported length flag was given in format string, a default tip should
**	be chosen.
*/

static t_extract_hex_rout		*ft_get_tip(uint8_t num)
{
	static t_extract_hex_rout	*array[8];

	if (!array[0])
	{
		array[0] = &ft_get_hex_default;
		array[1] = &ft_get_hex_short_short;
		array[2] = &ft_get_hex_short;
		array[3] = &ft_get_hex_long;
		array[4] = &ft_get_hex_long;
		array[5] = &ft_get_hex_long;
		array[6] = &ft_get_hex_long_long;
		array[7] = &ft_get_hex_long_cap;
	}
	return (array[num]);
}

inline static void				ft_printout(t_format *formstat, \
									t_fword *format, char *res, char *bogey)
{
	size_t						dims[2];

	if (*res)
	{
		dims[0] = ft_strlen(res);
		dims[1] = dims[0];
		if (format->filler == '0')
		{
			filling_mainbuf(res, 2, 0, formstat->fd);
			dims[1] -= 2;
			format->ftp_width(formstat, format, &dims[0], bogey);
		}
		else
			format->ftp_width(formstat, format, &dims[0], res);
	}
	else
	{
		dims[0] = ft_strlen(bogey);
		dims[1] = dims[0];
		format->ftp_width(formstat, format, &dims[0], bogey);
	}
}

uint8_t							ft_place_hexal(t_format *formstat, \
													t_fword *format)
{
	char						*buffs[2];
	char						*bogey;
	char						*buffer;

	if ((format->flags))
		format->filler = ' ';
	else
		format->precision = 1;
	if (!(buffer = (char *)ft_memalloc(24UL + FPRECISION + (FHASH * 2))))
		return (1);
	HEXRES = buffer + 1;
	HEXSET = "0123456789abcdef";
	bogey = ft_get_tip(format->length_t)(format, &buffs[0]);
	if (!*(bogey + 1))
		FHASH = 0;
	HEXRES = buffer;
	ft_memset((void *)(HEXRES + 2), '0', bogey + 1 - (HEXRES + 2));
	bogey = HEXRES + 2;
	if (FHASH && *bogey)
		*(short int *)HEXRES = (short int)30768U;
	ft_printout(formstat, format, HEXRES, bogey);
	free(HEXRES);
	if (formstat->errflag)
		return (1);
	return (0);
}
