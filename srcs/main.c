#include "../includes/execution.h"

void	fill_commands(t_command *commands, char **cmds, char *args[10][10], char **files)
{
	int i;

	i = 0;
	while (cmds[i])
	{
		commands[i].exec = cmds[i];
		commands[i].args = args[i];
		i++;
	}
	//commands[0].output = files[0];
	commands[0].input = files[1];
	commands[5].output = files[2];
	commands[5].input = files[3];
	commands[6].output = files[4];
	commands[6].input = files[5];
}

int main(int argc, char **argv)
{
	(void)argc, (void)argv;
	//numeric datas and pipes
	int	nb_cmds = 7;
	int pipes_idmax = 5;
	int pipesfd[pipes_idmax + 1][2];
	int pipe_count;
	//struct cmd
	char *cmds[] = {"/bin/cat", "/usr/bin/grep", "/bin/cat", "/usr/bin/rev", "/bin/cat", "/usr/bin/grep", "/usr/bin/wc", NULL};
	char *args[][10] = {{"cat", NULL}, {"grep", "file", NULL}, {"cat", NULL}, {"rev", NULL}, {"cat", "-e", NULL}, {"grep", "i", NULL}, {"wc", "-c", NULL}, {NULL}};
	char *files[]  ={"output1.txt", "input1.txt", "output2.txt", "input2.txt", "input3.txt", "output3.txt", NULL};
	t_command commands[10];
	//create pipes
	int i;
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
			if (i != 0)
			{
				//Not the first cmd -> input = previous pipe;
				if (commands[i].input != NULL)
					dup2(open(commands[i].input, O_RDWR | O_CREAT, 777) , STDIN_FILENO);
				else
					dup2(pipesfd[pipe_count - 1][READ] , STDIN_FILENO);
				if (commands[i].output != NULL)
					dup2(open(commands[i].output, O_RDWR | O_TRUNC | O_CREAT, 777), 1);
			}
			if (i < nb_cmds - 1)
			{
				//not the last cmd (i starts at 0 so if i = 6 we are at the 7th cmd) -> output = next pipe;
				if (commands[i].output != NULL)
					dup2(open(commands[i].output, O_RDWR | O_TRUNC | O_CREAT, 777), 1);
				else
					dup2(pipesfd[pipe_count][WRITE], STDOUT_FILENO);
				if (commands[i].input != NULL)
					dup2(open(commands[i].input, O_RDWR | O_CREAT, 777) , STDIN_FILENO);
			}
			//close
			int j = -1;
			while (++j <= pipes_idmax)
			{
				close(pipesfd[j][READ]);
				close(pipesfd[j][WRITE]);
			}
			execve(commands[i].exec, commands[i].args, NULL);
		}
		else
		{
			//dad
		}
	}
	//closing pipes
	i = -1;
	while (++i <= pipes_idmax)
	{
		close(pipesfd[i][READ]);
		close(pipesfd[i][WRITE]);
	}
	//waiting chidls
	int status;
	pid_t wait_return;
	while ((wait_return = wait(&status)) > 0);
}
