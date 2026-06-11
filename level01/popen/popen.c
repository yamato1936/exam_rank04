#include <unistd.h>
#include <stdlib.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int	fd[2], pid;

	if (!file || !argv || (type != 'r' && type != 'w') || pipe(fd) || (pid = fork()) < 0)
		return (-1);
	if (!pid)
	{
		if ((type == 'r' && dup2(fd[1], 1) < 0) || (type == 'w' && dup2(fd[0], 0) < 0))
			exit(1);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
		return (close(fd[1]), fd[0]);
	return (close(fd[0]), fd[1]);
}
