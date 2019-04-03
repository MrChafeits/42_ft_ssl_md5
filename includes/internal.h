/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: callen <callen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:16:14 by callen            #+#    #+#             */
/*   Updated: 2019/04/02 20:16:16 by callen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

int		get_command_(const char *s);
int		len_strtab(char **t);
void	ft_free_strtab(char ***tab);
int		panic_(int fd, char *str);

#endif
