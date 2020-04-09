#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mh.h"
#define SIZE 50468 
#define FIND_START "\"text\\\":\\\""
#define FIND_END "\""

int	get_file_size(int fd)
{
	off_t ls;
	
	ls = lseek(fd, 0, SEEK_END);
	printf("ls2 = %d\n", (int)ls);	
	lseek(fd, 0, SEEK_SET);
	return ((int)ls);
}

char	*findnextstr(char **buff)
{
	char *strtotrans;
	char *startstr;
	char *endstr;
	int len;

	startstr = ft_strstr(*buff, FIND_START);
	endstr = ft_strstr(startstr, FIND_END);
	*buff = endstr;
	len = endstr - startstr - 2;
	strtotrans = (char *)malloc(len * sizeof(char));
	if (strtotrans == NULL)
		return (NULL);
	ft_strncpy(strtotrans, startstr, len);
	return (strtotrans);
}

int	edit_file(const char *filename)
{
	int	fd;
	int	fs;
	int	strc;
	char	*buff;
	char	*strtotrans;
	char	*curbuf;

	fd = open(filename, O_RDWR);
	if (fd == -1)
	{
		printf("Error: Can not open file!\n");
		return (1);
	}
	fs = get_file_size(fd); 
	printf("fs = %d\n", fs);
	buff = (char *)malloc(fs * sizeof(char) + 1);
	if (buff == NULL)
	{
		printf("Error: malloc\n");
		return (2);
	}
	buff[read(fd, buff, fs)] = '\0';
	/* Search for needle in buff */
	strc = 0;
	curbuf = buff;
	while (*curbuf)
	{
		strtotrans = findnextstr(&curbuf);
		printf("THIS IS STR: %s\n", strtotrans);
		strc++;
	}
	/* Translate string through Yandex */
	
	/* Write translated string into buff */
	printf("\n\n\n\n\n\n\nEEEEEEEEEEEEEEEENNNNNNNNNNNNNNNNNNNNNDDDDDDDDDDDDDDDDDD\n");
	printf("GREP %d\n", strc);
	if (close(fd) == -1)
		printf("Close error\n");
	return (0);
}


int	main(void)
{
	/* 1. File searching */ 
	
	/* 2. File editing */
	edit_file("hermione.mcfunction");	
	printf("End of Program\n");
	/* End of 2. File editing */

	/* End of 1. File searching */
	return (0);
}
