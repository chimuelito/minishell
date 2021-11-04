#include "execution.h"
#include "minishell.h"
#include "parser.h"

int	allpipes_action(int pipesfd[][2], int nb_pipes, t_pipes action)
{
	int	i;

	i = -1;
	if (action == DESTROY)
	{
		while (++i < nb_pipes)
		{
			if (close(pipesfd[i][READ]) == -1)
				return (FAIL);
			if (close(pipesfd[i][WRITE]) == -1)
				return (FAIL);
		}
	}
	else
	{
		while (++i < nb_pipes)
		{
			if (pipe(pipesfd[i]) == -1)
				return (FAIL);
		}
	}
	return (SUCCESS);
}

int	build_exec(t_command *cmd, char **env)
{
	struct stat	stat_ret;

	stat(cmd->executable, &stat_ret);
	ft_pushfront_array((void ***)&cmd->args, cmd->executable, cmd->number_args);
	cmd->number_args++;
	ft_pushback_array((void ***)&cmd->args, NULL, cmd->number_args);
	if (open(cmd->executable, O_RDONLY) == -1)
		cmd->executable = create_command_path(env, cmd->executable);
	if (open(cmd->executable, O_DIRECTORY) >= 0)
		cmd->executable = NULL;
	if (cmd->executable == NULL)
	{
		if (cmd->input_redir != NULL)
			exit (SUCCESS);
		g_global.ret = 127;
		ft_putstr_fd("\x1B[31mCommand not found\n\033[0m", 2);
		exit(g_global.ret);
	}
	return (SUCCESS);
}

t_error	connections(int i, t_command *cmd, int pipesfd[][2])
{
	if (connect_input_pipe(i, cmd->input_redir, pipesfd) == FAIL)
		return (FAIL);
	if (connect_output_pipe(i, cmd->output_redir, pipesfd) == FAIL)
		return (FAIL);
	if (input_redirection(cmd->input_redir) == FAIL)
		return (FAIL);
	if (output_redirection(cmd->output_redir) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

int	child_stuff(int i, t_command **commands, int nb_pipes, int pipesfd[][2])
{
	signal(SIGINT, SIG_DFL);
	if (is_heredoc(commands[i]->input_redir) == 1)
		signal(SIGQUIT, SIG_IGN);
	else
		signal(SIGQUIT, SIG_DFL);
	if (connections(i, commands[i], pipesfd) == FAIL)
	{
		g_global.ret = 1;
		exit (g_global.ret);
	}
	if (!is_builtin(commands[i]))
		build_exec(commands[i], g_global.envp);
	if (allpipes_action(pipesfd, nb_pipes, DESTROY) == FAIL)
		return (FAIL);
	if (is_builtin(commands[i]))
	{
		check_builtin(commands[i]);
		exit(1);
	}
	else
	{
		execve(commands[i]->executable, commands[i]->args, g_global.envp);
		exit(34);
	}
	return (SUCCESS);
}

t_error	execution(t_command **commands)
{
	int		nb_pipes = g_global.num_cmds - 1;
	int		pipesfd[nb_pipes][2];
	t_pid	*pids;
	pid_t	fork_res;
	int		i;

	pids = malloc(g_global.num_cmds * sizeof(t_pid));
	if (pids == NULL)
		return (FAIL);
	if (allpipes_action(pipesfd, nb_pipes, INITIALIZE) == FAIL)
		return (FAIL);
	if (g_global.num_cmds == 1 && !commands[0]->output_redir)
	{
		if (check_builtin(commands[0]) == 1)
		{
			free(pids);
			return (SUCCESS);
		}
	}
	i = -1;
	while (++i < g_global.num_cmds)
	{
		wait_previous_heredoc(commands[i]->input_redir, pids, i);
		if (is_heredoc(commands[i]->input_redir) == 1)
			g_global.heredoc = TRUE;
		fork_res = fork();
		g_global.pid = fork_res;
		tcsetattr(STDIN_FILENO, TCSANOW, &g_global.term_save);
		if (fork_res == CHILD)
			child_stuff(i, commands, nb_pipes, pipesfd);
		else if (fork_res > 0)
			fill_pids(fork_res, commands[i]->input_redir, pids, i);
		else
			return (FAIL);
	}
	if (allpipes_action(pipesfd, nb_pipes, DESTROY) == FAIL)
		return (FAIL);
	free(pids);
	wait_childs();
	return (SUCCESS);
}
