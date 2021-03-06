/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strequ.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 22:59:13 by tmilon            #+#    #+#             */
/*   Updated: 2017/11/29 19:24:16 by tmilon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strequ(char const *s1, char const *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 != '\0')
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
	}
	if (*s2 != '\0')
		return (0);
	return (1);
}
