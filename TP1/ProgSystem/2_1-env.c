#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	extern char **environ;
    
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }

    return EXIT_SUCCESS;
} 
