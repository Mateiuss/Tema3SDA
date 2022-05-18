#include "structures.h"

int main()
{
    TArb root = init_root();
    TArb curr_dir = root;
    if (!root) {
        printf("Nu s-a putut aloca memorie pentru root!\n");
        return 1;
    }

    char read_line[51];
    while (fgets(read_line, 51, stdin)) {
        read_line[strcspn(read_line, "\n")] = 0;
        
        if (strstr(read_line, "touch")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            int success = touch(curr_dir, parser);
            if (!success) {
                free_root(root);
                root = NULL;
                curr_dir = NULL;
                return 1;
            }
        } else if (strstr(read_line, "ls")) {
            ls(curr_dir);
        } else if (strstr(read_line, "mkdir")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            int success = mkdir(curr_dir, parser);
            if (!success) {
                free_root(root);
                root = NULL;
                curr_dir = NULL;
                return 1;
            }
        } else if (strstr(read_line, "rmdir")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            rmdir(curr_dir, parser);
        } else if (strstr(read_line, "rm")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            rm(curr_dir, parser);
        } else if (strstr(read_line, "cd")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");

            cd(&curr_dir, parser);
        } else if (strstr(read_line, "pwd")) {
            pwd(curr_dir);
            printf("\n");
        } else if (strstr(read_line, "find -f")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");
            parser = strtok(NULL, " ");

            find_f(curr_dir, parser);
        } else if (strstr(read_line, "find -d")) {
            char *parser = strtok(read_line, " ");
            parser = strtok(NULL, " ");
            parser = strtok(NULL, " ");

            find_d(curr_dir, parser);
        } else if (strstr(read_line, "quit")) {
            break;
        }
    }

    free_root(root);
    root = NULL;

    return 0;
}