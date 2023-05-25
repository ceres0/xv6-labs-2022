#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "stdbool.h"
int main(int argc, char *argv[]){
	int fd[2];
	char *argv1[MAXARG], *buf;
	char *p;
	bool flag = true;
	buf = malloc(512);
	memset(argv1, 0, sizeof argv1);
	pipe(fd);
	int i;
	for(i = 1; i < argc; i++){
		argv1[i - 1] = argv[i]; 
	}
	while(flag){
		p = buf;
		while(read(0, p, 1)){
			if(*p == ' ' || *p == '\n'){
				break;
			}
			p++;
		}
		if(*p != '\n' && *p != ' '){
			p[1] = '\0';
			flag = 0;
		}
		else{
			p[0] = '\0';
		}
		if(p != buf){
			write(fd[1], buf, sizeof buf);
			wait(0);
			if(fork() == 0){
				read(fd[0], buf, sizeof buf);
				//printf(":%s\n", buf);
				argv1[argc - 1] = buf;
				exec(argv1[0], argv1);
				exit(1);
			}
		}
		//printf(".");
	}
	//printf("#\n");
	close(fd[1]);
	exit(0);
}
