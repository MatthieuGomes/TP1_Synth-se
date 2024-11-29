# TP1_Synth-se
TP1 de synthèse par Matthieu GOMES et Oumaima EL BOUROUMI

## Question 1

Nous avons séparé le code en 2 partie pour commencer : 
- La première partie est situé dans le fichier qui donne son nom au programme (enseash) et contient la fonction main. Cette dernière appelle la fonction `print_welcome_message()` définie dans le second fichier.
```c title="enseash.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "enseash.h"

int main(){
    print_welcome_message();        
}

```
*enseash.c*
```c title="enseash.h"
void print_welcome_message();
```
*enseash.h*
- Le second fichier (functions) contient pour le moment uniquement la fonction `print_welcome_message()` qui affiche un message de bienvenue.


On definie une taille max d'input pour la fonction `read` très grande pour ne pas avoir de problème de taille. Le reste se contente de suivre les consignes et d'utiliser les fonctions `write` et `read` pour afficher les messages et lire l'input de l'utilisateur.
```c title="functions.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"
#define MAX_INPUT_SIZE 1024
char *welcome_message = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'exit\'\n";
char *prompt_message = "enseash % ";


void print_welcome_message(){
    char input[MAX_INPUT_SIZE];
    write(STDOUT_FILENO, welcome_message, strlen(welcome_message));
    write(STDOUT_FILENO, prompt_message, strlen(prompt_message));
    read(STDIN_FILENO,input,strlen(input));
}
```
*functions.c*
```c title="functions.h"
void print_welcome_message();
```
*functions.h*

## Reorganisation du code

### Facilication de l'appel à `write` et `read`

Au vue de l'objectuf du TP, nous allons régulièrement avoir recours à `write` et `read`, mais nous utilisons 2 arguments répétitifs : `STDOUT_FILENO`/`STDIN_FILENO` et `strlen()`. Pour rendre plus agréable la lecture et le developpement, nous allons definir 2 fonctions qui vont nous permettre de simplifier l'appel à `write` et `read` en ne prenant en paramètre que le *message à afficher* ou *la variable dans laquelle stocker l'input* accompagnée de la taille maximum de l'input, afin déviter les répetitions de variables.

Pour plus de clarté, ces 2 fonctions seront définies dans un fichier `shell_utils.c` et `shell_utils.h`.

```c title="shell_utils.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"

#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoid buffer overflow

ssize_t print_shell(char *message){
    return write(STDOUT_FILENO, message, strlen(message));
}

ssize_t read_shell(char *input){
    ssize_t input_size = read(STDIN_FILENO,input,MAX_INPUT_SIZE);
    // ends correctly the string with a null character to avoid any buffer overflow
    input[input_size-1] = '\0';
    return input_size;
} 
```
*shell_utils.c*
`input[input_size-1] = '\0';` permet de terminer correctement l'input avec un caractère nul et supprimer le retour à la ligne.

```c title="shell_utils.h"
#include <unistd.h>

ssize_t read_shell(char *input);
ssize_t print_shell(char *message); 
```
*shell_utils.h*

Et voici la nouvelle implémentation de la fonction `print_welcome_message()` :
```c title="functions.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"
#include "shell_utils.h"
#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoi
char *welcome_message = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'exit\'\n";
char *prompt_message = "enseash % ";


void print_welcome_message(){
    char input[MAX_INPUT_SIZE];
    print_shell(welcome_message);
    print_shell(prompt_message);
    read_shell(input,MAX_INPUT_SIZE);
}
```
*functions.c*

### Ajout d'une fonction de concatenation multiple

Nous risquons d'avoir besoin d'une fonction de concatenation de chaines de caractères pour afficher des messages plus complexes. Nous allons donc ajouter une fonction `concat_with_necessary_end_null` dans un fichier utils.

