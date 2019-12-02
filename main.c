/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: seruiz <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/21 12:32:34 by seruiz       #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 11:57:23 by seruiz      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "get_next_line.h"

int	main(void)
{
	int fd[8];
	int ret;
	char *buff;
	int i;
	int j;

	i = 7;
	j = 0;
	printf("----------------------------------------------------------------------------------------\n");
	fd[0] = open("test/64bit_line.txt", O_RDONLY);
	fd[1] = open("test/64bit_paragraph.txt", O_RDONLY);
	fd[2] = open("test/bar.txt", O_RDONLY);
	fd[3] = open("test/empty.txt", O_RDONLY);
	fd[4] = open("test/fewchar_perline.txt", O_RDONLY);
	fd[5] = open("test/lomg_line.txt", O_RDONLY);
	fd[6] = open("test/normal.txt", O_RDONLY);
	fd[7] = open("test/only_newline.txt", O_RDONLY);

	//while (i <= 7)
	//{
	while((ret = get_next_line(fd[4], &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
		if (ret == 0 || j == 3)
			break;
		j++;
	}
	if ((ret = get_next_line(fd[4], &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
	}
	//i++;
	//}
	while((ret = get_next_line(fd[6], &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
		if (ret == 0)
			break;
	}
	if ((ret = get_next_line(fd[6], &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
	}
	while((ret = get_next_line(fd[4], &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
		if (ret == 0)
			break ;
	}
	if ((ret = get_next_line(fd[4], &buff)) >= 0)
	{
		printf("resut : %s\n ret : %d\n\n", buff, ret);
		free(buff);
	}
	i = 0;
	while (i <= 7)
	{
		close(fd[i]);
		i++;
	}
	return (0);
}
