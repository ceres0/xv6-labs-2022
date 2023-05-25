#include <kernel/types.h>
#include <user/user.h>
int main(int argc, char* argv[]){
	int p[2];
	if(pipe(p) < 0){
		fprintf(2, "Pingpong: pipe failed");
	}	
	int pid = fork();
	if(pid < 0){
		fprintf(2, "Pingpong: fork failed");
		exit(1);
	}
	else if(pid > 0){
		char ch;
		wait(0);
		if(read(p[0], &ch, 1) != 1){
			fprintf(2, "Pingpong: read failed");
			exit(1);
		}
		close(p[0]);
		close(p[1]);
		fprintf(2, "%d: received pong\n", getpid());
		exit(0);
	}
	else{
		if(write(p[1], "x", 1) != 1){
			fprintf(2, "Pingpong: write failed");
			exit(1);
		}
		close(p[0]);
		close(p[1]);
		fprintf(2, "%d: received ping\n", getpid());
		exit(0);
	}
}
