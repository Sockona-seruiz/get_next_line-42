/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line_utils.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/21 09:10:31 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/11/27 09:43:23 by seruiz      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "get_next_line.h"

void		ft_free(t_fd_list *first, char *line, char *str)
{
	t_fd_list	*list;
	t_fd_list	*tmp;

	if (str != 0)
		free(str);
	if (line != 0)
		free(line);
	list = first;
	tmp = NULL;
	while (list)
	{
		if (list->next)
			tmp = list->next;
		else
			tmp = NULL;
		free(list->buff);
		free(list);
		list = tmp;
	}
}

int			ft_free_lst(t_fd_list *first_fd, t_fd_list *current_fd)
{
	t_fd_list	*list;
	t_fd_list	*tmp;

	list = first_fd;
	tmp = NULL;
	while (list)
	{
		if (list->next != NULL)
			tmp = list->next;
		if (tmp == current_fd)
		{
			list->next = tmp->next;
			if (current_fd->buff != NULL)
				free(current_fd->buff);
			current_fd->buff = NULL;
			free(current_fd);
			current_fd = NULL;
			break ;
		}
		list = tmp;
	}
	return (0);
}

long int	ft_eof(char *line, t_fd_list *first_fd,
			t_fd_list *current_fd, int ret)
{
	long int i;

	i = 0;
	if (ret == -2)
	{
		while (line[i])
			i++;
		return (i);
	}
	if (first_fd == current_fd)
	{
		first_fd = first_fd->next;
		if (current_fd->buff != NULL)
			free(current_fd->buff);
		current_fd->buff = NULL;
		return ((int)0);
	}
	if (ret == 0)
		return (ft_free_lst(first_fd, current_fd));
	if (ret == -1)
		ft_free(first_fd, line, 0);
	return (ret == -1 ? (int)-1 : (int)0);
}

t_fd_list	*ft_lstnewfd(int fd, t_fd_list *first_fd)
{
	t_fd_list	*result;
	int			nbr;

	nbr = fd;
	if ((result = malloc(sizeof(t_fd_list))) == 0)
	{
		ft_free(first_fd, 0, 0);
		return (0);
	}
	result->fd = nbr;
	result->buff = NULL;
	result->next = NULL;
	return (result);
}

char		*ft_strjoin(char *s1, char *s2, t_fd_list *first_fd)
{
	char	*dest;
	size_t	i;
	size_t	j;
	size_t	len;
	size_t	linelen;

	linelen = ft_eof(s1, NULL, NULL, -2);
	len = linelen + ft_eof(s2, NULL, NULL, -2);
	i = -1;
	j = -1;
	if (NULL == (dest = (char *)malloc(sizeof(char) * (len + 1))))
	{
		ft_free(first_fd, s1, s2);
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
