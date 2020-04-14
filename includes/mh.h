#ifndef MH_H
# define MH_H

void	ft_putchar(char c);
void	ft_putstr(char *str);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *haystack, const char *needle, int len);
char	*ft_strncpy(char *dest, char *src, int n);
typedef	struct	s_tr_list t_trlist;
struct	s_tr_list
{
	int		ii;
	int		len;
	char		*str_to_trans;
	char		*translated_str;
	t_trlist	*next;
};

#endif
