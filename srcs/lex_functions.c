#include "lexer.h"

t_error	f_transition(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	(void)token_list;
	
	if (ft_isalnum(cursor))
	{
		append_buffer(buffer, cursor);
		*state = ST_IN_WORD;
	}
	else if (cursor == '"')
	{
		*state = ST_OPEN_DQUOTE;
		buffer->n_dquote++;
	}
	else if (cursor == '\'')
	{
		*state = ST_OPEN_SQUOTE;
		buffer->n_squote++;
	}
	else if (cursor == '<')
		*state = ST_LESS;
	else if (cursor == '>')
		*state = ST_GREAT;
	else if (cursor == '|')
		if (add_token(token_list, PIPE) == FAIL)
			return (FAIL);
	return (SUCCESS);
}

t_error	f_inword(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	t_token *token;

	if (ft_isalnum(cursor))
		append_buffer(buffer, cursor);
	else if (ft_isspace(cursor) || cursor == '\0' || cursor == '<' || cursor == '>' || cursor == '|')
	{
		append_buffer(buffer, '\0');
		token = new_token(WORD, buffer->buf);
		if (!token)
			return (FAIL);
		ft_lstadd_back(token_list, ft_lstnew(token));
		initialize_buffer(buffer);// si on est a la fin de la ligne, il faut free le buffer.buf
		if (cursor == '|')
			if (add_token(token_list, PIPE) == FAIL)
				return (FAIL);
	}
	else if (cursor == '"')
		buffer->n_dquote++;
	else if (cursor == '\'')
		buffer->n_squote++;
	set_machine_state(cursor, state);
	return (SUCCESS);
}

void	set_machine_state(char cursor, enum e_machine_states *state)
{
	if (ft_isspace(cursor))
		*state = ST_TRANSITION;
	else if (ft_isalnum(cursor))
		*state = ST_IN_WORD;
	else if (cursor == '<')
		*state = ST_LESS;
	else if (cursor == '>')
		*state = ST_GREAT;
	else if (cursor == '"')
		*state = ST_OPEN_DQUOTE;
	else if (cursor == '\'')
		*state = ST_OPEN_SQUOTE;
	else if (cursor == '|')
		*state = ST_TRANSITION;
}

t_error	f_doublequote(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	(void)token_list;

	if (ft_isalnum(cursor) || ft_isspace(cursor) ||\
			ft_is_incharset(cursor, "<>.,?/\\()"))
		append_buffer(buffer, cursor);
	else if (cursor == '"') 
	{
		buffer->n_dquote--; // puisqu'on est dans ST_DQUOTE, on passe forcement a n_dquote == 0
		if (buffer->n_squote == 0)
			*state = ST_IN_WORD;
		else
			*state = ST_OPEN_SQUOTE;
	}
	else if (cursor == '\'') // la c'est le cas plus delicat a gerer
	{
		buffer->n_squote++;
		*state = ST_OPEN_SQUOTE;
	}
	else if (cursor == '\0')
		printf("Error: unclosed double quote\n");
	else
		printf("non covered case in f_doublequote\n");
	return (SUCCESS);
}

t_error	f_singlequote(char cursor, enum e_machine_states *state, t_list **token_list, t_buffer *buffer)
{
	(void)token_list;

	if (cursor == '\'') 
	{
		buffer->n_squote--;
		if (buffer->n_dquote == 0)
			*state = ST_IN_WORD;
		else
			*state = ST_OPEN_DQUOTE;
	}
	else if (cursor == '"')
	{
		buffer->n_dquote++;
		*state = ST_OPEN_DQUOTE;
	}
	else if (cursor == '\0')
		printf("Error: unclosed single quote\n");
	else if (ft_isascii(cursor))
		append_buffer(buffer, cursor);
	else
		printf("non covered case in f_singlequote\n");
	return (SUCCESS);
}