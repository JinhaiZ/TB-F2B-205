#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define BUF_SIZE 1024

int server;	/* socket du serveur en attente de connection */
char * sockname;	/* nom de la socket */

static inline int max(int x, int y) {
  return x > y ? x : y; 
}  
  
void hdlr_fin(int sig) {
  printf("Reception du signal %d. Arret du bus !\n", sig);
  close(server);
  unlink(sockname);
  exit(EXIT_SUCCESS);
}


int main(int argc, char **argv) {
  struct sockaddr_un a;
  int * clients;	/* sockets des clients */
  int max_clients;	/* nombre de clients */
  char buffer[BUF_SIZE];
  int r, i, j;
  ssize_t rd_sz, wr_sz;

  if (argc != 3) {
    fprintf(stderr,
      "Utilisation:\n\t%s <Unix socket path> <max clients>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /*
  * The SIGPIPE signal will be received if the peer has gone away
  * and an attempt is made to write data to the peer. Ignoring this
  * signal causes the write operation to receive an EPIPE error.
  * Thus, the user is informed about what happened.
  */
  signal(SIGPIPE, SIG_IGN);
  
  /* pour une terminaison propre sur QUIT TERM INT SEGV... */
  signal(?????,  hdlr_fin);
  signal(?????, hdlr_fin);
  signal(?????, hdlr_fin);
  signal(?????, hdlr_fin);

  max_clients = atoi(argv[2]);
  clients = (int *) malloc( max_clients * sizeof(int) );
  for (i=0; i<max_clients; i++)	/* initialisation de cette structure */
    clients[i] = -1;	/* convention choisie  -1 = pas de client */ 


  /* creation de la socket serveur */
  if ((server = ????????????????) < 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  /* preparation de la structure d'adresse de la socket serveur sur 
     laquelle on attend les connexions */
  memset(&a, 0, sizeof(a));
  a.sun_family = ?????????;
  sockname = strncpy(???????????????????????/);

  if (bind(???????????????????????) < 0) {
    perror("bind()");
    close(server);
    exit(EXIT_FAILURE);
  }
  if ( listen(?????????????????) < 0 ) {
    perror("socket()"); 
    close(server);
    exit(EXIT_FAILURE);
  }
  
  /* le corps du programme */
  printf("Demarrage du bus.\n");
  for (;;) {
    int nfds = 0;
    fd_set rd_set;  /* Catalogue des sockets interessantes en lecture */
    
    /* Nettoyage de ce catalogue */
    FD_ZERO(&rd_set);

    /* On y place les descripteurs interessants en lecture */
    /* On s'interesse a la socket du serveur pour d'eventuelles connexions */
    FD_SET(server, &rd_set);
    nfds = max(nfds, server);
    /* ajoute les sockets des clients */
    for (i=0; i<max_clients; i++) {
      if ( clients[i] > 0 ) {
        FD_SET(clients[i], &rd_set);
        nfds = max(nfds, clients[i]);
      }
    }

    /* Se bloque en attente de quelque chose d'interessant sur une socket */
    r = select(????????????????????????);

    if (r == -1 && errno == EINTR)
      continue;
    if (r < 0) {
      perror("select()");
      exit(EXIT_FAILURE);
    }

    if (FD_ISSET(server, &rd_set)) {	/* on a une nouvelle connection */
      r = accept(???????????????????????);
      if (r < 0) {
	perror("accept()");
      } else {
        /* on recherche un emplacement de libre dans notre liste de clients */
        for (i=0; (i<max_clients) && (clients[i] > 0); i++);
        if (i < max_clients) {
          printf("Un nouveau client %d se connecte.\n", i);
          /* on y ajoute la socket du client */
          clients[i] = r;
        }
        else {
          printf("Plus de place pour un nouveau client.\n");
          close(r);
        }
      }
    }

    /* Traite les lectures possibles */
    /* Pour chaque donnee lue, on la renvoie a tous les autres */
    for (i=0; i<max_clients; i++) {
      if ( (clients[i] > 0) && FD_ISSET(clients[i], &rd_set)) {
        /* Hypothese : on lit le paquet d'un seul coup !
           Prevoir une buffer assez grand */
        rd_sz = recv(?????????????????????);
	if (rd_sz < 0) {
	  perror("recv()");
	  fprintf(stderr, "...probleme avec le client %d\n",i);
          shutdown(????????????);
          clients[i] = ???????????;
        } else if (rd_sz == 0) {
          printf("Le client %d et partit.\n", i);
          close(??????????);
          clients[i] = ??????????;
        } else if (rd_sz > 0) {
          printf("Reception de %d octets du client %d : [\n", rd_sz, i);
	  /* On ecrit sur la sortie standard */
          write(??????????????????);
          printf("]\n");
          /* Envoie le paquet (en un seul coup) a tous les autres clients */
          for (j=0; j<max_clients; j++) {
            if ( (clients[j] > 0) && (i != j) ) {
              wr_sz = send(??????????????????);
              if ( wr_sz < 0 ) {
                /* cloture du client j */
                perror("send()");
                fprintf(stderr, "...probleme avec le client %d\n",j);
               ????????????????????????
              } else if (wr_sz == 0 ) {
                printf("Le client %d et partit.\n", j);
                ?????????????????
              } else 
                printf("Envoie de %d octets au client %d.\n", wr_sz, j);
            }
          }
        }
      }
    }
    
  }
  /* superflu apres la boucle infinie */
  exit(EXIT_SUCCESS);
}
