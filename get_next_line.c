/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/28 11:13:00 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/11/28 14:59:33 by seruiz      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

# include "get_next_line.h"

int		ft_free(char *s1, char *s2, t_fd_list *lst)
{
	if (s1 != NULL)
		free(s1);
	if (s2 != NULL)
		free(s2);
	if (lst->buff != NULL)
		free(lst->buff);
	return (-1);
}

size_t	ft_strlen(char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2, t_fd_list *lst)
{
	char    *dest;
	size_t  i;
	size_t  j;
	size_t  len;
	size_t  linelen;

	linelen = ft_strlen(s1);
	len = linelen + ft_strlen(s2);
	i = -1;
	j = -1;
	if (NULL == (dest = (char *)malloc(sizeof(char) * (len + 1))))
	{
		ft_free(s1, s2, lst);
		return (NULL);
	}
	while (s1[++i])
		dest[i] = s1[i];
	while (s2[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	free(s2);
	if (s1 != NULL)
		free(s1);
	return (dest);
}

int		ft_setfd_buff(int i, char *reader, t_fd_list *lst)
{
	char	*result;
	int		j;

	j = 0;
	if ((result = malloc(sizeof(char) * (BUFFER_SIZE - i))) == 0)
		return (ft_free(reader, NULL, lst));
	i++;
	while (reader[i])
	{
		result[j] = reader[i];
		i++;
		j++;
	}
	result[j] = '\0';
	if (lst->buff != NULL)
		free(lst->buff);
	lst->buff = result;
	return (0);
}

int		compute_buff(char *reader, char **line, t_fd_list *lst)
{
	int i;
	char *buff;

	if ((buff = malloc(sizeof(char) * (ft_strlen(reader) + 1))) == 0)
	{
		printf("test");
		return (-1);
	}
	i = 0;
	while (reader[i])
	{
		buff[i] = reader[i];
		if (reader[i] == '\n')
		{
			buff[i] = '\0';
			*line = ft_strjoin(*line, buff, lst);
			return (ft_setfd_buff(i, reader, lst));
		}
		i++;
	}
	buff[i] = '\0';
	*line = ft_strjoin(*line, buff, lst);
	return (1);
}

int		ft_eof(t_fd_list *lst)
{
	if (lst->buff != NULL)
		free(lst->buff);
	lst->buff = NULL;
	return (0);
}

int		treat_buff(int fd, char **line, t_fd_list *lst)
{
	int ret;
	char *reader;
	int retval;
	int test;

	test = 0;
	retval = 0;
	if (lst->buff != NULL)
		if ((test = compute_buff(lst->buff, line, lst)) == 0)
			return (1);
	if (test == -1)
		return (-1);
	if ((reader = malloc(sizeof(char) * (BUFFER_SIZE + 1))) == 0)
		return (ft_free(NULL, NULL, lst));
	if ((ret = read(fd, reader, BUFFER_SIZE)) <= 0)
		retval = 1;
	reader[ret] = '\0';
	while (retval == 0 && ((test = compute_buff(reader, line, lst)) == 1))
	{
		if (test == -1)
			return (-1);
		if ((ret = read(fd, reader, BUFFER_SIZE)) <= 0)
			retval = 1;
		reader[ret] = '\0';
	}
	free(reader);
	if (test == -1)
		return (-1);
	return (retval == 1 ? ft_eof(lst) : 1);
}

int		get_next_line(int fd, char **line)
{
	static t_fd_list	static_lst[FD_SETSIZE];

	if (fd < 0 || (read(fd, 0, 0)) < 0 || line == NULL
		|| BUFFER_SIZE <= 0)
		return (-1);
	if ((*line = malloc(sizeof(char) * 1)) == 0)
	{
		if (static_lst[fd].buff != NULL)
			free(static_lst[fd].buff);
		return (-1);
	}
	*line[0] = '\0';
	return (treat_buff(fd, line, &static_lst[fd]));
}
