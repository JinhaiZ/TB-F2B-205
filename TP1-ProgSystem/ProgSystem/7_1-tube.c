#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
	int fd[2];
	pid_t cpid;
    char fa_buf;
    char ch_buf;


	if (pipe(fd) < 0){
		perror("Create pipe failed\n");
		exit(EXIT_FAILURE);
	}
    // create child process
    if ((cpid = fork()) < 0) {
      printf("Fork failed\n");
      exit(EXIT_FAILURE);
    }
	
	switch(cpid) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
        case 0:
            close(fd[1]);
			while (read(fd[0], &ch_buf, 1) > 0) { // Child reads from pipe
                write(STDOUT_FILENO, &ch_buf, 1); // Child writes to standard output
            }
			exit(EXIT_SUCCESS);
        default:
            close(fd[0]);
            while (read(0, &fa_buf, 1) > 0) { // Father reads from standard input
                write(fd[1], &fa_buf, 1); // Father writes to pipe
            }
			wait(NULL);
			exit(EXIT_SUCCESS);
	}
}
