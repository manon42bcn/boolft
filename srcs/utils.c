/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:39:49 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/05 11:19:32 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	len;

	if (!s || !fd)
		return ;
	len = strlen(s);
	write(fd, s, len);
}

t_bool	ft_match_cmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (FALSE);
		i++;
	}
	if (s1[i] != s2[i])
		return (FALSE);
	return (TRUE);
}
