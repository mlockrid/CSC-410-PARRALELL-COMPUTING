#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Starting shield power level
int shield_power = 50;  

int main() {
    pid_t pid;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);

    // Create 4 child processes - 4 different characters adjusting shield power
    for (int i = 0; i < 4; i++) {
        pid = fork();

        if (pid == 0) {  // Child process
            if (i == 0) {
                printf("Luke increases the shield power by 25%%\n");
                shield_power = shield_power + 25;
                printf("Sheild is at %d%% power\n",shield_power);
                return 0;
            } else if (i == 1) {
                printf("Han increases the shield power by 20%%\n");
                shield_power = shield_power + 20;
                printf("Sheild is at %d%% power\n",shield_power);
                return 0;
            } else if (i == 2) {
                printf("Chewbacca increases the shield power by 30%%\n");
                shield_power = shield_power + 30;
                printf("Sheild is at %d%% power\n",shield_power);
                return 0;
            } else if (i == 3) {
                printf("Leia increases the shield power by 15%%\n");
                shield_power = shield_power + 15;
                printf("Sheild is at %d%% power\n",shield_power);
                return 0;
            }
            _exit(0);  // Child exits after adjusting shield power
        } else if (pid < 0) {
            // Process creation failed
            while(wait(NULL) > 0);
            printf("Error: Failed to create child process\n");
            return 1;
        }
    }

    while(wait(NULL) > 0);
    printf("All child proccesses are complete\n");


    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}
