#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "enseash.h"

#include "shell_utils.h"
#include "utils.h"

#define concat(...) concat_with_necessary_end_null(__VA_ARGS__, NULL) // define the macro concat to call the function concat_with_necessary_end_null with the NULL argument at the end

#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoid problems with large inputs

char exit_command[] = "exit";
char prompt_message[] = "enseash % ";
char exit_message[] = "Bye bye...\n";

int main(){
    print_shell(concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\'\n"));       
    return REPL();
}

int REPL(){
    while(1){
        char input[MAX_INPUT_SIZE];
        
        print_shell(prompt_message);
        read_shell(input,MAX_INPUT_SIZE);
        if(strncmp(input,exit_command,strlen(input)) == 0 ){
            print_shell(exit_message);
            return EXIT_SUCCESS;
        }
        else{ // this else is not necessary but it is here to make the code more readable
            int exit_code_cmd = execute_command(input);
            if(exit_code_cmd == EXIT_FAILURE){
                print_shell("La commande a échoué, réessayez\n");
            }
        }
    }
}