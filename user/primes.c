#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_proc(int* fd)
{
	int p;
	close(fd[1]);
	if(read(fd[0], (void*)&p, sizeof(p)) != sizeof(p))
	{
		fprintf(2, "read error\n");
		exit(1);
	}
	printf("prime %d\n", p);

	int p1;
	if(read(fd[0], (void*)&p1, sizeof(p1)))
	{
		int fd1[2];
		pipe(fd1);
		if(fork()==0)
		{
			new_proc(fd1);
		}
		else
		{
			close(fd1[0]);

			do
			{
				/* 重点0 */
				if(p1%p != 0)
				{	
					write(fd1[1], (void*)&p1, sizeof(p1));
				}
			}while(read(fd[0], (void*)&p1, sizeof(p1)));

			close(fd1[1]);
			close(fd[0]);
			wait(0);
			exit(0);
		}
	}
	exit(0);
}
		

int
main(int argc, char* argv[])
{
	int start = 2;
	int end   = 35;

	int fd[2];
	pipe(fd);

	if(fork()==0)
	{
		new_proc(fd);
	}
	else
	{
		close(fd[0]);
		for(int i = start; i < end; i++)
		{
			if(write(fd[1], (void*)&i, sizeof(i)) != 4)
			{
				fprintf(2, "Write fail\n");
				exit(0);
			} 
		}
		close(fd[1]);
		wait(0);
	}
	
	return 0;
}
