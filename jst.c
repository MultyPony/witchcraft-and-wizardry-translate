#include <stdio.h>
#include "parson.h"


int	main(int argc, char **argv)
{
	JSON_Value *root_value;
	JSON_Array *j_array;
	JSON_Object *j_obj;
	const char *str;

	printf("argc = %d\n", argc);
	root_value = json_parse_string(argv[1]);
	j_obj = json_value_get_object(root_value);
	j_array = json_object_get_array(j_obj, "text");
	str = json_array_get_string(j_array, 0);
	printf("%s\n", str);
	json_value_free(root_value);
	return (0);
}
