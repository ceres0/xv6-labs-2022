#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *fn){
	char *pfn, buf[512], *p;
	//printf("find %s ...\n", path);
	int fd;
	struct dirent de;
	struct stat st;
	if((fd = open(path, 0)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}
	//printf("%d %s\n", st.type, fmtname(path));
	switch(st.type){
	case T_DEVICE:
	case T_FILE:
		pfn = fmtname(path);
		//printf("%s : %s\n", pfn, fn);
		if(strcmp(pfn, fn) == 0)
			printf("%s\n", path);
		break;
	case T_DIR:
		strcpy(buf, path);
		p = buf + strlen(buf);
		*p++ = '/';
		while(read(fd, &de, sizeof(de)) == sizeof de){
			//printf("de: %s\n", de.name);
			if(de.inum == 0 || strcmp(de.name, "..") == 0 || strcmp(de.name, ".") == 0) continue;
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			find(buf, fn);
		}
		break;
	}
	close(fd);

}
int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Usage: find [<path>] <filename>\n");
		exit(1);
	}
	else if(argc == 2){
		find(".", argv[1]);
		exit(0);
	}
	find(argv[1], argv[2]);
	exit(0);
}