```c title="utils.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


char* concat_with_necessary_end_null(char* string, ...) {


    va_list args; // list of arguments
    va_start(args, string); // start to read the arguments : args by args readable in the variable called string 
    
    size_t total_length = strlen(string);
    char* current;
    while ((current = va_arg(args, char*)) != NULL) {
        total_length += strlen(current);
    }
    va_end(args); // end of the reading of the arguments

    char* result = (char*)malloc(total_length + 1);

    result[0] = '\0'; 
    strcat(result, string); // start filling the result with the first string

    va_start(args, string); // going again to throught the arguments
    while ((current = va_arg(args, char*)) != NULL) {
        strcat(result, current);
    }
    va_end(args);

    return result;
}
```
*utils.c*

```c title="utils.h"
char * concat_with_necessary_end_null(char * string, ...);
```
*utils.h*

### Dynamisation du message de bienvenue

pour rendre le message de bienvenue plus facilement modifiable, nous allons inserer la variable `exit_commabnd` dans le message de bienvenue. De cette manière, si nous changeons la commande de sortie, le message de bienvenue sera automatiquement mis à jour. Pour ça nous utiliserons la fonction `concat_with_necessary_end_null` définie précédemment et nous définirons la variable `exit_command` dans le fichier `functions.c`. De plus, pour plus de faciliter nous definirons une macro `concat` qui permettra d'appeler la fonction `concat_with_necessary_end_null` sans taper l'argument `NULL` à la fin (necessaire pour que le parcours des arguments se termine correctement).

```c title="functions.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    read_shell(input,MAX_INPUT_SIZE);
}
```
*functions.c*

## Question 2

### a) Lecture de la commande saisie

Pour verifier que nous lisons bien la commande, nous allons afficher l'input de l'utilisateur après qu'il ait appuyé sur entrée. Pour cela, utilisons simplement la commande `print_shell` définie précédemment.

```c title="functions.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    read_shell(input,MAX_INPUT_SIZE);
    print_shell(concat("Vous avez saisi : ",input,"\n"));
}
```
*functions.c*

### b) Exécution d’une commande simple (sans argument)

Pour l'execution d'une commande simple, nous allons ajouter un fonction `execute_command` qui prend en argument la commande à executer. Nous prendrons en compte :
* les erreurs de forking
* les erreurs d'execution de la commande
Nous ajouterons cette fonction à notre fichier `functions.c` et nous remplacerons l'affichage de l'input par l'execution de la commande.

```c title="functions.c - execute_command()"
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
```
*functions.c - execute_command()*

```c title="functions.c - print_welcome_message()"
void print_welcome_message(){
    char input[MAX_INPUT_SIZE];
    print_shell(concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\'\n"));
    print_shell(prompt_message);
    read_shell(input,MAX_INPUT_SIZE);
    execute_command(input);
}
```
*functions.c - print_welcome_message()*

Nous avons également ajouter la fonction à notre fichier `functions.h` 

```c title="functions.h"
void print_welcome_message();
int execute_command(char *command);
```

### c) Retour au prompt enseash % et attente de la commande suivante

Pour revenir au prompt, nous allons devoir decouper notre fonction `print_welcome_message` en 2... Ou s'en débarasser.

Nous allons donc créer une fonction `REPL` (Read-Eval-Print-Loop) qui va afficher le `prompt_message`, lire l'input de l'utilisateur, executer la commande et recommencer.

La fonction `print_welcome_message` devient donc inutile et nous allons la supprimer et print le message de bienvenu avant la fonction `REPL` dans le `main` et, sachant que cette fonction est finalement le coeur de notre programme, nous allons la déplacer dans le fichier `enseash.c`.

En ce qui concerne la fonction `execute_command`, nous allons la déplacer dans utils.c, car elle n'est pas spécifique à notre shell.

Detail à noter, nous avons renommé la fonction `read_from_shell` en `read_shell` pour uniformiser les commandes de shell_utils.

Commençons par définir `REPL` :

