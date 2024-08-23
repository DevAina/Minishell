NAME = minishell

FLAGS = -Wall -Werror -Wextra -L./libft -lft -lreadline -g

SRCS = ./built_in/cd.c ./built_in/export.c ./src/mns_utils.c ./src/main.c \
		./src/tokenizer.c ./src/parse_input.c ./src/exec.c ./src/exec_utils.c \
		./src/expander.c ./src/analyze_tokens.c

OBJS = ${SRCS:.c=.o}

all : ${NAME}

.c.o :
	cc -c -Wall -Werror -Wextra -g $< -o ${<:.c=.o}

${NAME} : ${OBJS}
	make bonus -C ./libft
	cc -o ${NAME} ${OBJS} ${FLAGS}

clean :
	make clean -C ./libft
	rm -rf ${OBJS}

fclean : clean
	make fclean -C ./libft
	rm -rf ${NAME}

re: fclean all

.PHONY: re fclean clean all
