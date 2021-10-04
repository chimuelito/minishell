#include "../includes/execution.h"

void	close_all_pipes(int pipesfd[][2], int pipesid_max)
{
	int j = -1;

	while (++j <= pipesid_max)
	{
		close(pipesfd[j][READ]);
		close(pipesfd[j][WRITE]);
	}
}

void	fill_commands(t_command *commands, char **cmds, char *args[10][10], char *files[][20])
{
	int i;

	i = 0;
	while (cmds[i])
	{
		commands[i].exec = cmds[i];
		commands[i].args = args[i];
		i++;
	}
	commands[0].input = NULL;
	commands[0].output = NULL;
	commands[1].input = NULL;
	commands[1].output = NULL;
	commands[2].input = files[0];
	commands[2].output = files[1];
	commands[3].input = NULL;
	commands[3].output = NULL;
	commands[4].input = files[2];
	commands[4].output = NULL;
	commands[5].input = NULL;
	commands[5].output = files[3];
	commands[6].input = NULL;
	commands[6].output = NULL;
	
}

void	heredoc(char *terminator, int fd)
{
	char *line;

	line = NULL;
	while (get_next_line(READ, &line) != 0 && ft_strncmp(line, terminator, ft_strlen(terminator)) != 0)
	{
		write(fd, line, ft_strlen(line));
		write (fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
}

int main(int argc, char **argv)
{
	(void)argc, (void)argv;
	//numeric datas and pipes
	int	nb_cmds = 7;
	int pipes_idmax = 5;
	int pipesfd[pipes_idmax + 1][2];
	int pipe_count;
	int j;
	//struct cmd
	char *cmds[] = {"/bin/ls", "/usr/bin/grep", "/bin/cat", "/usr/bin/rev", "/bin/cat", "/usr/bin/grep", "/usr/bin/wc", NULL};
	char *args[][10] = {{"ls", NULL}, {"grep", "file", NULL}, {"cat", NULL}, {"rev", NULL}, {"cat", "-e", NULL}, {"grep", "i", NULL}, {"wc", "-c", NULL}, {NULL}};
	t_command commands[10];
	char *files[][20] = {{"input1.txt", "input2.txt", "input3.txt", "heredoc", NULL}, {"output1.txt", NULL}, {"input4.txt", NULL}, {"output2.txt", "output3.txt", "output4.txt", NULL}, {NULL}};
	//create pipes
	int i;
	int fd;

	i = -1;
	while (++i <= pipes_idmax)
		pipe(pipesfd[i]);
	//forks and processes
	fill_commands(commands, cmds, args, files);
	i = -1;
	while (++i < nb_cmds)
	{
		pipe_count = i;
		if (fork() == 0)
		{
			if (i != 0 && commands[i].input == NULL)
				dup2(pipesfd[pipe_count - 1][READ] , STDIN_FILENO);
			if (i < nb_cmds - 1 && commands[i].output == NULL)
				dup2(pipesfd[pipe_count][WRITE], STDOUT_FILENO);
			if (commands[i].input != NULL)
			{
				j = -1;
				while (commands[i].input[++j])
				{
					if (ft_strncmp(commands[i].input[j], "heredoc", 7) == 0)
					{
						fd = open("heredoc.txt", O_RDWR | O_CREAT | O_TRUNC, 777);
						heredoc("eof", fd);
						fd = open("heredoc.txt", O_RDWR);
						dup2(fd, STDIN_FILENO);
					}
					else
						open(commands[i].input[j], O_RDWR, 777);
				}
				if (ft_strncmp(commands[i].input[j - 1], "heredoc", 7) != 0)
					dup2(open(commands[i].input[j - 1], O_RDWR, 777) , STDIN_FILENO);
			}
			if (commands[i].output != NULL)
			{
				j = -1;
				while (commands[i].output[++j])
					dup2(open(commands[i].output[j], O_RDWR | O_TRUNC | O_CREAT, 777), STDOUT_FILENO);
			}
			close_all_pipes(pipesfd, pipes_idmax);
			execve(commands[i].exec, commands[i].args, NULL);
		}
	}
	//closing pipes
	close_all_pipes(pipesfd, pipes_idmax);
	//waiting chidls
	int status;
	pid_t wait_return;
	while ((wait_return = wait(&status)) > 0);
}
