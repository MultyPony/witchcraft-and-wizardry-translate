#include <unistd.h>
#include <stddef.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	while (*str)
	{
		ft_putchar(*str);
		str++;
	}
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*n;

	while (*haystack)
	{
		n = needle;
		if ((*haystack)  == (*n))
		{
			while (*haystack && *n && (*haystack) == (*n))
			{
				haystack++;
				n++;
			}
			if (*n == '\0')
			{
				ft_putstr("Found!!!\n");
				return ((char *)haystack);
			}
		}
		haystack++;
	}
	return (NULL);
}

char	*ft_strncpy(char *dest, char *src, int n)
{
	int i;
	
	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
		dest[i++] = '\0';
	return (dest);
}
