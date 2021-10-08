#include "minishell.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	char	*line;
	t_list	*token_list;
	t_command	**cmd_array;

	while (1)
	{
		line = readline("minishell ===> ");
		token_list = tokenizer(line);
		print_token_list(token_list);
		cmd_array = parser(token_list);
		print_command_array(cmd_array, g_global.num_cmds);
		ft_lstclear(&token_list, free_token);
		free(line);
	}
	return (0);
}
