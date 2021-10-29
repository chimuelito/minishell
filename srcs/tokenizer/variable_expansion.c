#include "lexer.h"
#include "execution.h"

t_error	f_var_substitution(char cursor, enum e_machine_states *state,
		t_list **token_list, t_buffer *buffer)
{
	char			*str;
	static t_buffer	var_buf = {NULL, 0, 0};

	if (cursor == '?')
	{
		str = ft_itoa(g_global.ret);
		tokenize_variable(str, buffer, token_list);
		free(str);
		*state = ST_IN_WORD;
	}
	else if (ft_isalnum(cursor) || cursor == '_')
		append_buffer(&var_buf, cursor);
	else
	{
		if (var_buf.pos == 0 && !ft_is_incharset(cursor, "\"\'"))
			tokenize_variable("$", buffer, token_list);
		else
			if (process_expanded_var(&var_buf, buffer, token_list) == FAIL)
				return (FAIL);
		if (ft_isspace(cursor) || cursor == '\0')
			push_buf_to_toklist(buffer, token_list, WORD);
		set_machine_state(cursor, state);
	}
	return (SUCCESS);
}

t_error	process_expanded_var(t_buffer *var_buf, t_buffer *buffer,
		t_list **token_list)
{
	char	*str;

	append_buffer(var_buf, '\0');
	str = my_getenv(var_buf->buf);
	if (tokenize_variable(str, buffer, token_list) == FAIL)
		return (FAIL);
	free(str);
	free(var_buf->buf);
	initialize_buffer(var_buf);
	return (SUCCESS);
}

t_error	tokenize_variable(char *expanded_var, t_buffer *buffer,
		t_list **token_list)
{
	char	*str;

	if (expanded_var == NULL)
		return (SUCCESS);
	str = expanded_var;
	while (*str)
	{
		if (ft_isspace(*str))
		{
			if (buffer->pos > 0)
				push_buf_to_toklist(buffer, token_list, WORD);
		}
		else
			append_buffer(buffer, *str);
		str++;
	}
	return (SUCCESS);
}

t_error	f_var_substit_dquote(char cursor, enum e_machine_states *state,
		t_list **token_list, t_buffer *buffer)
{
	char		*str;
	static t_buffer var_buf = {NULL, 0, 0};
	(void)token_list;

	if (cursor == '?')
	{
		str = ft_itoa(g_global.ret);
		append_str_to_buffer(buffer, str);	
		free(str);
		*state = ST_OPEN_DQUOTE;
	}
	else if (ft_isalnum(cursor) || cursor == '_')
		append_buffer(&var_buf, cursor);
	else
	{
		append_buffer(&var_buf, '\0');
		str = my_getenv(var_buf.buf);
		append_str_to_buffer(buffer, str);
		if (cursor == '\"')
			*state = ST_IN_WORD;
		else
			*state = ST_OPEN_DQUOTE;
	}
	return (SUCCESS);
}

char	*my_getenv(char	*var)
{
	int		i;
	char	*variable;
	char	*ret_str;

	i = find_key_index(g_global.envp, var);
	if (i == -1)
		return (NULL);
	variable = ft_strchr(g_global.envp[i], '=');
	ret_str = ft_strdup(variable + 1);
	return (ret_str);
}
