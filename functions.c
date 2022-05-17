#include "structures.h"

TArb init_root()
{
    TArb root = calloc(1, sizeof(TCel));
    if (!root) {
        return NULL;
    }
    root->info = calloc(1, sizeof(TDir));
    if (!root->info) {
        free(root);
        return NULL;
    }
    ((TD)root->info)->nume = malloc(sizeof(5));
    if (!((TD)root->info)->nume) {
        free(root->info);
        free(root);
        return NULL;
    }
    strcpy(((TD)root->info)->nume, "root");

    return root;
}

void free_root(TArb *root)
{
    // TODO
}

TArb init_cell(void *info)
{
    TArb new_cell = calloc(1, sizeof(TCel));
    if (!new_cell) {
        return NULL;
    }
    new_cell->info = info;

    return new_cell;
}

TF init_file(char *name)
{
    TF file = malloc(sizeof(TFile));
    if (!file) {
        return NULL;
    }
    file->nume = malloc(strlen(name));
    if (!file->nume) {
        free(file);
        return NULL;
    }
    strcpy(file->nume, name);

    return file;
}

TD init_dir(char *name)
{
    TD dir = malloc(sizeof(TDir));
    if (!dir) {
        return NULL;
    }
    dir->nume = malloc(strlen(name));
    if (!dir->nume) {
        free(dir);
        return NULL;
    }
    strcpy(dir->nume, name);
    dir->dir_root = dir->file_root = NULL;

    return dir;
}

int search_file(TArb a, char *name) {
    if (!a) {
        return 0;
    }
    if (strcmp(((TF)a->info)->nume, name) == 0) {
        return 1;
    }
    if (strcmp(((TF)a->info)->nume, name) > 0) {
        return search_file(a->st, name);
    }
    return search_file(a->dr, name);
}

int search_dir(TArb a, char *name)
{
    if (!a) {
        return 0;
    }
    if (strcmp(((TD)a->info)->nume, name) == 0) {
        return 1;
    }
    if (strcmp(((TD)a->info)->nume, name) > 0) {
        return search_file(a->st, name);
    }
    return search_file(a->dr, name);
}

void add_in_file_tree(TArb a, TArb cell) {
    if (strcmp(((TF)a->info)->nume, ((TF)cell->info)->nume) > 0) {
        if (!a->st) {
            a->st = cell;
        } else {
            add_in_file_tree(a->st, cell);
        }
    } else {
        if (!a->dr) {
            a->dr = cell;
        } else {
            add_in_file_tree(a->dr, cell);
        }
    }
}

void add_in_dir_tree(TArb a, TArb cell) {
    if (strcmp(((TD)a->info)->nume, ((TD)cell->info)->nume) > 0) {
        if (!a->st) {
            a->st = cell;
        } else {
            add_in_dir_tree(a->st, cell);
        }
    } else {
        if (!a->dr) {
            a->dr = cell;
        } else {
            add_in_dir_tree(a->dr, cell);
        }
    }
}

int touch(TArb curr_dir, char *name)
{
    if (search_file(((TD)curr_dir->info)->file_root, name)) {
        printf("File %s already exists!\n", name);
        return 1;
    } 

    if (search_dir(((TD)curr_dir->info)->dir_root, name)) {
        printf("Directory %s already exists!\n", name);
        return 1;
    }

    TF file = init_file(name);
    if (!file) {
        return 0;
    }
    file->parent = curr_dir;
    TArb new_cell = init_cell(file);
    if (!new_cell) {
        free(file);
        return 0;
    }

    if (!((TD)curr_dir->info)->file_root) {
        ((TD)curr_dir->info)->file_root = new_cell;
        return 1;
    }

    add_in_file_tree(((TD)curr_dir->info)->file_root, new_cell);

    return 1;
}

int mkdir(TArb curr_dir, char *name)
{
    if (search_file(((TD)curr_dir->info)->file_root, name)) {
        printf("File %s already exists!\n", name);
        return 1;
    } 

    if (search_dir(((TD)curr_dir->info)->dir_root, name)) {
        printf("Directory %s already exists!\n", name);
        return 1;
    }

    TD dir = init_dir(name);
    if(!dir) {
        return 0;
    }
    dir->parent = curr_dir;
    TArb new_cell = init_cell(dir);
    if (!new_cell) {
        free(dir);
        return 0;
    }

    if (!((TD)curr_dir->info)->dir_root) {
        ((TD)curr_dir->info)->dir_root = new_cell;
        return 1;
    }

    add_in_dir_tree(((TD)curr_dir->info)->dir_root, new_cell);

    return 1;
}

void print_dir(TArb a)
{
    if (!a) {
        return;
    }

    print_dir(a->st);
    printf("%s ", ((TD)a->info)->nume);
    print_dir(a->dr);
}

void print_file(TArb a)
{
    if (!a) {
        return;
    }

    print_dir(a->st);
    printf("%s ", ((TF)a->info)->nume);
    print_dir(a->dr);
}

void ls(TArb curr_dir)
{
    print_dir(((TD)curr_dir->info)->dir_root);
    print_file(((TD)curr_dir->info)->file_root);
    printf("\n");
}

void rm(TArb curr_dir, char *name)
{
    if (!search_file(((TD)curr_dir->info)->file_root, name)) {
        printf("File %s doesn't exist!\n", name);
        return;
    }
}