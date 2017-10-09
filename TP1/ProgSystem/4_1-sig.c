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
struct sigaction sa_avant;

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

void sighandlerINT(int signum) {
  printf("Message: %s\n", msg1);
}

void sighandlerINT2(int signum) {
  printf("Message: %s\n", msg2);
}

void sighandlerQUIT(int signum) {
  printf("change siginal\n");
  sigaction(SIGINT, &sa_avant, &sa_avant);
}

int main() {
  printf("PID: %d\n", getpid());
  struct sigaction sa_int, sa_int2, sa_quit;
  sa_int.sa_handler = &sighandlerINT;
  sa_int2.sa_handler = &sighandlerINT2;
  sa_quit.sa_handler = &sighandlerQUIT;
  sa_avant = sa_int2;
  sigaction(SIGINT, &sa_int, NULL);
  sigaction(SIGQUIT, &sa_quit, NULL);
  
  travail();
}
