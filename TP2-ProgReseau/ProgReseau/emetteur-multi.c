#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 100000

int main(int argc, char **argv) {
	int sfd, s, val, volume;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	struct sockaddr *sa;
	socklen_t salen;
	char buf[BUFSIZE];
	ssize_t nsend;

	if (argc != 3) {
		printf("Usage: %s port_serveur volume\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	volume = atoi(argv[2]);
	if (volume == 0 || volume > BUFSIZE) {
		fprintf(stderr, "Donnez un volume inferieur a %d octets\n", BUFSIZE);
		exit(EXIT_FAILURE);
	}

	/* Initialisation du buffer */
	memset(buf, 'a', volume);

	/*
	 * Obtention de l'adresse IP du distant, a partir de son nom par
	 * consultation du fichier /etc/hosts ou de la base hosts des NIS
	 * ou du DNS (Domain Name Service)
	 * cf. man getaddrinfo(3)
	 */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;          /* IPv4 ou IPv6 */
	hints.ai_socktype = SOCK_DGRAM;       /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;                /* Any protocol */

	s = getaddrinfo("224.2.2.10", argv[1], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	/* getaddrinfo() retourne une liste de structures d'adresses.
	   On essaie chaque adresse jusqu'a ce que socket(2) reussisse. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		/* Ouverture de la socket */
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd >= 0)
			break;
	}
	if (rp == NULL) {     /* Aucune adresse valide */
		fprintf(stderr, "Impossible d'ouvrir une socket vers %s\n", argv[1]);
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/*
	 * Construction de la structure d'adresse du distant
	 */
	sa=malloc(rp->ai_addrlen);
	memcpy(sa, rp->ai_addr, rp->ai_addrlen);
	salen=rp->ai_addrlen;

	freeaddrinfo(result); /* Plus besoin */

	/* Permettons le broadcast (IPv4 only) */
	val = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	for(;;){
		char *pt;
		pt = fgets(buf, volume, stdin);
		if(pt == NULL){
			printf("Sortie du client\n");
			exit(EXIT_SUCCESS);
		}

		/* Envoi donnees */
		nsend = sendto(sfd, buf, volume, 0, sa, salen);
		if (nsend <= 0)
			perror("sendto");
	}
	exit(EXIT_SUCCESS);
}
