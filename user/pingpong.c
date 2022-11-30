#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
	int pid;
	int p[2];
	pipe(p);

	if(fork()==0) /* child receive->send */
	{
		pid = getpid();
		char buf[1];
		memset(buf, 0, sizeof(buf));
		if(read(p[0], buf, sizeof(buf) < 0)) /* 从管道读 */
		{
			fprintf(2, "read error\n");
			exit(0);
		}
		printf("%d: received ping\n", pid);
		close(p[0]);

		if(write(p[1], buf, sizeof(buf)) != sizeof(buf)) /* send a byte to the parent*/
		{
			fprintf(2, "write error\n");
			exit(0);
		}
		close(p[1]);
		exit(0);
	}
	else /* parent */
	{
		pid = getpid();
		char buf[1];
		memset(buf, 0, sizeof(buf));
		buf[0] = 'a';
		if(write(p[1], buf, sizeof(buf)) != sizeof(buf)) /* send a byte to the child */
		{
			fprintf(2, "write error\n");
			exit(0);
		}
		close(p[1]);

		wait(0);
		if(read(p[0], buf, sizeof(buf) < 0)) /* 从管道读 */
		{
			fprintf(2, "read error\n");
			exit(0);
		}
		printf("%d: received pong\n", pid);
		close(p[0]);
		exit(0);
	}
	return 0;
}
		
