#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "functions.h"
#include "shell_utils.h"
#include "utils.h"


#define concat(...) concat_with_necessary_end_null(__VA_ARGS__, NULL) // define the macro concat to call the function concat_with_necessary_end_null with the NULL argument at the end

#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoid problems with large inputs

char exit_command[] = "exit";
char prompt_message[] = "enseash % ";


void print_welcome_message(){
    char input[MAX_INPUT_SIZE];
    print_shell(concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\'\n"));
    print_shell(prompt_message);
    read_from_shell(input,MAX_INPUT_SIZE);
    execute_command(input);
}

int execute_command(char *command){
    pid_t pid = fork();
    // checks if the fork was successful OR if the process is the child
    if(pid <= 0){
        // checks if the fork was successful
        if(pid < 0){
            perror("Fork Error");
        }
        // checks if the successfully forked process has error during execution
        else
        {
            execlp(command,command,NULL);
            perror("Command Error");
        }
        exit(EXIT_FAILURE);
    }
    else{
        int status;
        waitpid(pid,&status,0);
        // Check if the child process exited normally
        if (!WIFEXITED(status)) {
            print_shell("La commande à échouhé\n");
        } 
        return WEXITSTATUS(status); // 1 if failed, 0 if success
    }
}