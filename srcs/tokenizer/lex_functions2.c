#include "lexer.h"

t_error	f_less(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	if (ft_isalnum(cursor) || ft_isspace(cursor) || cursor == '\0' || cursor == '"'
			|| cursor == '\'')
	{
		if (add_token_to_list(token_list, LESS, "") == FAIL)
			return (FAIL);
		if (ft_isalnum(cursor))
			append_buffer(buffer, cursor); // need to save the character
		set_machine_state(cursor, state);
	}
	else if (cursor == '<')
	{
		if (add_token_to_list(token_list, DLESS, "") == FAIL)
			return (FAIL);
		*state = ST_TRANSITION;
	}
	else if (cursor == '>')
		printf("syntax error\n");
	else
		printf("non covered case in f_less\n");
	return (SUCCESS);
}

t_error	f_great(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	if (ft_isalnum(cursor) || ft_isspace(cursor) || cursor == '\0' || cursor == '"'
			|| cursor == '\'')
	{
		if (add_token_to_list(token_list, GREAT, "") == FAIL)
			return (FAIL);
		if (ft_isalnum(cursor))
			append_buffer(buffer, cursor); // need to save the character
		set_machine_state(cursor, state);
	}
	else if (cursor == '>')
	{
		if (add_token_to_list(token_list, DGREAT, "") == FAIL)
			return (FAIL);
		*state = ST_TRANSITION;
	}
	else if (cursor == '<')
		printf("syntax error\n");
	else
		printf("non covered case in f_great\n");
	return (SUCCESS);
}
