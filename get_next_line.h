/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.h                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/21 09:10:06 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 11:52:54 by seruiz      ###    #+. /#+    ###.fr     */
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

typedef struct			s_fd_list
{
	int					fd;
	char				*buff;
	struct s_fd_list	*next;
}						t_fd_list;
void					ft_setfd_buff(char *reminder, t_fd_list *current_fd,
						t_fd_list *first_fd, char *line);
int						compute_buff(char *buff, t_fd_list *current_fd,
						char **line, t_fd_list *first_fd);
int						treat_buff(char *str, t_fd_list *current_fd,
						char **line,
						t_fd_list *first_fd);
t_fd_list				*search_fd(int fd, t_fd_list *first_fd);
int						get_next_line(int fd, char **line);
size_t					ft_strlen(const char *s);
t_fd_list				*ft_lstnewfd(int fd, t_fd_list *first_fd);
void					ft_free(t_fd_list *first, char *line, char *buff);
char					*ft_strjoin(char *s1, char *s2, t_fd_list *first_fd);
int						ft_eof(char *line, t_fd_list *first_fd,
						t_fd_list *current_fd, int ret);
#endif
