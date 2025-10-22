/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kisikaya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 22:11:47 by kisikaya          #+#    #+#             */
/*   Updated: 2021/12/18 23:58:12 by kisikaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

t_stream* gnl_newstream(int fd)
{
	t_stream* stream;

	stream = (t_stream*)malloc(sizeof(*stream));
	if (!stream)
		return (NULL);
	stream->fd = fd;
	stream->next = NULL;
	stream->index = 0;
	stream->buf[0] = '\0';
	return (stream);
}

t_stream* gnl_recover_fd_stream(t_stream** stream_lst, int fd)
{
	t_stream* cur;
	t_stream* tmp;

	if (!*stream_lst)
	{
		*stream_lst = gnl_newstream(fd);
		return (*stream_lst);
	}
	cur = *stream_lst;
	while (cur)
	{
		if (cur->fd == fd)
			return (cur);
		tmp = cur;
		cur = cur->next;
	}
	tmp->next = gnl_newstream(fd);
	return (tmp->next);
}

int gnl_recover_line(t_stream* stream, char** line)
{
	int len;
	char* little;
	char* temp_line;
	int index_temp;

	len = gnl_strchri(stream->buf + stream->index, '\n') + 1;
	temp_line = *line;
	if (len == 0)
	{
		*line = gnl_strjoin(*line, stream->buf + stream->index);
		gnl_free_line(temp_line);
		return (0);
	}
	little = (char*)malloc(sizeof(*little) * (len + 1));
	if (!little && !gnl_free_line(*line))
		return (-1);
	index_temp = stream->index;
	while ((stream->index) < index_temp + len)
		*little++ = stream->buf[stream->index++];
	*little = '\0';
	*line = gnl_strjoin(*line, little - len);
	free(little - len);
	gnl_free_line(temp_line);
	return (1);
}

int gnl_strchri(char* s, unsigned char c)
{
	int i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
		return (i);
	else
		return (-1);
}

char* gnl_strjoin(char* s1, char* s2)
{
	char* joined;
	size_t len;

	len = gnl_strchri(s1, '\0') + gnl_strchri(s2, '\0');
	if (!len)
		return (NULL);
	joined = (char*)malloc(sizeof(char) * (len + 1));
	if (!joined)
		return (NULL);
	while (s1 && *s1)
		*joined++ = *s1++;
	while (s2 && *s2)
		*joined++ = *s2++;
	*joined = '\0';
	return (joined - len);
}
