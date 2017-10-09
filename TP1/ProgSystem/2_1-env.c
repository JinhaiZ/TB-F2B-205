#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	extern char **environ;
	char *env_value;

    if (argc == 1) {
        for (int i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]);
            i++;
        }
    } 
    else if (argc == 2) {
        env_value = getenv(argv[1]);
		if (env_value == NULL){
			printf("variable %s don't exist\n", argv[1]);
			return EXIT_FAILURE;
		} else {
            printf("%s=%s\n", argv[1], env_value);
        }
    } 
    else {
        printf("Bad usage : %s [environment_variable]\n", argv[0]);
		return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
} 
