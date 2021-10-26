# detecting OS to compile on Linux
UNAME = $(shell uname)

SRCS_FOLDER = srcs
OBJS_FOLDER = bin

SRCS = 	tokenizer/lexer.c \
		tokenizer/lex_functions.c \
		tokenizer/lex_functions2.c \
		tokenizer/token_utils.c \
		tokenizer/link_tokens.c \
		tokenizer/variable_expansion.c \
		tokenizer/debug.c \
		parser/parser.c \
		parser/debug.c \
		parser/array_utils.c \
		parser/parse_funcs.c \
		signal/signals.c \
		execution/free.c \
		execution/exec.c \
		execution/debug.c \
		execution/copy_env.c \
		execution/cmd_path.c \
		execution/replace_pwd.c \
		execution/plugs/heredocs.c \
		execution/plugs/wait_pid.c \
		execution/plugs/inputs_manip.c \
		execution/plugs/outputs_manip.c \
		execution/builtins/builtin1.c \
		execution/builtins/builtin2.c \
		execution/builtins/echo_utils.c \
		execution/builtins/builtin_utils.c \

OBJS = $(addprefix $(OBJS_FOLDER)/, $(SRCS:.c=.o))

NAME = minishell
INCLUDES = includes

HEADERS_LIST= $(addprefix $(INCLUDES)/, $(SRCS_LIST:.c=.h))
# La liste des .h marche comme ca, mais c'est pas opti. Faudrait que seul le .o correspondant
# au .h modifie soit recompile.

RM = rm -f
CC = clang
CFLAGS = -Werror -Wall -Wextra -I$(INCLUDES) -I$(LIBFT)/includes -g3 #-fsanitize=address
LIBFT	= libft
#readline stuffs:
LDFLAGS="-L$(HOME)/.brew/opt/readline/lib"
CPPFLAGS="-I$(HOME)/.brew/opt/readline/include"

$(OBJS_FOLDER)/%.o: $(SRCS_FOLDER)/%.c $(INCLUDES)/minishell.h $(HEADERS_LIST) $(LIBFT)/$(LIBFT).a
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: libft_ $(NAME) 

ifeq ($(UNAME), Linux)
$(NAME) : $(OBJS) $(INCLUDES) $(SRCS_FOLDER)/main.c | libft_
	$(CC) $(CFLAGS) -lreadline $(LDFLAGS) $(CPPFLAGS) -L$(LIBFT)  $(OBJS) $(SRCS_FOLDER)/main.c -o $(NAME) -lft
else
$(NAME) : $(OBJS) $(INCLUDES) $(SRCS_FOLDER)/main.c | libft_
	$(CC) $(CFLAGS) -lreadline $(LDFLAGS) $(CPPFLAGS) -L$(LIBFT) -lft $(OBJS) $(SRCS_FOLDER)/main.c -o $(NAME)
endif

libft_:
	$(MAKE) -C $(LIBFT)

clean:
	$(MAKE) clean -C $(LIBFT)
	$(RM) -r $(OBJS_FOLDER)
	$(RM) -r $(NAME).dSYM

fclean: clean
	$(MAKE) fclean -C $(LIBFT)
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all
