#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define BUFSIZE 512

int main(int argc, char **argv) {
  int s, sfd, r;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  char buf[BUFSIZE], *pt;
  ssize_t nread, nwrite;

  if (argc != 3) {
    printf("Usage: %s nom_machine_distante port_serveur\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /*
   * Obtention de l'adresse IP du distant, a partir de son nom par
   * consultation du fichier /etc/hosts ou de la base hosts des NIS
   * ou du DNS (Domain Name Service)
   * cf. man getaddrinfo(3)
   */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;          /* IPv4 ou IPv6 */
  hints.ai_socktype = SOCK_STREAM;      /* Stream socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;                /* Any protocol */

  s = getaddrinfo(argv[1], argv[2], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }


  /* getaddrinfo() retourne une liste de structures d'adresses.
     On essaie chaque adresse jusqu'a ce que connect(2) reussisse.
     Si socket(2) (ou connect(2)) echoue, on (ferme la socket et on)
     essaie l'adresse suivante. cf man getaddrinfo(3) */
  for (rp = result; rp != NULL; rp = rp->ai_next) {

    /* Ouverture de la socket */
    sfd = ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    if (sfd == -1)
      continue;

    /* Connexion au distant */
    r = ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    if (r != -1)
      break;            /* Succes */
    close(sfd);
  }

  if (rp == NULL) {     /* Aucune adresse valide */
    perror("connect");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(result); /* Plus besoin */


  /* Dans ce qui suit, n'oubliez pas de tester le code de retour des
   * fonctions utilisees et d'emettre un message d'erreur avec perror(),
   * puis de sortir avec exit(). */

  /* Boucle de communication */
  for (;;) {
    /* Lecture socket */
    nread = read(sfd, buf, BUFSIZE);
    if (nread == 0) {
      printf("Connexion rompue\n");
      exit(EXIT_SUCCESS);
    } else if (nread < 0) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    buf[nread] = '\0';
    /* Affichage ecran du message lu sur la socket */
    printf("Message recu '%s'", buf);

    /* Lecture clavier. Si on tape <Control-D>, gets() rend NULL
     * <Control-D> symbolise la fin de fichier, ici la terminaison
     * du client */
    pt = fgets(buf, BUFSIZE, stdin);
    if (pt == NULL) {
      printf("Sortie du client\n");
      exit(EXIT_SUCCESS);
    }
    nwrite = write(sfd, buf, strlen(buf));
    if (nwrite < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }
}
