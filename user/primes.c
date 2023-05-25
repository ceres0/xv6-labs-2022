#include <kernel/types.h>
#include <user/user.h>
void Close(int *p){
	close(p[0]);
	close(p[1]);
}
void primes(){
	int num, p[2], res;
	if(read(0, &num, 1) != 1) return;
	fprintf(1, "prime %d\n", num);
	pipe(p);
	while(read(0, &res, 1)){
		if(res % num){
			write(p[1], &res, 1);
		}
	}
	close(0);
	dup(p[0]);
	Close(p);
	primes();
}
int main(int argc, char* argv[]){
	int i;
	int p[2];
	pipe(p);
	int pid = fork();
	if(pid > 0){
		close(0);
		dup(p[0]);
		Close(p);
		primes();
		exit(0);
	}
	else{
		for(i = 2; i < 36; i++){
			write(p[1], &i, 1);
		}
		exit(0);
	}
}
