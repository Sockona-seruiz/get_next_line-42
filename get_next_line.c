/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line_char.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/18 13:36:29 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 11:46:43 by seruiz      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "get_next_line.h"

void		ft_setfd_buff(char *reminder, t_fd_list *current_fd,
			t_fd_list *first_fd, char *line)
{
	int			i;
	int			j;
	char		*result;

	j = 0;
	while (reminder[j] != '\n')
		j++;
	j++;
	i = ft_strlen(reminder) - j;
	if ((result = malloc(sizeof(char) * (i + 1))) == 0)
		return (ft_free(first_fd, reminder, line));
	i = 0;
	while (reminder[j])
	{
		result[i] = reminder[j];
		i++;
		j++;
	}
	result[i] = '\0';
	if (reminder != current_fd->buff)
		free(reminder);
	free(current_fd->buff);
	current_fd->buff = result;
}

int			compute_buff(char *buff, t_fd_list *current_fd,
			char **line, t_fd_list *first_fd)
{
	char	*buffchar;
	int		i;

	if ((buffchar = malloc(sizeof(char) * (BUFFER_SIZE + 1))) == 0)
	{
		ft_free(first_fd, *line, buff);
		return (-1);
	}
	i = 0;
	while (buff[i])
	{
		buffchar[i] = buff[i];
		if (buff[i] == '\n')
		{
			buffchar[i] = '\0';
			*line = ft_strjoin(*line, buffchar, first_fd);
			ft_setfd_buff(buff, current_fd, first_fd, *line);
			return (0);
		}
		i++;
	}
	buffchar[i] = '\0';
	*line = ft_strjoin(*line, buffchar, first_fd);
	return (1);
}

int			treat_buff(char *str, t_fd_list *crfd, char **line,
			t_fd_list *first_fd)
{
	char	*reader;
	int		ret;
	int		retval;

	retval = 0;
	if (str != NULL)
		if ((compute_buff(str, crfd, line, first_fd)) == 0)
			return (1);
	if ((reader = malloc(sizeof(char) * (BUFFER_SIZE + 1))) == 0)
	{
		ft_free(first_fd, *line, 0);
		return (-1);
	}
	if ((ret = read(crfd->fd, reader, BUFFER_SIZE)) <= 0)
		retval = 1;
	reader[ret] = '\0';
	while (retval == 0 && ((compute_buff(reader, crfd, line, first_fd))) == 1)
	{
		if ((ret = read(crfd->fd, reader, BUFFER_SIZE)) <= 0)
			retval = 1;
		reader[ret] = '\0';
	}
	if (retval)
		free(reader);
	return (retval == 1 ? ft_eof(*line, first_fd, crfd, ret) : 1);
}

t_fd_list	*search_fd(int fd, t_fd_list *first_fd)
{
	t_fd_list	*current_fd;

	current_fd = first_fd;
	if (current_fd == 0)
		return (0);
	while (current_fd->next != NULL)
	{
		if (current_fd->fd == fd)
			return (current_fd);
		current_fd = current_fd->next;
	}
	return (current_fd);
}

int			get_next_line(int fd, char **line)
{
	static t_fd_list	*first_fd;
	t_fd_list			*current_fd;
	char				*str;

	if ((*line = malloc(sizeof(char) * 1)) == 0)
		return (-1);
	*line[0] = '\0';
	str = NULL;
	if (fd < 0 || (read(fd, 0, 0)) < 0)
		return (-1);
	current_fd = search_fd(fd, first_fd);
	if (current_fd == NULL)
	{
		first_fd = ft_lstnewfd(fd, first_fd);
		current_fd = first_fd;
	}
	else if (current_fd->fd == fd)
		str = current_fd->buff;
	else
	{
		current_fd->next = ft_lstnewfd(fd, first_fd);
		current_fd = current_fd->next;
	}
	return (treat_buff(str, current_fd, line, first_fd));
}
