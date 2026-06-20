#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int	i;
	int	in;
	int	fd[2];
	int	pid;
	int	st;
	int	ret;

	i = 0;
	in = -1;
	ret = 0;
	while (cmds[i])
	{
		fd[0] = -1;
		fd[1] = -1;
		if (cmds[i + 1] && pipe(fd) < 0)
		{
			if (in != -1)
				close(in);
			return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			if (in != -1)
				close(in);
			if (fd[0] != -1)
				close(fd[0]);
			if (fd[1] != -1)
				close(fd[1]);
			return (1);
		}
		if (pid == 0)
		{
			if (in != -1)
			{
				if (dup2(in, 0) < 0)
					exit(1);
				close(in);
			}
			if (cmds[i + 1])
			{
				if (dup2(fd[1], 1) < 0)
					exit(1);
				close(fd[0]);
				close(fd[1]);
			}
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
	{
		if (st != 0)
			ret = 1;
	}
	return (ret);
}
