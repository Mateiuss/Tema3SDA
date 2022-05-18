#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct director {
    char *nume;
    void *parent;     // TArb
    void *file_root;  // TArb
    void *dir_root;   // TArb
} TDir, *TD;

typedef struct fisier {
    char *nume;
    void *parent;
} TFile, *TF;

typedef struct arbore {
    void *info;
    struct arbore *st;
    struct arbore *dr;
} TCel, *TArb;

TArb init_root();
int touch(TArb curr_dir, char *name);
void ls(TArb curr_dir);
void free_root(TArb root);
int mkdir(TArb curr_dir, char *name);
void rm(TArb curr_dir, char *name);
void rmdir(TArb curr_dir, char *name);
void cd(TArb *curr_dir, char *name);
void pwd(TArb curr_dir);
void find_f(TArb curr_dir, char *name);
void find_d(TArb curr_dir, char *name);
