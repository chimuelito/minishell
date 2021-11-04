#include "signals.h"

void	setup_terminal()
{
	struct termios term_buff;

	if (tcgetattr(STDIN_FILENO, &g_global.term_save) == -1)
		printf("error tcgetattr\n");
	term_buff = g_global.term_save;
	term_buff.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term_buff);
}

void	setup_main_signals(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
}

void	handler(int signal)
{
	if ((signal == SIGQUIT || signal == SIGINT) && g_global.pid != 0)
		process(signal);
	else
	{
		if (signal == SIGINT)
		{
			ft_putchar_fd('\n', STDIN_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		if (signal == SIGQUIT)
		{
			ft_putchar_fd('\r', 1);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	process(int signal)
{
	if (kill(g_global.pid, signal) == 0)
	{
		if (signal == SIGINT)
			ft_putchar_fd('\n', 1);
		if (signal == SIGQUIT && g_global.heredoc == FALSE)
			ft_putstr_fd("Quit: 3\n", 1);
		if (open("heredoc.txt", O_RDONLY, 0644) >= 0)
			unlink("heredoc.txt");
	}
	else if (signal == SIGINT)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_redisplay();
	}
}
