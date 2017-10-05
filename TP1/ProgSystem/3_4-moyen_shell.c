/* 
 * Auteur(s):
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define LINE_SIZE 256
#define NB_ARG_MAX 256

int main() {
  int pid, longueur, i, n;
  char tab[LINE_SIZE], *s;
  char *arg_tab[NB_ARG_MAX];

  while (TRUE) {
    fputs("petit_shell1...> ", stdout);	/* Affichage d'un prompt */
    s = fgets(tab, LINE_SIZE, stdin);	/* lecture du clavier */
    if (s == NULL) {
      fprintf(stderr, "Fin du Shell\n");
      exit(EXIT_SUCCESS);
    }
    longueur = strlen(s);
    tab[longueur - 1] = '\0';

    /* Analyse de la ligne de commande     
     * Dans un vrai Shell c'est beaucoup plus ellabore
     * avec l'analyse de la syntaxe, des metacaracteres, etc... */

    /* On repere l'adresse du debut de chaque mot de la ligne 
     * Cette adresse est stockee dans le tableau arg_tab
     * Le mot est termine par le caractere ASCII NUL (8eb a 0)
     * (Faites  man ascii   pour visualiser le tableau des
     * caracteres ascii)
     * Le tableau arg_tab est termine par le pointeur NULL */

    longueur = strlen(tab);
    i = 0;
    n = 0;
    while (i < longueur) {
      /* On passe les blancs et tabulation */
      while (tab[i] == ' ' || tab[i] == '\t') {
	i++;
      }
      if (tab[i] == '\0')
	break;			/* fin de ligne, on sort */

      /* stockage dans arg_tab de l'adresse trouvee */
      arg_tab[n++] = &tab[i];

      /* on va juste apres la fin du mot */
      while (tab[i] != ' ' && tab[i] != '\t' && tab[i] != '\0')
	i++;
      /* On place un caractere ASCII NUL apres le dernier
       * caractere du mot */
      tab[i++] = '\0';
    }

    /* Si n vaut 0, la ligne ne contenait que des blancs 
     * ou tabulations, on retourne en debut de boucle principale */
    if (n == 0)
      continue;

    /* On termine le tableau par le pointeur NULL */
    arg_tab[n] = NULL;

    /********* Fin de l'analyse de la ligne de commande *******/

    pid = fork();

    switch (pid) {
    ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?}
  }
}
