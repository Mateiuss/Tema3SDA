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
    ((TD)root->info)->nume = malloc(5);
    if (!((TD)root->info)->nume) {
        free(root->info);
        free(root);
        return NULL;
    }
    strcpy(((TD)root->info)->nume, "root");

    return root;
}

void free_file(void *f)
{
    if (!f) {
        return;
    }
    free(((TF)f)->nume);
    free(f);
}

void free_cell(TArb cell, void(*free_type)(void*))
{
    free_type(cell->info);
    free(cell);
}

void free_file_tree(TArb root)
{
    if (!root) {
        return;
    }
    free_file(root->info);
    free_file_tree(root->st);
    free_file_tree(root->dr);
    free(root);
}

void free_dir(void *d)
{
    if (!d) {
        return;
    }
    free(((TD)d)->nume);
    free_file_tree(((TD)d)->file_root);
}

void free_root(TArb root)
{
    if (!root) {
        return;
    }
    free_dir(root->info);
    free_root(((TD)root->info)->dir_root);
    free_root(root->st);
    free_root(root->dr);
    free(root->info);
    free(root);
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
    file->nume = malloc(strlen(name) + 1);
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
    dir->nume = malloc(strlen(name) + 1);
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

TArb give_file(TArb a, TArb *parent, char *name)
{
    while (a && strcmp(((TF)a->info)->nume, name) != 0) {
        *parent = a;

        if (strcmp(name, ((TF)a->info)->nume) < 0) {
            a = a->st;
        } else {
            a = a->dr;
        }
    }

    return a;
}

void rm(TArb curr_dir, char *name)
{
    if (!search_file(((TD)curr_dir->info)->file_root, name)) {
        printf("File %s doesn't exist!\n", name);
        return;
    }

    TArb parent = NULL;
    TArb root = ((TD)curr_dir->info)->file_root;
    TArb delete = give_file(root, &parent, name);

    if (delete->st == NULL && delete->dr == NULL) {
        if (delete != root) {
            if (parent->st == delete) {
                parent->st = NULL;
            } else {
                parent->dr = NULL;
            }
        } else {
            ((TD)curr_dir->info)->file_root = NULL;
        }

        free_cell(delete, free_file);
    } else if (delete->st && delete->dr) {
        TArb min = delete->dr;
        TArb min_parent = delete;
        while (min->st) {
            min_parent = min;
            min = min->st;
        }

        void *info = delete->info;
        delete->info = min->info;
        min->info = info;

        if (min_parent->st == min) {
            min_parent->st = NULL;
        } else {
            min_parent->dr = NULL;
        }
        
        free_cell(min, free_file);
    } else {
        if (root == delete) {
            if (delete->st) {
                ((TD)curr_dir->info)->file_root = delete->st;
            } else {
                ((TD)curr_dir->info)->file_root = delete->dr;
            }
            free_cell(delete, free_file);
        } else {
            if (delete->st) {
                if (parent->st == delete) {
                    parent->st = delete->st;
                } else {
                    parent->dr = delete->st;
                }
                free_cell(delete, free_file);
            } else {
                if (parent->st == delete) {
                    parent->st = delete->dr;
                } else {
                    parent->dr = delete->dr;
                }
                free_cell(delete, free_file);
            }
        }
    }
}

TArb give_dir(TArb a, TArb *parent, char *name)
{
    while (a && strcmp(((TD)a->info)->nume, name) != 0) {
        *parent = a;

        if (strcmp(name, ((TD)a->info)->nume) < 0) {
            a = a->st;
        } else {
            a = a->dr;
        }
    }

    return a;
}

void free_dir_root(TArb dir)
{
    free_root(((TD)dir->info)->dir_root);
    free_dir(dir->info);
    free(dir->info);
    free(dir);
}

void rmdir(TArb curr_dir, char *name)
{
    if (!search_dir(((TD)curr_dir->info)->dir_root, name)) {
        printf("Directory %s doesn't exist!\n", name);
        return;
    }

    TArb parent = NULL;
    TArb root = ((TD)curr_dir->info)->dir_root;
    TArb delete = give_dir(root, &parent, name);

    if (delete->st == NULL && delete->dr == NULL) {
        if (delete != root) {
            if (parent->st == delete) {
                parent->st = NULL;
            } else {
                parent->dr = NULL;
            }
        } else {
            ((TD)curr_dir->info)->dir_root = NULL;
        }

        free_dir_root(delete);
    } else if (delete->st && delete->dr) {
        TArb min = delete->dr;
        TArb min_parent = delete;
        while (min->st) {
            min_parent = min;
            min = min->st;
        }

        void *info = delete->info;
        delete->info = min->info;
        min->info = info;

        if (min_parent->st == min) {
            min_parent->st = NULL;
        } else {
            min_parent->dr = NULL;
        }
        
        free_dir_root(min);
    } else {
        if (root == delete) {
            if (delete->st) {
                ((TD)curr_dir->info)->dir_root = delete->st;
            } else {
                ((TD)curr_dir->info)->dir_root = delete->dr;
            }
            free_dir_root(delete);
        } else {
            if (delete->st) {
                if (parent->st == delete) {
                    parent->st = delete->st;
                } else {
                    parent->dr = delete->st;
                }
                free_dir_root(delete);
            } else {
                if (parent->st == delete) {
                    parent->st = delete->dr;
                } else {
                    parent->dr = delete->dr;
                }
                free_dir_root(delete);
            }
        }
    }
}

void cd(TArb *curr_dir, char *name)
{
    if (strcmp(name, "..") == 0) {
        *curr_dir = ((TD)(*curr_dir)->info)->parent;
        return;
    } else if (!search_dir(((TD)(*curr_dir)->info)->dir_root, name)) {
        printf("Directory not found!\n");
        return;
    }

    TArb parent = NULL;
    *curr_dir = give_dir(((TD)(*curr_dir)->info)->dir_root, &parent, name);
}

void pwd(TArb curr_dir)
{
    if (!curr_dir) {
        return;
    }
    pwd(((TD)curr_dir->info)->parent);
    printf("/%s", ((TD)curr_dir->info)->nume);
}
