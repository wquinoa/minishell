/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_start.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wquinoa <wquinoa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 19:00:39 by jalvaro           #+#    #+#             */
/*   Updated: 2020/07/25 16:21:01 by wquinoa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				prs_args_check(t_env *env, void **beg, char *buf)
{
	t_prs	*prs;

	prs = *beg;
	while (prs)
	{
		if (prs->arg && prs->command == '|' && (!prs->command ||
			!prs->next->arg))
		{
			write(1, "> ", 2);
			if (!parseargs(env, prs->next, *beg, buf))
				return (0);
			if (!prs->next || !prs->next->arg)
				continue ;
		}
		if (prs->command && (!prs->next || !prs->next->arg))
		{
			ft_fput("%s: %c\n", SYNTAX_ERR, &prs->command, 2);
			prslst_free(*beg);
			*beg = prslstback(prs, 0);
		}
		prs = prs->next;
	}
	return (1);
}

t_prs			*parse_start(t_env *env)
{
	t_prs	*prs;
	void	*beg;
	char	*buf;

	prs = 0;
	prs = prslstback(prs, 0);
	beg = prs;
	buf = malloc(1);
	if (!prs || !buf || read(0, buf, 0) == -1)
	{
		prs ? free(prs) : 0;
		return (0);
	}
	ft_fput(PROMPT, SHELL, g_shell.cwd, 1);
	if (!parseargs(env, prs, beg, buf))
		return (0);
	prs = beg;
	if (!prs_args_check(env, &beg, buf))
		ft_perror_exit("b42h");
	free(buf);
	return (beg);
}