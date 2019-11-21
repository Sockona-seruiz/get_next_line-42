/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line_char.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/18 13:36:29 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/11/21 10:21:03 by seruiz      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

//#define BUFFER_SIZE 4

typedef	struct s_fd_list
{
	int             	fd;
	char            	*buff;
	struct s_fd_list	*next;
}   t_fd_list;

void	ft_free(t_fd_list *first, char *line)
{
	t_fd_list  *list;
	t_fd_list  *tmp;

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

size_t  ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

t_fd_list	*ft_lstnewfd(int fd, t_fd_list *first_fd)
{
	t_fd_list  *result;
	int			nbr;

	nbr = fd;
	if ((result = malloc(sizeof(t_fd_list *))) == 0)
	{
		ft_free(first_fd, 0);
		return (0);
	}
	result->fd = nbr;
	result->buff = NULL;
	result->next = NULL;
	return (result);
}

void    ft_setfd_buff(char *reminder, t_fd_list *current_fd, t_fd_list *first_fd)
{
	int i;
	char *result;

	i = ft_strlen(reminder);
	/*
	if (i == 0)
	{
		current_fd->buff = NULL;
		return ;
	}
	*/
	if ((result = malloc(sizeof(char) * (i + 1))) == 0)
		return (ft_free(first_fd, 0));
	i = 0;
	while (reminder[i])
	{
		result[i] = reminder[i];
		i++;
	}
	result[i] = '\0';
	printf("Reminder = %s\n", result);
	current_fd->buff = result;
}

char    *ft_strjoin(char *s1, char *s2, t_fd_list *first_fd)
{
	char    *dest;
	size_t  i;
	size_t  j;
	size_t  len;

	//if (!s1 || !s2) Has been comented, not sure
	//	return (s1);
	len = ft_strlen(s1) + ft_strlen(s2);
	i = -1;
	j = -1;
	if (NULL == (dest = (char *)malloc(sizeof(char) * (len + 1))))
	{
		ft_free(first_fd, s1);
		return (NULL);
	}
	while (s1[++i])
		dest[i] = s1[i];
	while (s2[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	//printf("dest = %s  ", dest);
	return (dest);
}

int		compute_buff(char *buff, t_fd_list *current_fd, char **line, t_fd_list *first_fd)
{
	char buffchar[BUFFER_SIZE + 1];
	int i;

	i = 0;
	while (buff[i])
	{
		buffchar[i] = buff[i];
		if (buff[i] == '\n')
		{
			buffchar[i] = '\0';
			//printf("last = %s\n", buffchar);
			*line = ft_strjoin(*line, buffchar, first_fd);
			//printf("line = %s\n", *line);
			ft_setfd_buff(buff+i+1, current_fd, first_fd);
			return (0);
		}
		i++;
	}
	buffchar[i] = '\0';
	*line = ft_strjoin(*line, buffchar, first_fd);
	//printf("line itter = %s\n", *line);
	return (1);
}

int		treat_buff(char *str, t_fd_list *current_fd, char **line, t_fd_list *first_fd)
{
	char    reader[BUFFER_SIZE + 1];
	int     ret;

	if (str != NULL) //On a donc un reminder
	{
		printf("reminder was : %s\n", str);
		if ((compute_buff(str, current_fd, line, first_fd)) == 0)
			return (1); //Meaning buff has a \n
	}
	if ((ret = read(current_fd->fd, reader, BUFFER_SIZE)) <= 0)
		return (ret); //Il faudrais return ret car ret = -1 = erreur / ret = 0 = EOF / ret > 0 = ligne lue
	reader[ret] = '\0';
	while ((compute_buff(reader, current_fd, line, first_fd)) == 1)
	{
		if ((ret = read(current_fd->fd, reader, BUFFER_SIZE)) <= 0)
			return (ret); //same here
		reader[ret] = '\0';
	}
	return (1);
}

t_fd_list *search_fd(int fd, t_fd_list *first_fd)
{
	t_fd_list *current_fd;

	current_fd = first_fd;
	if (current_fd == 0)
		return (0);
	while (current_fd->next != NULL) //&& (current_fd == first_fd || first_fd != NULL)) // On parcours la liste des fd
	{
		printf("current_fd->fd = %d\n", current_fd->fd);
		if (current_fd->fd == fd)
		{
			printf("retrurned current_fd\n");
			return (current_fd);
		}
		current_fd = current_fd->next;
	}
	return (current_fd);
}

int     get_next_line(int fd, char **line)
{
	static t_fd_list	*first_fd;
	t_fd_list			*current_fd;
	char				*str;
	int					ret;

	if (first_fd != NULL)
		printf("First fd : %d", first_fd->fd);
	*line = "\0";
	str = NULL;
	if (fd < 0 || (read(fd, 0, 0)) < 0)
		return (-1);
	printf("fd %d opened\n", fd);
	current_fd = search_fd(fd, first_fd);
	if (current_fd == NULL)
	{
		printf("First fd : %d\n", fd);
		first_fd = ft_lstnewfd(fd, first_fd);
		current_fd = first_fd;
	}
	else if (current_fd->fd == fd)
	{
		printf("fd %d found\n", fd);
		printf("Reminder was : %s\n", current_fd->buff);
		str = current_fd->buff;
	}
	else
	{
		printf("fd %d not found\n", fd);
		current_fd->next = ft_lstnewfd(fd, first_fd);
		printf("Current_fd->fd = %d\n", current_fd->fd);
		current_fd = current_fd->next;
		printf("Current_fd->fd = %d\n", current_fd->fd);
	}
	if ((ret = treat_buff(str, current_fd, line, first_fd)) > 0)
		return (1);
	return (ret);
}

int main(void)
{
	int fd;
	int fd2;
	int fd3;
	int ret;
	char *buff;

	printf("----------------------------------------------------------------------------------------\n");
	fd = open("test.txt", O_RDONLY);
	fd2 = open("test_2.txt", O_RDONLY);
	fd3 = open("test_3.txt", O_RDONLY);

	while((ret = get_next_line(fd, &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
		if (ret == 0)
			break;
	}
	/*
	printf("%d\n", get_next_line(fd, &buff));
	printf("result : %s\n\n", buff);
	free(buff);
	printf("%d\n", get_next_line(fd, &buff));
	printf("result : %s\n\n", buff);
	free(buff);
	printf("%d\n", get_next_line(fd, &buff));
	printf("result : %s\n\n", buff);
	free(buff);
	printf("%d\n", get_next_line(fd, &buff));
	printf("result : %s\n\n", buff);
	free(buff);
	printf("%d\n", get_next_line(fd, &buff));
	printf("result : %s\n\n", buff);
	free(buff);
	printf("%d\n", get_next_line(fd, &buff));
	printf("result : %s\n\n", buff);
	free(buff);
	get_next_line(fd2, &buff);
	printf("result : %s\n\n", buff);
	free(buff);
	get_next_line(fd2, &buff);
	printf("result : %s\n\n", buff);
	free(buff);
	get_next_line(fd2, &buff);
	printf("result : %s\n\n", buff);
	free(buff);

	get_next_line(fd3, &buff);
	printf("result : %s\n\n", buff);
	free(buff);
	get_next_line(fd3, &buff);
	printf("result : %s\n\n", buff);
	free(buff);
	get_next_line(fd3, &buff);
	printf("result : %s\n\n", buff);
	free(buff);
	
	   get_next_line(fd, &buff);
	   printf("%s\n\n", buff);
	   free(buff);
	   get_next_line(fd2, &buff);
	   printf("%s\n\n", buff);
	   free(buff);
	   get_next_line(fd, &buff);
	   printf("%s\n\n", buff);
	   free(buff);
	   get_next_line(fd3, &buff);
	   printf("%s\n\n", buff);
	   free(buff);
	   */
	close(fd);
	close(fd2);
	close(fd3);
	return (0);
}
