/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_wchar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/04 18:45:59 by jye               #+#    #+#             */
/*   Updated: 2016/12/09 16:03:24 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>
#include <stdlib.h>

static unsigned long int	ft_wstrlen(const int *str)
{
	register const int *cp;

	if (str == NULL)
		return (0);
	cp = str;
	while (*cp)
		++cp;
	return (cp - str);
}

static unsigned long int	wchar_conv(t_format *c_flag, char *bstr, const int *qstr)
{
	unsigned long int	offset;
	int					prec;
	int					word;

	if (qstr == NULL)
		return (6);
	offset = 0;
	if (c_flag->flag & 32)
	{
		prec = c_flag->precision;
		while (*qstr)
		{
			word = w_char(*qstr++, bstr + offset);
			offset += word;
			if (offset > (unsigned int)prec)
				return (offset - word);
		}
	}
	else
		while (*qstr)
			offset += w_char(*qstr++, bstr + offset);
	return (offset);
}

static void					pp_handler(t_format *c_flag, t_conv *tmp)
{
	int		pad;

	pad = c_flag->pad - tmp->size;
	if (c_flag->flag & 2)
	{
		write(1, tmp->content, tmp->size);
		while (pad-- > 0)
			write(1, " ", 1);
	}
	else
	{
		while (pad-- > 0)
			write(1, " ", 1);
		write(1, tmp->content, tmp->size);
	}
}

int							f_wstring(t_format *c_flag, va_list arg)
{
	int					*wchar;
	char				*a;
	t_conv				tmp;
	unsigned long int	w_len;

	wchar = va_arg(arg, int *);
	a = NULL;
	if (wchar == NULL)
		a = SNULL;
	w_len = ft_wstrlen(wchar);
	if (a == NULL)
		if ((a = ft_strnew(w_len * 4)) == NULL)
			exit(EXIT_FAILURE);
	tmp.size = wchar_conv(c_flag ,a, wchar);
	tmp.content = a;
	if (c_flag->pad != 0)
		pp_handler(c_flag, &tmp);
	else
		write(1, a, tmp.size);
	if (wchar != NULL)
		free(a);
	return (tmp.size > (unsigned int)c_flag->pad ? tmp.size : c_flag->pad);
}
