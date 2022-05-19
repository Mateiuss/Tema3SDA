/* DUDU Matei-Ioan 313CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME(a) ((TF)(a)->info)->nume
#define FILE_PARENT(a) ((TF)(a)->info)->parent
#define DIR_NAME(a) ((TD)(a)->info)->nume
#define DIR_DIRROOT(a) ((TD)(a)->info)->dir_root
#define DIR_FILEROOT(a) ((TD)(a)->info)->file_root
#define DIR_PARENT(a) ((TD)(a)->info)->parent
#define MAX 51

// Definitiile structurilor folosite:
// * Structura pentru director
typedef struct director {
    char *nume;
    void *parent;
    void *file_root;
    void *dir_root;
} TDir, *TD;

// * Structura pentru fisier
typedef struct fisier {
    char *nume;
    void *parent;
} TFile, *TF;

// * Structura pentru arborele binar de cautare
typedef struct arbore {
    void *info;
    struct arbore *st;
    struct arbore *dr;
} TCel, *TArb;

// Prototipurile functiilor din functions.c pe care le-am apelat in main.c:
TArb init_root();
int touch(TArb curr_dir, char *name);
void ls(TArb curr_dir);
int mkdir(TArb curr_dir, char *name);
void rm(TArb curr_dir, char *name);
void rmdir(TArb curr_dir, char *name);
void cd(TArb *curr_dir, char *name);
void pwd(TArb curr_dir);
void find_f(TArb curr_dir, char *name);
void find_d(TArb curr_dir, char *name);
void free_root(TArb root);
