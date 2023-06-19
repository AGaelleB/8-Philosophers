/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 14:44:40 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 12:04:51 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdint.h>

char	*get_next_line(int fd);
size_t	ft_strlen_gnl(const char *str);
char	*ft_strchr_gnl(const char *s, int c);
char	*ft_strlcpy_gnl(char *dst, char *src);
char	*ft_strlcat_gnl(char *dst, char *src);
void	*ft_calloc_gnl(size_t nmemb, size_t size);
char	*ft_strjoin_gnl(char *str, char *buf);

#endif