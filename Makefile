NAME = mctranslate
HLPR = yat
SOURCES = source/main.c source/strhelp.c

all: ${NAME}

${NAME}:
	gcc -ggdb -Iincludes -Wall -Wextra -Werror ${SOURCES} -o ${NAME}

${HLPR}:
	gcc -ggdb -lcurl -Iincludes -Wall -Wextra -Werror source/parson.c tya.c -o ${HLPR}
fclean:
	rm -r -f ${NAME}

re: fclean all
