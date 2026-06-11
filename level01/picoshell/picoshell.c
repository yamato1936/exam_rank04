#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int	i = 0, in = -1, fd[2], pid, st, ret = 0;

	while (cmds[i])
	{
		fd[0] = -1;
		fd[1] = -1;
		if (cmds[i + 1] && pipe(fd))
			return (1);
		pid = fork();
		if (pid < 0)
			return (1);
		if (!pid)
		{
			if (in != -1)
				dup2(in, 0);
			if (cmds[i + 1])
				dup2(fd[1], 1);
			if (in != -1)
				close(in);
			if (fd[0] != -1)
				close(fd[0]);
			if (fd[1] != -1)
				close(fd[1]);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (in != -1)
			close(in);
		if (fd[1] != -1)
			close(fd[1]);
		in = fd[0];
		i++;
	}
	if (in != -1)
		close(in);
	while (wait(&st) > 0)
		if (st)
			ret = 1;
	return (ret);
}
