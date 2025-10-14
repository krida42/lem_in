/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kisikaya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 22:11:45 by kisikaya          #+#    #+#             */
/*   Updated: 2021/12/19 00:00:14 by kisikaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stream	*stream_lst = NULL;
	t_stream		*stream;
	char			*line;
	int				ret_recover_line;
	int				ret_read_stream;

	stream = gnl_recover_fd_stream(&stream_lst, fd);
	line = NULL;
	ret_recover_line = gnl_recover_line(stream, &line);
	ret_read_stream = 42;
	while (!ret_recover_line && ret_read_stream)
	{
		stream->index = 0;
		ret_read_stream = gnl_read_stream(stream);
		if (ret_read_stream == -1)
			return (gnl_free_line(line));
		ret_recover_line = gnl_recover_line(stream, &line);
	}
	if (ret_recover_line == -1)
		return (NULL);
	if (!ret_read_stream)
		gnl_clear_stream(&stream_lst, stream);
	return (line);
}

int	gnl_read_stream(t_stream *stream)
{
	int	lenr;

	lenr = read(stream->fd, stream->buf, BUFFER_SIZE);
	if (lenr > -1)
		stream->buf[lenr] = '\0';
	return (lenr);
}

void	*gnl_free_line(char *line)
{
	if (line)
		free(line);
	return (NULL);
}

void	gnl_clear_stream(t_stream **stream_lst, t_stream *stream)
{
	t_stream	*cur;
	t_stream	*prev;

	cur = *stream_lst;
	if (!*stream_lst)
		return ;
	else if ((*stream_lst)->fd == stream->fd)
	{
		*stream_lst = (*stream_lst)->next;
		free(stream);
	}
	else
	{
		while (cur->next)
		{
			prev = cur;
			cur = cur->next;
			if (cur->fd == stream->fd)
			{
				prev->next = cur->next;
				free(stream);
				break ;
			}
		}
	}
}
