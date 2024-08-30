NAME = minishell

FLAGS = -Wall -Werror -Wextra -L./libft -lft -lreadline -g

SRCS = ./built_in/cd.c ./built_in/export.c ./src/mns_utils.c ./src/main.c \
		./src/tokenizer.c ./src/parse_input.c ./src/exec.c ./src/exec_utils.c \
		./src/expander.c ./src/analyze_tokens.c ./built_in/pwd.c ./built_in/echo.c \
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		./built_in/env.c ./built_in/unset.c ./src/handler_sigint.c
=======
		./built_in/env.c ./src/handler_sigint.c
>>>>>>> 6c1e4c2 (Resolve conflict)
=======
		./built_in/env.c
>>>>>>> 7f7acfb (Resolve conflict)
=======
		./built_in/env.c ./built_in/unset.c ./src/handler_sigint.c
>>>>>>> 4d843aa (Add: gandler_sigint.c)

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
