#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 512
#define MAXPENDING 5    /* Max connection requests */

void communication(int s, struct sockaddr_storage from, int fromlen, char* host, ssize_t nread, ssize_t nwrite, int ns, char* message, char* buf) {
  /* Reconnaissance de la machine cliente */
   
  s = getnameinfo((struct sockaddr *)&from, fromlen,
  host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
  if (s == 0)
    printf("Debut avec client '%s'\n", host);
  else
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

  for (;;) {
    nwrite = write(ns, message, strlen(message));
    if (nwrite < 0) {
      perror("write");
      close(ns);
      break;
    }
    nread = read(ns, buf, BUFSIZE);
    if (nread == 0) {
      printf("Fin avec client '%s'\n", host);
      close(ns);
      break;
    } else if (nread < 0) {
      perror("read");
      close(ns);
      break;
    }
    buf[nread] = '\0';
    printf("Message recu '%s'\n", buf);
  }  
}

void sig_handler(int signum) {
  printf("child signal recevied\n");
  int pid = wait(NULL);
  printf("child process %d terminated\n", pid);
}

int main(int argc, char **argv) {
  int pid, st, sfd, s, ns, r;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  char buf[BUFSIZE];
  ssize_t nread, nwrite;
  struct sockaddr_storage from;
  socklen_t fromlen;
  char host[NI_MAXHOST];
  char *message = "Message a envoyer: ";

  if (argc != 2) {
    printf("Usage: %s  port_serveur\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Construction de l'adresse locale (pour bind) */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;           /* Force IPv6 */
  hints.ai_socktype = SOCK_STREAM;      /* Stream socket */
  hints.ai_flags = AI_PASSIVE;          /* Adresse IP joker */
  hints.ai_flags |= AI_V4MAPPED|AI_ALL; /* IPv4 remapped en IPv6 */
  hints.ai_protocol = 0;                /* Any protocol */

  s = getaddrinfo(NULL, argv[1], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() retourne une liste de structures d'adresses.
     On essaie chaque adresse jusqu'a ce que bind(2) reussisse.
     Si socket(2) (ou bind(2)) echoue, on (ferme la socket et on)
     essaie l'adresse suivante. cf man getaddrinfo(3) */
  for (rp = result; rp != NULL; rp = rp->ai_next) {

    /* Creation de la socket */
    sfd = socket(rp->ai_family, rp->ai_socktype, 0); 
    if (sfd == -1)
      continue;

    /* Association d'un port a la socket */
    r = bind(sfd, rp->ai_addr, rp->ai_addrlen);
    if (r == 0)
      break;            /* Succes */
    close(sfd);
  }

  if (rp == NULL) {     /* Aucune adresse valide */
    perror("bind");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(result); /* Plus besoin */

  /* Positionnement de la machine a etats TCP sur listen */
  listen(sfd, MAXPENDING); 

  for (;;) {
    /* Acceptation de connexions */
    fromlen = sizeof(from);
    ns = accept(sfd, (struct sockaddr *)&from, &fromlen);
    if (ns == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    pid = fork();

    switch(pid) {
      case -1: //problem
        fprintf(stderr, "Error of server\n");
        exit(EXIT_FAILURE);
      case 0: //we are in the child
        close(sfd); // close father socket
        communication(s, from, fromlen, host, nread, nwrite, ns, message, buf);
        exit(EXIT_SUCCESS);
      default: //we are in the father
        close(ns);
        signal(SIGCLD, sig_handler);
    }
  }
}
