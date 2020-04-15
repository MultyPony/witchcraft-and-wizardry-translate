#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "mh.h"
#define SIZE 50468
#define FIND_START "\"text\\\":\\\""
#define FIND_NEWLINE "\\\\n"
#define FIND_END "\""

int	get_file_size(int fd)
{
	off_t ls;

	ls = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	return ((int)ls);
}

char	*skip(char *str)
{
	if (NULL == str)
		return (NULL);
	while (*str && !(*str == '\\' && *(str + 1) == '\"') && !((*str >= '0' && *str <= '9') 
				|| (*str >= 'a' && *str <= 'z') 
				|| (*str >= 'A' && *str <= 'Z')))
	{
		if (*str == '\\' && *(str + 1) == 'n')
			str++;
		str++;
	}
	return (str);
}

t_trlist	*getlistelem(char *buff, int shift)
{
	t_trlist	*elem;
	char		*startstr;
	char		*endstr;
	char 		*shifted_buff;

	shifted_buff = buff + shift;
	startstr = ft_strstr(shifted_buff, FIND_START);
	startstr = skip(startstr);
	if (NULL == startstr)
		return (NULL);
	elem = (t_trlist *)malloc(sizeof(t_trlist));
	if (NULL == elem)
		return (NULL);
	elem->ii = startstr - buff;
	endstr = ft_strstr(startstr, FIND_END);
	if (NULL == endstr)
		return (NULL);
	elem->len = endstr - startstr - 1;
	endstr = ft_strnstr(startstr, FIND_NEWLINE, elem->len);
	if (NULL != endstr)
		elem->len = endstr - startstr - 2;
	elem->str_to_trans = (char *)malloc(elem->len * sizeof(char));
	if (NULL == elem->str_to_trans)
		return (NULL);
	ft_strncpy(elem->str_to_trans, startstr, elem->len);
	return (elem);
}

t_trlist	*findall(char *buff)
{
	t_trlist	*trl;
	t_trlist	*strl;
	
	trl = getlistelem(buff, 0);
	strl = trl;
	while (strl != NULL)
	{
		strl->next = getlistelem(buff, strl->ii + strl->len);
		strl = strl->next;
	}
	return (trl);
}

void	display_list(t_trlist *trl)
{
	while (trl != NULL)
	{
		printf("String to translate:\t""\x1b[36m""%s\n""\x1b[0m", trl->str_to_trans);
		trl = trl->next;
	}
}

void	display_list_tr(t_trlist *trl)
{
	while (trl != NULL)
	{
		if (strlen(trl->str_to_trans) > 0)
			printf("Translated string:\t""\x1b[36m""%s\n""\x1b[0m", trl->translated_str);
		trl = trl->next;
	}
}

void	display_list_all(t_trlist *trl)
{
	while (trl != NULL)
	{
		if (strlen(trl->str_to_trans) > 0)
		{
			printf("\nOriginal:\t""\x1b[31m""%s\n""\x1b[0m", trl->str_to_trans);
			printf("Translated:\t""\x1b[93m""%s\n""\x1b[0m", trl->translated_str);
		}
		trl = trl->next;
	}
}

char	*insert_str(char *buff, char *str_to_insert, int ii, int old_str_len)
{
	char	*newbuff;
	int	taillen;
	int	new_str_len;

	taillen = strlen(buff) - ii - old_str_len;
	new_str_len = strlen(str_to_insert);
	newbuff = (char *)malloc(sizeof(char) * (ii + new_str_len + taillen) + 1);
	if (NULL == newbuff)
		return (NULL);
	strncpy(newbuff, buff, ii);
	strncpy(newbuff + ii, str_to_insert, new_str_len);
	strncpy(newbuff + (ii + new_str_len), buff + (ii + old_str_len - 1), taillen);
	newbuff[ii + new_str_len + taillen] = '\0';
	return (newbuff);
}

int	edit_file(const char *filename)
{
	int	fd;
	int	fs;
//	int	strc;
	char	*buff;
//	char	*strtotrans;
	t_trlist	*trl;

	fd = open(filename, O_RDWR);
	if (fd == -1)
	{
		printf("Error: Can not open file!\n");
		return (1);
	}
	fs = get_file_size(fd); 
//	printf("fs = %d\n", fs);
	buff = (char *)malloc(fs * sizeof(char) + 1);
	if (buff == NULL)
	{
		printf("Error: malloc\n");
		return (2);
	}
	buff[read(fd, buff, fs)] = '\0';
	printf("old buff: %s\n", buff);
	/* Search all needles in buff */
	trl = findall(buff);

//	display_list(trl);

	/* Translate string through Yandex */
//	translate(trl);

//	display_list_tr(trl);
//	display_list_all(trl);
	buff = insert_str(buff, "ТЕСТ!!!", trl->ii, trl->len);
//	buff = insert_str(buff, "ТЕСТ!!!", trl->next->ii, trl->next->len);
	printf("new buff: %s\n", buff);
	/* Write translated string into buff */
	printf("\n\n\n\n\n\n\nEEEEEEEEEEEEEEEENNNNNNNNNNNNNNNNNNNNNDDDDDDDDDDDDDDDDDD\n");
	if (close(fd) == -1)
		printf("Close error\n");
	return (0);
}


int	main(void)
{
	/* Init part */
	curl_global_init(CURL_GLOBAL_ALL);	
	/* 1. File searching */ 
	
	/* 2. File editing */
	edit_file("temp/hermione.mcfunction");	
	printf("End of Program\n");
	/* End of 2. File editing */

	/* End of 1. File searching */

	curl_global_cleanup();
	return (0);
}
