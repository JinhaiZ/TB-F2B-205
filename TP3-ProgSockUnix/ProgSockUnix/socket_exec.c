#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>


static int mysocketpair(int d, int type, int protocol, int fd[2])
{
	int listener;
	struct sockaddr_in sock;
	struct sockaddr_in sock2;
	socklen_t socklen = sizeof(sock);
	
	fd[0] = fd[1] = listener = -1;

	memset(&sock, 0, sizeof(struct sockaddr_in));
	
	listener = socket(d, type, protocol);

  memset(&sock2, 0, sizeof(struct sockaddr_in));

  sock2.sin_family = d;

  bind(listener, (struct sockaddr *)&sock2, sizeof(sock2));

	listen(listener, 1);

	getsockname(listener, (struct sockaddr *)&sock, &socklen);

	fd[1] = socket(d, type, protocol);

	//set_blocking(fd[1], 0);

	sock.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	connect(fd[1], (struct sockaddr *)&sock, socklen);


	fd[0] = accept(listener, (struct sockaddr *)&sock, &socklen);


	connect(fd[1], (struct sockaddr *)&sock, socklen);

	close(listener);

	//set_blocking(fd[1], 1);

	return 0;
}

int socket_exec(const char *command)
{
	int fd[2];
	if (mysocketpair(PF_UNIX, SOCK_STREAM, 0, fd) != 0) {
	  printf("socketpair failed\n");
		return -1;
	}
	if (fork() == 0) {
		close(fd[0]);
		close(0);
		close(1);
		if (dup(fd[1]) == -1) {
			exit(1);
		}
		if (dup(fd[1]) == -1) {
			exit(1);
		}
		exit(execl("/bin/sh", "sh", "-c", command, (char *) 0));
	}
	close(fd[1]);
	return fd[0];
}

#define MSG "coucou \ntout le monde\n"

int main() {
  char buff[200];
  ssize_t sz;

  int se = socket_exec("wc");
  write(se, buff, strlen(MSG) + 1);

  shutdown(se, SHUT_WR);

  sz = read(se, buff, 200);
  buff[sz] = '\0';
  printf("Result %s", buff);

  wait(0);
  exit(EXIT_SUCCESS);
}