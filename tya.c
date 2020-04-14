#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "parson.h"
#define URL "https://translate.yandex.net/api/v1.5/tr.json/translate?key=trnsl.1.1.20200412T133057Z.0ed1826ac6e541ac.6ae792287bcc8caaa44d3a4c7211d9d853fe5641&lang=en-ru&text="

char	*json_to_str(char *json_str)
{
	JSON_Value	*root_value;
	JSON_Array	*j_array;
	JSON_Object	*j_obj;
	const char	*str;
	char		*res;
//	printf("argc = %d\n", argc);
	root_value = json_parse_string(json_str);
	j_obj = json_value_get_object(root_value);
	j_array = json_object_get_array(j_obj, "text");
	str = json_array_get_string(j_array, 0);
	res = (char *)malloc(strlen(str) + 1);
	strncpy(res, str, strlen(str) + 1);
//	printf("%s\n", str);
	json_value_free(root_value);
//	printf("%s\n", str);
	return (res);
}

size_t	write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	size_t rsize =  size * nmemb;
	char **ptr;
	char *str;
	ptr = userp;
	*ptr = (char *)malloc(rsize + 1);
	if (*ptr == NULL)
		return (0);
	str = json_to_str((char *)buffer);
	strncpy((char *)*ptr, str, strlen(str) + 1);
//	printf("buffer: %s\n", (char *)buffer);
//	printf("%s\n", (char *) *ptr);

	return (0);
}

char	*get_translate(char *str_to_translate)
{
	CURL *ch;
	char *translated_str;
	char req_str[1000] = URL;
	char *url_end;

	//translated_str = malloc(1);
	ch = curl_easy_init();
	url_end = curl_easy_escape(ch, str_to_translate, 0);
//	strcat(req_str, str_to_translate);
	strcat(req_str, url_end);
	curl_free(url_end);
	//ch = curl_easy_init();
	curl_easy_setopt(ch, CURLOPT_URL, req_str);
	curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)&translated_str);
	curl_easy_perform(ch);
	curl_easy_cleanup(ch);
	return (translated_str);
}

char	*getstr(char *str)
{	
	int c;

	
	while ((c = getchar()) != '\n')
	{
		if (c == EOF)
			return (NULL);
		*str = c;
		str++;
	}
	return str;
}

int	main(void)
{
	char *res;
	curl_global_init(CURL_GLOBAL_ALL);
	res = get_translate("If you find any interesting or rare books please bring them to me.\\\n\\\n I'll pay 50 ≈ for every rare book you bring me. \\\n\\\n");
	printf("%s\n", res);
	res = get_translate("Trade rare books)");
	printf("%s\n", res);
	curl_global_cleanup();
	return (0);
}

