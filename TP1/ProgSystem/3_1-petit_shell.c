/* 
 * Auteur(s):
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TABSIZE 512

int main() {
  int pid, longueur;
  char tab[TABSIZE], *s;

  for (;;) {
    fputs("petit_shell...> ", stdout);	/* Affichage d'un prompt */
    s = fgets(tab, TABSIZE, stdin);

    if (s == NULL) {
      fprintf(stderr, "Fin du Shell\n");
      exit(EXIT_SUCCESS);
    }

    longueur = strlen(s);
    tab[longueur - 1] = '\0';

    pid = fork();

    /* Actions:
     * 
     * Si dans pere alors
     *   wait(NULL);
     * sinon alors
     *   execution de la commande recuperee dans tab;
     *   message d'erreur: fprintf(stderr, "Erreur dans le exec\n")
     * fin si
     */

  }
}
