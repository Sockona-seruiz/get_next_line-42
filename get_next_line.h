/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.h                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/28 12:38:08 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 10:29:10 by seruiz      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

typedef	struct		s_fd_list
{
	char			*buff;
}					t_fd_list;
int					get_next_line(int fd, char **line);
char				*ft_strjoin(char *s1, char *s2, t_fd_list *lst);
int					ft_setfd_buff(int i, char *reader,
					char *buff, t_fd_list *lst);
int					compute_buff(char *reader, char **line, t_fd_list *lst);
int					treat_buff(int fd, char **line, t_fd_list *lst);
int					ft_free(char *s1, char *s2, t_fd_list *lst);
int					ft_eof(t_fd_list *lst);
size_t				ft_strlen(char *s);
#endif
