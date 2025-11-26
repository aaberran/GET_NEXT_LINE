/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaberran <aaberran@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 12:20:06 by aaberran          #+#    #+#             */
/*   Updated: 2025/11/24 18:27:37 by aaberran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char **stash)
{
	char	*line;
	int		i;

	if (!stash || !*stash)
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
	if (!(*stash)[i])
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
	static char	*stash;
	char		*line;

	if (fd < 0)
		return (free(stash), stash = NULL, NULL);
	if (BUFFER_SIZE <= 0 || BUFFER_SIZE >= 2147483647)
		return (free(stash), stash = NULL, NULL);
	stash = read_and_join(fd, &stash);
	if (!stash || !*stash)
		return (free(stash), stash = NULL, NULL);
	line = extract_line(&stash);
	if (!line)
		return (free(stash), stash = NULL, NULL);
	stash = save_rest(&stash);
	return (line);
}
