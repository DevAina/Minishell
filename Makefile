NAME = minishell

FLAGS = -Wall -Werror -Wextra -L./libft -lft -lreadline -g

SRCS = ./built_in/cd.c ./built_in/export.c ./src/mns_utils.c ./src/main.c \
		./src/tokenizer.c ./src/parse_input.c ./src/exec.c ./src/exec_utils.c \
<<<<<<< HEAD
		./src/expander.c
=======
		./src/expander.c 
>>>>>>> ef128f7 (blabla)

OBJS = ${SRCS:.c=.o}

all : ${NAME}

.c.o :
	cc -c $< -o ${<:.c=.o} ${FLAGS}

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
