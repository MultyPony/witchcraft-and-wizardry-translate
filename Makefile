NAME = mctranslate

all: ${NAME}

${NAME}:
	gcc -Wall -Wextra -Werror *.c -o ${NAME}
