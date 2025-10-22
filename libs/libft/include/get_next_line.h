/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kisikaya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 22:11:50 by kisikaya          #+#    #+#             */
/*   Updated: 2021/12/18 23:31:12 by kisikaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# define BUFFER_SIZE 32

typedef struct s_stream
{
	int				index;
	int				fd;
	char			buf[BUFFER_SIZE + 1];
	struct s_stream	*next;
}	t_stream;

char		*get_next_line(int fd);
t_stream	*gnl_newstream(int fd);
void		gnl_printstream(t_stream *stream_lst);
t_stream	*gnl_recover_fd_stream(t_stream **stream_lst, int fd);
int			gnl_recover_line(t_stream *stream, char **line);
int			gnl_strchri(char *s, unsigned char c);
char		*gnl_strjoin(char *s1, char *s2);
int			gnl_read_stream(t_stream *stream);
void		*gnl_free_line(char *line);
void		gnl_clear_stream(t_stream **stream_lst, t_stream *stream);
#endif
