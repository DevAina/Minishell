NAME = minishell

FLAGS = -Wall -Werror -Wextra -L./libft -lft -lreadline -g

BUILT_IN = ./built_in/cd.c ./built_in/export.c ./built_in/pwd.c ./built_in/echo.c \
		./built_in/env.c ./built_in/unset.c ./built_in/exit.c ./built_in/export_utils.c

LEXER = ./src/lexer/tokenizer.c ./src/lexer/token_creation.c ./src/lexer/token_utils.c \
		./src/lexer/fd_handling.c

CHECK_TOKENS = ./src/check_tokens/handle_heredoc.c ./src/check_tokens/utils.c ./src/check_tokens/analyze_tokens.c

EXPANDER = ./src/expander/expander.c ./src/expander/expand_helpers.c ./src/expander/expander_utils.c

PARSER = ./src/parser/free_ast.c ./src/parser/init_utils.c ./src/parser/parse_utils.c ./src/parser/parse.c \
			./src/parser/redirection_utils.c

UTILS = ./src/utils/mns_utils.c ./src/utils/env_utils.c

EXEC = ./src/exec/exec.c ./src/exec/exec_redir.c ./src/exec/exec_utils.c ./src/exec/exec_utils1.c

SRCS =  ${LEXER} ${CHECK_TOKENS} ${EXPANDER} ${PARSER} ${UTILS} ${BUILT_IN}  ${EXEC}\
		./src/main.c

OBJS = ${SRCS:.c=.o}


# Couleurs
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
RESET = \033[0m

all : ${NAME}

.c.o :
	@echo "${YELLOW}Compiling $<...${RESET}"
	@cc -c -Wall -Werror -Wextra -g $< -o ${<:.c=.o}

${NAME} : ${OBJS}
	@make --no-print-directory bonus -C ./libft
	@cc -o ${NAME} ${OBJS} ${FLAGS}
	@echo "${CYAN}${NAME} has been successfully built!${RESET}"

clean :
	@echo "${RED}Cleaning object files...${RESET}"
	@make --no-print-directory clean -C ./libft
	@rm -rf ${OBJS}

fclean : clean
	@echo "${RED}Performing full clean...${RESET}"
	@make --no-print-directory fclean -C ./libft
	@rm -rf .history_file
	@rm -rf ${NAME}

re: fclean all

.PHONY: re fclean clean all
