/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmazzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/01 01:40:01 by lmazzi            #+#    #+#             */
/*   Updated: 2017/03/06 10:50:21 by lmazzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** first part of the function creates a temporary piece that is the sum of a section of the final square map (only four lines) plus the new piece added.
** the four lines of the final square map are casted one by one from unsigned short to unsigned long long, to create a temporary section where 
** the piece (also an unsigned long long) is inserted using the operator |.
**
** the second part is to compute the size of the pieces on the x and y axes to check later if the piece goes beyond the borders of the square.
*/

t_lry	scn(unsigned short *map, int xy, t_lry piece)
{
	t_lry	new;

	if (!piece)
	{
		new = (t_lry)map[xy] << 48;
		new |= (t_lry)map[xy + 1] << 32 | (t_lry)map[xy + 2] << 16 |
			map[xy + 3];
		return (new);
	}
	else
	{
		if (xy == 'x' && (xy = 3))
			while (!(piece & (TXMAP >> xy)))
				--xy;
		if (xy == 'y' && (xy = 3))
			while (!(piece & (TYMAP >> (16 * xy))))
				--xy;
		return (xy + 1);
	}
}
/*
** put the new piece in the map using the bit operator |
** erase the piece from the map using the bit operator ^
*/
void	*as(t_riche *s, t_lry t, int y, int choice)
{
	int	a;

	a = -1;
	if (choice)
		while (++a < 4)
			s->map[a + y] |= (unsigned short)(t >> ((3 - a) * 16));
	else
		while (++a < 4)
			s->map[a + y] ^= (unsigned short)(t >> ((3 - a) * 16));
	return (s);
}

int		ad(t_lry pc, t_riche *s, int x, int y)
{
	int		a;

	a = 0;
	while (s->opt[a] != pc && s->opt[a])
		++a;
	s->opt[a] = pc;
	s->optx[a] = x;
	s->opty[a] = y;
	return (0);
}
/*
** setting of the basic variables to be used in the computations
** if a similar piece has already been seen before it gets the optx, opty coordinates so it skips some of the possibilities
** thus saving some computation time
*/
void	ft_opti(int *n, t_riche *s, t_lry pc)
{
	int			a;

	a = 0;
	while (s->opt[a] != pc && s->opt[a])
		++a;
	n[0] = s->optx[a];
	n[1] = s->opty[a];
	s->p = 0;
	n[2] = (int)scn(NULL, 'x', pc);
	n[3] = (int)scn(NULL, 'y', pc);
}
/*
** shift the provided piece as high (*p << 16) left (*p << 1) as possible to make comparisons simpler
** check if the piece provided is one of the correct forms defined in exa in the .h
** otherwise error
*/

int		rbt(t_lry *p)
{
	if (*p)
	{
		while (!(T2 & *p))
			*p = *p << 1;
		while (!(T1 & *p))
			*p = *p << 16;
		if (!(*p ^ T1) || !(*p ^ T2) || !(*p ^ T3) || !(*p ^ T4) ||
				!(*p ^ T5) || !(*p ^ T6) || !(*p ^ T7) || !(*p ^ T8) ||
				!(*p ^ T9) || !(*p ^ T10) || !(*p ^ T11) || !(*p ^ T12) ||
				!(*p ^ T13) || !(*p ^ T14) || !(*p ^ T15) || !(*p ^ T16) ||
				!(*p ^ T17) || !(*p ^ T18) || !(*p ^ T19))
			return (0);
	}
	return (-1);
}
