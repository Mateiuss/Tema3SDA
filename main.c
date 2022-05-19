/* DUDU Matei-Ioan 313CB */
#include "structures.h"

int main()
{
    // Initializez directorul root al ierarhiei de fisiere
    TArb root = init_root();
    if (!root) {
        printf("Nu s-a putut aloca memorie pentru root!\n");
        return 1;
    }

    // curr_dirr - pointer la fisierul curent in care se afla programul
    TArb curr_dir = root;

    char *read_line = malloc(MAX);
    if (!read_line) {
        free_root(root);
        return 1;
    }
    while (fgets(read_line, 51, stdin)) {
        // Inlocuiesc caracterul '\n' cu '\0' de la sfarsitul liniei citite
        read_line[strcspn(read_line, "\n")] = 0;
        
        // Identific instructiunea introdusa
        // Pentru functiile touch si mkdir verific si succesul operatiei,
        // deoarece este posibil sa fie alocari care nu au reusit
        if (strstr(read_line, "touch")) {           // touch
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            int success = touch(curr_dir, parser);
            if (!success) {
                free_root(root);
                free(read_line);
                return 1;
            }
        } else if (strstr(read_line, "mkdir")) {   // mkdir
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            int success = mkdir(curr_dir, parser);
            if (!success) {
                free_root(root);
                free(read_line);
                return 1;
            }
        } else if (strstr(read_line, "ls")) {      // ls
            ls(curr_dir);
        } else if (strstr(read_line, "rmdir")) {   // rmdir
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            rmdir(curr_dir, parser);
        } else if (strstr(read_line, "rm")) {      // rm
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            rm(curr_dir, parser);
        } else if (strstr(read_line, "cd")) {      // cd
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            cd(&curr_dir, parser);
        } else if (strstr(read_line, "pwd")) {     // pwd
            pwd(curr_dir);
            printf("\n");
        } else if (strstr(read_line, "find -f")) { // find -f
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");
            parser = strtok(NULL, " ");

            find_f(root, parser);
        } else if (strstr(read_line, "find -d")) { // find -d
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");
            parser = strtok(NULL, " ");

            find_d(root, parser);
        } else if (strstr(read_line, "quit")) {    // quit
            break;
        }
    }

    // Eliberez memoria alocata
    free_root(root);
    free(read_line);

    return 0;
}