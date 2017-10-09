/* 
 * Auteur(s):
 */

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

char msg1[] = "message 1";
char msg2[] = "message 2";
int prevSig = -1;

int sig_avant = -1;		/* pour la question 4.3 */

void hdl_sys1(int n) {
  printf("hdl_sys1: Signal recu: %d\n", n);

}


void travail() {
  /* Je travaille tres intensement !    */
  /* Ne cherchez pas a comprendre ;-) */
  /* Il n'y a rien a modifier ici     */
  const char msg[] = "-\\|/";
  const int sz = strlen(msg);
  int i = 0;

  for (;;) {
    write(STDOUT_FILENO, "\r", 1);
    usleep(100000);
    write(STDOUT_FILENO, " => ", 4);
    write(STDOUT_FILENO, &msg[i++], 1);
    if (i == sz) i = 0;
  }
}
void travail() __attribute__((noreturn));
/* Petit raffinement pour le compilateur: cette fonction ne termine pas */

void sighandlerINT1(int signum) {
  printf("Message: %s\n", msg1);
}

void sighandlerINT2(int signum) {
  printf("Message: %s\n", msg2);
}

void sighandlerQUIT(int signum) {
  printf("change siginal\n");
  sig_avant = signal(SIGINT, sig_avant);
}

int main() {
  printf("PID: %d\n", getpid());
  signal(SIGINT, sighandlerINT1);
  sig_avant = sighandlerINT2;
  signal(SIGQUIT, sighandlerQUIT);
  
  /* ? ? ? ? ? ? */
  
  travail();
  
}
