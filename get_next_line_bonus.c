/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaberran <aaberran@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 21:04:58 by aaberran          #+#    #+#             */
/*   Updated: 2025/11/11 21:29:06 by aaberran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*extract_line(char **stash)
{
	char	*line;
	int		i;

	if (!stash || !*stash || !**stash)
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	line = malloc(i + ((*stash)[i] == '\n') + 1);
	if (!line)
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n')
	{
		line[i] = (*stash)[i];
		i++;
	}
	if ((*stash)[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*save_rest(char **stash)
{
	int		i;
	int		j;
	char	*rest;

	if (!stash || !*stash)
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	if (!(*stash)[i] || !(*stash)[i + 1])
		return (free(*stash), *stash = NULL, NULL);
	rest = malloc(ft_strlen(*stash) - i);
	if (!rest)
		return (free(*stash), *stash = NULL, NULL);
	i++;
	j = 0;
	while ((*stash)[i])
		rest[j++] = (*stash)[i++];
	rest[j] = '\0';
	free(*stash);
	*stash = NULL;
	return (rest);
}

static char	*join_and_free(char *stash, char *buf)
{
	char	*tmp;

	tmp = stash;
	stash = ft_strjoin(stash, buf);
	if (!stash)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (stash);
}

static char	*read_and_join(int fd, char **stash)
{
	char	*buf;
	ssize_t	bytes;

	if (!*stash)
		*stash = ft_strdup("");
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(*stash), *stash = NULL, NULL);
	bytes = 1;
	while (bytes > 0 && !ft_strchr(*stash, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buf), free(*stash), *stash = NULL, NULL);
		buf[bytes] = '\0';
		*stash = join_and_free(*stash, buf);
		if (!*stash)
			return (free(buf), NULL);
		if (bytes == 0)
			break ;
	}
	free(buf);
	return (*stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[OPEN_MAX];
	char		*line;

	if (fd < 0 || fd >= OPEN_MAX)
		return (NULL);
	if (BUFFER_SIZE <= 0 || BUFFER_SIZE >= 2147483647)
		return (NULL);
	stash[fd] = read_and_join(fd, &stash[fd]);
	if (!stash[fd] || !*stash[fd])
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	line = extract_line(&stash[fd]);
	if (!line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	stash[fd] = save_rest(&stash[fd]);
	return (line);
}
