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

ssize_t read_from_shell(char *input){
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

ssize_t read_from_shell(char *input);
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
    read_from_shell(input,MAX_INPUT_SIZE);
}
```

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
    read_from_shell(input,MAX_INPUT_SIZE);
}
```