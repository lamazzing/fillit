/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillitopti.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmazzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 03:15:51 by lmazzi            #+#    #+#             */
/*   Updated: 2017/03/09 03:29:44 by lmazzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** display function, it registers all pieces by converting bits back to chars in the static table but only displays once the solution is reached
*/
char	*dis(t_lry piece, int y, int p, int sqr)
{
	static char	end[183] = {0};
	int			a;
	int			b;

	a = 0;
	if (!end[0] && !(b = 0))
		while (b < sqr && ++a)
		{
			if (a == sqr + 1 && !(a = 0) && ++b)
				end[a - 1 + b * (sqr + 1)] = '\n';
			else
				end[a - 1 + b * (sqr + 1)] = '.';
		}
	end[sqr * (sqr + 1) - 1] = '\0';
	a = -1;
	b = 0;
	while (b < 4)
	{
		++a;
		if (a == sqr && ++b)
			a = -1;
		if (((t_lry)0x8000000000000000ull >> (a + b * 16)) & piece)
			end[a + b * (sqr + 1) + y * (sqr + 1)] = p + 'A';
	}
	return (end);
}

/*
** Backtracking function, check for first available place to put the piece 1 to piece n
** if let's say n doesn't fit in the square the backtracking erases n and shifts n - 1 by one place on the right and retries to solve for n piece
** if still it doesn't fit it does the same for n - 2 and can go back till piece 1, if none of the combinations fits in the square the square is increased
** The fact that all operations are made bitwise speeds up the computation of the solution
*/

int		backtracking(t_lry *pcs, int p, int sqr, t_riche *s)
{
	int			n[4];

	ft_opti(n, s, pcs[p]);
	while (!s->p)
	{
		while ((scn(s->map, n[1], 0) & (pcs[p] >> n[0])) != 0)
			if (++n[0] && sqr < (n[0] + n[2]) && ++n[1])
				n[0] = 0;
		if (scn(s->map, n[1], 0) & (pcs[p] >> n[0])
				|| sqr < n[1] + n[3] || sqr < n[0] + n[2])
			return (0);
		as(s, (pcs[p] >> n[0]), n[1], 1);
		if (s->nbp == 1 && (s->end = dis((pcs[p] >> n[0]), n[1], p, sqr)))
			ft_putstr(s->end);
		if (p == s->nbp - 1 && (s->end = dis((pcs[p] >> n[0]), n[1], p, sqr)))
			return (1);
		if (s->nbp != 1 && !ad(pcs[p], s, n[0], n[1]))
			s->p = backtracking(pcs, p + 1, sqr, s);
		if (!s->p && as(s, (pcs[p] >> n[0]), n[1], 0) && !ad(pcs[p], s, 0, 0))
			if (++n[0] && sqr < (n[0] + n[2]) && ++n[1])
				n[0] = 0;
	}
	if ((s->end = dis((pcs[p] >> n[0]), n[1], p, sqr)) && !p)
		ft_putstr(s->end);
	return (1);
}

/*
** start from the smallest possible square based on the number of pieces to fit
** if there's no solution we increase the size of the square by one
** Creation of a bit map of 16 unsigned short, which can contain the biggest possible square in the project (16 * 16).
*/

int		begin(t_lry *pcs, t_riche *s)
{
	int				sqr;

	s->p = 0;
	sqr = ft_root(2, (4 * s->nbp));
	if (sqr * sqr < (4 * s->nbp))
		++sqr;
	while (!s->p)
	{
		s->map = (unsigned short[16]){0};
		s->p = backtracking(pcs, 0, sqr, s);
		if (!s->p)
			++sqr;
	}
	return (0);
}

/*
** conversion from map of chars to bits, every empty case corresponds to 0 every full case corresponds to 1
*/
int		ft_save(char *r, t_riche *s)
{
	t_lry *pc;

	pc = (t_lry[26]){0};
	while (++s->p < 26 && (s->y = -1))
	{
		while (++s->y < 4 && (s->x = -1))
			while (++s->x < 5)
			{
				if (s->x == 4 && (r[s->x + 5 * s->y + 21 * s->p] != '\n'))
					return (-1);
				else if (s->x < 4)
				{
					if (r[s->x + 5 * s->y + 21 * s->p] != '#' &&
							r[s->x + 5 * s->y + 21 * s->p] != '.')
						return (-1);
					pc[s->p] |= (t_lry)(r[s->x + 5 * s->y + 21 * s->p] == '#')
						<< (15 - s->x + (3 - s->y) * 16);
				}
			}
		if (rbt(&pc[s->p]) || (r[20 + 21 * s->p] != '\n' && r[20 + 21 * s->p]))
			return (-1);
		else if (r[20 + 21 * s->p] == '\0')
			break ;
	}
	return (begin(pc, s));
}

/*
** Usage: ./fillit map
** declaration of variables for later optimisation
** counting of pieces
*/

int		main(int argc, char **argv)
{
	char	r[547];
	t_riche	s[1];
	int		x;
	int		y;

	if (argc != 2 && !(s->nbp = 0))
	{
		ft_putstr("usage: ./fillit tetriminos_file\n");
		return (0);
	}
	s->opt = (t_lry[19]){0};
	s->optx = (int[19]){0};
	s->opty = (int[19]){0};
	if ((y = open(argv[1], O_RDONLY)) == -1 ||
			(x = read(y, r, 546)) == -1)
	{
		ft_putstr("error\n");
		return (0);
	}
	while (21 * s->nbp <= x)
		++s->nbp;
	r[x] = 0;
	if ((s->p = -1) && ft_save(r, s) == -1)
		ft_putstr("error\n");
	return (0);
}
