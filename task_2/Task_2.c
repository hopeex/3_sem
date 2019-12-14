#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

typedef struct
{
  int down[2]; 
  int up[2]; 
} dpipe_t;

int main()
{
	int pid;
	dpipe_t dpipe;
	int size;
	char buf[4096];
	char sourse[10];//from whom we will receive data
	int test[2];
	test[0] = pipe(dpipe.up);
	test[1] = pipe(dpipe.down);
	if ((test[0] < 0) || (test[1] < 0)){
		printf("Pipe creation is failed!\n");
		return -1;
		}

	
	while(1){//we create a loop
		printf("From whom do you want to receive data? ch/par:");
		scanf("%s", sourse);
		pid = fork();
		if (pid < 0){
			printf("fork failed!\n");
			close(dpipe.down[0]);
			close(dpipe.down[1]); 
			close(dpipe.up[0]);
           		close(dpipe.up[1]);
			return 0;
		}
		buf[0] = '\0';
		if (strcmp("ch", sourse) == 0) { //we want to get data from child
			if (pid > 0)//we are in parent process
			{
				while((size = read(dpipe.up[0], buf, sizeof(buf))-1) > 0)
						printf("%d: Parent recieved %s\n", getpid(), buf);	
			}
			else{ //we are in child process
				while((size = read(0, buf, sizeof(buf)-1)) > 0){
					buf[size-1] = '\0';
					printf("%d: Child sent %s", getpid(), buf);
					write(dpipe.up[1], buf, size);
					}
				}
		
			}
		else if (strcmp("par", sourse) == 0) { //we want to get data from parent
			if (pid == 0)//we are in child process
                        	{
                                	while((size = read(dpipe.down[0], buf, sizeof(buf))-1) > 0)
                                    		printf("%d: Child recieved %s\n", getpid(), buf);
                        	}
                       	else{//we are in parent process
                                while((size = read(0, buf, sizeof(buf)-1)) > 0){
                                        	buf[size-1] = '\0';
                                        	printf("%d: Parent sent:%s", getpid(), buf);
                                        	write(dpipe.down[1], buf, size);
                                	}
				}
			}
		else{
			printf("You have printed a wrong word\n");
			return 0;
			}
		}
	close(dpipe.down[0]);
        close(dpipe.down[1]);
        close(dpipe.up[0]);
        close(dpipe.up[1]);
	return 0;
}
