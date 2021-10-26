#include "lexer.h"

t_error	f_transition(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	if (ft_is_incharset(cursor, INVALID_WORD_CHAR))
		return (syntax_error(cursor));
	else if (ft_is_incharset(cursor, "<>\"\'$") || ft_isspace(cursor))
		; // do nothing, only update machine state
	else if (cursor == '|')
	{
		if (add_token_to_list(token_list, PIPE, "") == FAIL)
			return (FAIL);
	}
	else if (ft_isascii(cursor))
		append_buffer(buffer, cursor);
	set_machine_state(cursor, state);
	return (SUCCESS);
}

t_error	f_inword(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	if (ft_is_incharset(cursor, INVALID_WORD_CHAR))
		return (syntax_error(cursor));
	else if (ft_isspace(cursor) || ft_is_incharset(cursor, "<>|") || cursor == '\0')
	{
		if (push_buf_to_toklist(buffer, token_list, WORD) == FAIL)
			return (FAIL);
		if (cursor == '|')
			if (add_token_to_list(token_list, PIPE, "") == FAIL)
				return (FAIL);
//		if (cursor == '"' || cursor == '\'')
//			link_last_token(*token_list);
	}
	else if (ft_isascii(cursor) && !ft_is_incharset(cursor, "\"\'$"))
		append_buffer(buffer, cursor);
	set_machine_state(cursor, state);
	return (SUCCESS);
}

t_error	f_doublequote(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	(void)token_list;

	if (cursor == '"') 
	{
		*state = ST_IN_WORD;
//		if (push_buf_to_toklist(buffer, token_list, WORD) == FAIL)
//			return (FAIL);
	}
	else if (cursor == '\0')
	{
		printf("Error: unclosed double quote\n");
		return (FAIL);
	}
	else if (ft_isascii(cursor))
		append_buffer(buffer, cursor);
	else
		printf("non covered case in f_doublequote\n");
	return (SUCCESS);
}

t_error	f_singlequote(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	(void)token_list;

	if (cursor == '\'') 
	{
		*state = ST_TRANSITION;
//		if (push_buf_to_toklist(buffer, token_list, WORD_NOEXPAND) == FAIL)
//			return (FAIL);
	}
	else if (cursor == '\0')
	{
		printf("Error: unclosed single quote\n");
		return (FAIL);
	}
	else if (ft_isascii(cursor))
		append_buffer(buffer, cursor);
	else
		printf("non covered case in f_singlequote\n");
	return (SUCCESS);
}

t_error	f_var_substitution(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{

	(void)state;
	(void)token_list;
	(void)buffer;
	static t_buffer	var_buf = {NULL, 0, 0};

	if (ft_isspace(cursor) || cursor == '\0')
	{
		if (var_buf.buf == NULL)
			append_buffer(&var_buf, '$');
		append_buffer(&var_buf, '\0');
		printf("var_buf: %s\n", var_buf.buf);
		
		free(var_buf.buf);
		initialize_buffer(&var_buf);
//		push_buf_to_toklist(buffer, token_list, WORD);
	}
	else if (ft_is_incharset("\"\'")

	else if (ft_isalnum(cursor) || cursor == '_')
		append_buffer(&var_buf, cursor);
	
	return (SUCCESS);
}

/*    USELESS FUNCTION?? I THINK SO !
 
t_error	f_word_transition(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	if (ft_is_incharset(cursor, INVALID_WORD_CHAR))
		return (syntax_error(cursor));
	else if (cursor == '|')
	{
		if (add_token_to_list(token_list, PIPE, "") == FAIL)
			return (FAIL);
	}
	else if (cursor == '"' || cursor == '\'')
		link_last_token(*token_list);
	else if (cursor == '\0' || ft_isspace(cursor))
		;
	else // all other regular characters
	{
		link_last_token(*token_list);
		append_buffer(buffer, cursor);
	}
	set_machine_state(cursor, state);
	return (SUCCESS);
}
*/
