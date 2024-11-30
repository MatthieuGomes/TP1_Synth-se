#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#define concat(...) concat_with_necessary_end_null(__VA_ARGS__, NULL) // define the macro concat to call the function concat_with_necessary_end_null with the NULL argument at the end




char * generate_welcome_message(char* exit_command, char* exit_key_name){
    return concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\' ou \'",exit_key_name,"\' \n");
}

char * generate_prompt_infos(int exit_code){
    return concat("[exit:",int_to_str(exit_code),"]");
}

char * generate_prompt_message(char* prompt_title, char* prompt_suffix,char* prompt_infos){
    char *prompt_base_ptr=NULL;
    if(prompt_infos != NULL){
        prompt_base_ptr=concat(prompt_title," ",prompt_infos);
    }
    else{
        prompt_base_ptr=prompt_title;
    }
    char prompt_base[strlen(prompt_base_ptr)];
    strcpy(prompt_base,prompt_base_ptr);
    return concat(prompt_base, " ",prompt_suffix," ");
}