```c title= enseash.c - REPL()
int REPL(){
    while(1){
        char input[MAX_INPUT_SIZE];
        
        print_shell(prompt_message);
        read_shell(input,MAX_INPUT_SIZE);

        int exit_code_cmd = execute_command(input);
        if(exit_code_cmd == EXIT_FAILURE){
            print_shell("La commande a échoué, réessayez\n");
        }
        
    }
}
```
*enseash.c - REPL()*

Nous avons également modifié la fonction `execute_command` en externalisant l'affichage des erreurs dans le REPL pour plus de modularité de clareté.

```c title= utils.c - execute_command()
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
        return WEXITSTATUS(status); // 1 if failed, 0 if success
    }
}
```
*utils.c - execute_command()*

Enfin, nous avons modifier le `main` pour appeler `REPL` après le message de bienvenue, supprimant par la même occasion la fonction `print_welcome_message`.

```c title= enseash.c - main()
int main(){
    print_shell(concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\'\n"));       
    return REPL();
}
```
*enseash.c - main()*

au final, nous avons les fichiers suivants :

```c title= enseash.c
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

int main(){
    print_shell(concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\'\n"));       
    return REPL();
}

int REPL(){
    while(1){
        char input[MAX_INPUT_SIZE];
        
        print_shell(prompt_message);
        read_shell(input,MAX_INPUT_SIZE);

        int exit_code_cmd = execute_command(input);
        if(exit_code_cmd == EXIT_FAILURE){
            print_shell("La commande a échoué, réessayez\n");
        }
        
    }
}
```
*enseash.c*

```c title= shell_utils.c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_utils.h"

ssize_t print_shell(char *message){
    return write(STDOUT_FILENO, message, strlen(message));
}

ssize_t read_shell(char *input, int max_input_size){
    ssize_t input_size = read(STDIN_FILENO,input,max_input_size);
    // ends correctly the string with a null character to avoid any buffer overflow
    input[input_size-1] = '\0';
    return input_size;
}
```
*shell_utils.c*

```c title= utils.h
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#include "utils.h"


char* concat_with_necessary_end_null(char* string, ...) {


    va_list args; // list of arguments
    va_start(args, string); // start to read the arguments : args by args readable in the variable called string 
    
    size_t total_length = strlen(string);
    char* current;
    while ((current = va_arg(args, char*)) != NULL) {
        total_length += strlen(current);
    }
    va_end(args); // end of the reading of the arguments

    char* result = (char*)malloc(total_length + 1);

    result[0] = '\0'; 
    strcat(result, string); // start filling the result with the first string

    va_start(args, string); // going again to throught the arguments
    while ((current = va_arg(args, char*)) != NULL) {
        strcat(result, current);
    }
    va_end(args);

    return result;
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
        return WEXITSTATUS(status); // 1 if failed, 0 if success
    }
}
```
*utils.c*

Et leur `*.h` respectifs.

```c title= enseash.h
int main();
int REPL();
```
*enseash.h*

```c title= shell_utils.h
#include <unistd.h>

ssize_t read_shell(char *input, int max_input_size);
ssize_t print_shell(char *message);
```
*shell_utils.h*

```c title= utils.h
char * concat_with_necessary_end_null(char * string, ...);
int execute_command(char *command);
```
*utils.h*

Les fichiers `functions.c` et `functions.h` ont été supprimés car ils sont vides.


## Question 3

### Gestion de la sortie de la sortie du shell par la commande 'exit'

Pour gérer la sortie du shell par la commande `'exit'`, nous allons ajouter une condition dans la fonction `REPL` qui vérifie si la commande saisie vaut bien `'exit'`. Si c'est le cas, la fonction `REPL` retourne `EXIT_SUCCESS` et le programme printera le `exit_message` (Bye bye...) avant de se terminer.

```c title= enseash.c - REPL()
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
```
*enseash.c - REPL()*

avec la definition de `exit_message` dans `enseash.c` :

```c title= enseash.c - exit_message
char exit_message[] = "Bye bye...\n";
```
*enseash.c - exit_message*
