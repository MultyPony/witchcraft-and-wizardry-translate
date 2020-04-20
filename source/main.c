#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <dirent.h>
#include "mh.h"
#define FIND_START "\"text\\\":\\\""
#define FIND_NEWLINE "\\\\n"
#define FIND_END "\""

typedef struct	s_lent
{
	struct dirent *ent;
	struct lent *next;
}		t_lent;

int	isdir(char *n)
{
	struct stat st;
	stat(n, &st);
	if (S_ISDIR(st.st_mode))
		return(1);
	return (0);
}

typedef struct	s_list	t_list;

struct	s_list
{
	char *dirn;
	t_list *next;
};

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
/*
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
	elem->len = endstr - startstr - 2;
	endstr = ft_strnstr(startstr, FIND_NEWLINE, elem->len);
	if (NULL != endstr)
		elem->len = endstr - startstr - 3;
//	if (' ' == buff[elem->ii + elem->len - 1])
//		elem->len -= 1;
	elem->str_to_trans = (char *)malloc(elem->len * sizeof(char));
	if (NULL == elem->str_to_trans)
		return (NULL);
	ft_strncpy(elem->str_to_trans, startstr, elem->len + 1);
	return (elem);
}
*/
t_trlist	*getlistelem(char *buff, int shift)
{
	t_trlist	*elem;
	char		*startstr;
	char		*endstr;
	char 		*shifted_buff;
	int		len;
	int		blen;

	blen = strlen(buff);
	shifted_buff = buff + shift;
	startstr = ft_strstr(shifted_buff, FIND_START);
	startstr = skip(startstr);
	if (NULL == startstr)
		return (NULL);
	endstr = ft_strstr(startstr, FIND_END);
	if (NULL == endstr)
		return (NULL);
	len = endstr - startstr - 2;
	endstr = ft_strnstr(startstr, FIND_NEWLINE, len);
	if (NULL != endstr)
		len = endstr - startstr - 3;
	if (len == 0 && shift < blen)
	{
		return (getlistelem(buff, shift + 1));
	}
	elem = (t_trlist *)malloc(sizeof(t_trlist));
	if (NULL == elem)
		return (NULL);
	elem->ii = startstr - buff;
	elem->len = len;
	elem->str_to_trans = (char *)malloc(elem->len * sizeof(char) + 1);
	if (NULL == elem->str_to_trans)
		return (NULL);
	ft_strncpy(elem->str_to_trans, startstr, elem->len + 1);
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


/* Insert string into buffer at def position */
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
	strncpy(newbuff + (ii + new_str_len), buff + (ii + old_str_len), taillen);
	newbuff[ii + new_str_len + taillen] = '\0';
	return (newbuff);
}

void	changeii(t_trlist *trl, int shift)
{
	while (NULL != trl)
	{
		trl->ii += shift;
		trl = trl->next;
	}
}

int	edit_file(const char *filename)
{
	int	fd;
	int	fs;
	char	*buff;
	t_trlist	*trl;
	t_trlist	*strl;

	fd = open(filename, O_RDWR);
	if (fd == -1)
	{
		printf("Error: Can not open file!\n");
		return (1);
	}
	fs = get_file_size(fd); 
	buff = (char *)malloc(fs * sizeof(char) + 1);
	if (buff == NULL)
	{
		printf("Error: malloc\n");
		return (2);
	}
	buff[read(fd, buff, fs)] = '\0';
	lseek(fd, 0, SEEK_SET);
	/* Search all needles in buff */
	trl = findall(buff);
	/* Translate string through Yandex */
	translate(trl);
//	display_list_all(trl);
	strl = trl;
	while (NULL != strl)
	{
		buff = insert_str(buff, strl->translated_str, strl->ii, strl->len);
		changeii(strl->next, strlen(strl->translated_str) - strl->len);
		strl = strl->next;
	}
	/* Write translated string into buff */
	free(trl);
	write(fd, buff, strlen(buff));
	if (close(fd) == -1)
		printf("Close error\n");
	return (0);
}

/* Search for a file and translate it */

void	search_translate(char *current_dir)
{
	DIR		*dir;
	struct dirent	*dt;
	char		*full_path;
	t_list		*tl;
	t_list		*stl;

	dir = opendir(current_dir);
	if (dir)
	{
		tl = NULL;
		stl = NULL;
		while ((dt = readdir(dir)) != NULL)
		{
			if (strcmp(dt->d_name, ".") != 0 
				&& strcmp(dt->d_name, "..") != 0
				&& strcmp(dt->d_name, ".git") != 0)
			{
				full_path = (char *)malloc(sizeof(char) * (strlen(current_dir) + strlen(dt->d_name)  + 2));
				strcpy(full_path, current_dir);
				strcat(full_path, "/");
				strcat(full_path, dt->d_name);
				if (isdir(full_path)) 
				{		
					//add folder name to list
					if (NULL == stl)
					{
						tl = (t_list *)malloc(sizeof(t_list));	
						if (NULL == tl)
							return ;
						stl = tl;

					}
					else
					{
						tl->next = (t_list *)malloc(sizeof(t_list));
						if (NULL == tl->next)
							return ;
						tl = tl->next;
					}
					tl->dirn = full_path;
					tl->next = NULL;
				}
				else
					edit_file(full_path);
			}
		}
		while (stl && stl->dirn)
		{
			search_translate(stl->dirn);
			free(stl->dirn);
			stl = stl->next;
		}
		free(tl);
	}
}

int	main(void)
{
	curl_global_init(CURL_GLOBAL_ALL);	
	search_translate(".");
	printf("End of Program\n");
	curl_global_cleanup();
	return (0);
}
