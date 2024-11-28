#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"
#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoi
char *welcome_message = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'exit\'\n";
char *prompt_message = "enseash % ";


void print_welcome_message(){
    char input[MAX_INPUT_SIZE];
    write(STDOUT_FILENO, welcome_message, strlen(welcome_message));
    write(STDOUT_FILENO, prompt_message, strlen(prompt_message));
    read(STDIN_FILENO,input,strlen(input));
}

