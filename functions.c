/* DUDU Matei-Ioan 313CB */
#include "structures.h"

// Functie de initializare a fisierului root
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
    DIR_NAME(root) = malloc(5);
    if (!DIR_NAME(root)) {
        free(root->info);
        free(root);
        return NULL;
    }
    strcpy(DIR_NAME(root), "root");

    return root;
}

// Functie care initializeaza un nod de arbore, care primeste ca parametru
// informatia deja alocata
TArb init_cell(void *info)
{
    TArb new_cell = calloc(1, sizeof(TCel));
    if (!new_cell) {
        return NULL;
    }
    new_cell->info = info;

    return new_cell;
}

// Functie care initializeaza un fisier
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

// Functie care initializeaza un director
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

// Functie care elibereaza memoria alocata pentru un fisier
void free_file(void *f)
{
    if (!f) {
        return;
    }
    free(((TF)f)->nume);
    free(f);
}

// Functie care elibereaza memoria alocata pentru un nod de arbore. Aceasta
// primeste ca parametrii nodul si pointer la o functie de eliberare a
// informatiei
void free_cell(TArb cell, void(*free_type)(void*))
{
    free_type(cell->info);
    free(cell);
}

// Functie de eliberare a memoriei pentru un arbore de fisiere
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

// Functie care elibereaza memoria unui director
void free_dir(void *d)
{
    if (!d) {
        return;
    }
    free(((TD)d)->nume);
    free_file_tree(((TD)d)->file_root);
}

// Functie de eliberare a unei intregi ierarhii de fisiere. Folosita pentru
// a elibera memoria alocata de root si de tot ce contine acesta
void free_root(TArb root)
{
    if (!root) {
        return;
    }
    free_dir(root->info);
    free_root(DIR_DIRROOT(root));
    free_root(root->st);
    free_root(root->dr);
    free(root->info);
    free(root);
}

// Functie care cauta un fisier dupa nume intr-un arbore de fisiere si intoarce
// 1 daca a fost gasit si 0 altfel
int search_file(TArb a, char *name) {
    if (!a) {
        return 0;
    }
    if (strcmp(FILE_NAME(a), name) == 0) {
        return 1;
    }
    if (strcmp(FILE_NAME(a), name) > 0) {
        return search_file(a->st, name);
    }
    return search_file(a->dr, name);
}

// Functie care face acelasi lucru ca functia precedenta, doar ca pentru
// directoare
int search_dir(TArb a, char *name)
{
    if (!a) {
        return 0;
    }
    if (strcmp(DIR_NAME(a), name) == 0) {
        return 1;
    }
    if (strcmp(DIR_NAME(a), name) > 0) {
        return search_file(a->st, name);
    }
    return search_file(a->dr, name);
}

// Functie care adauga un nod nou in arborele de fisiere
void add_in_file_tree(TArb a, TArb cell) {
    if (strcmp(FILE_NAME(a), FILE_NAME(cell)) > 0) {
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

// Functie care adauga un nod nou in arborele de directoare
void add_in_dir_tree(TArb a, TArb cell) {
    if (strcmp(DIR_NAME(a), DIR_NAME(cell)) > 0) {
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

// Functia touch care creeaza un fisier in directorul curent, in cazul in care
// nu exista un director sau un fisier cu acelasi nume
int touch(TArb curr_dir, char *name)
{
    // Verific daca exista deja un fisier/director cu numele dat ca parametru
    if (search_file(DIR_FILEROOT(curr_dir), name)) {
        printf("File %s already exists!\n", name);
        return 1;
    } 

    if (search_dir(DIR_DIRROOT(curr_dir), name)) {
        printf("Directory %s already exists!\n", name);
        return 1;
    }

    // Creez un fisier cu numele dat
    TF file = init_file(name);
    if (!file) {
        return 0;
    }
    // Salvez in fisier faptul ca directorul curent este parintele acestuia
    file->parent = curr_dir;

    // Creez un nod ce contine fisierul
    TArb new_cell = init_cell(file);
    if (!new_cell) {
        free(file);
        return 0;
    }

    // Daca arborele de fisiere este gol, setez noul nod ca radacina
    if (!DIR_FILEROOT(curr_dir)) {
        DIR_FILEROOT(curr_dir) = new_cell;
        return 1;
    }

    // Altfel, adaug in arborele binar de cautare pentru fisiere pe fisierul
    // nou creat
    add_in_file_tree(DIR_FILEROOT(curr_dir), new_cell);

    return 1;
}

// Pentru functia mkdir este fix acelasi proces ca pentru touch, doar ca se 
// lucreaza cu directoare in loc de fisiere
int mkdir(TArb curr_dir, char *name)
{
    if (search_file(DIR_FILEROOT(curr_dir), name)) {
        printf("File %s already exists!\n", name);
        return 1;
    } 

    if (search_dir(DIR_DIRROOT(curr_dir), name)) {
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

    if (!DIR_DIRROOT(curr_dir)) {
        DIR_DIRROOT(curr_dir) = new_cell;
        return 1;
    }

    add_in_dir_tree(DIR_DIRROOT(curr_dir), new_cell);

    return 1;
}

// Functie care printeaza continutul arborelui de directoare in ordinea SRD
void print_dir(TArb a)
{
    if (!a) {
        return;
    }

    print_dir(a->st);
    printf("%s ", DIR_NAME(a));
    print_dir(a->dr);
}

// Functie care printeaza continutul arborelui de fisiere in ordinea SRD
void print_file(TArb a)
{
    if (!a) {
        return;
    }

    print_dir(a->st);
    printf("%s ", FILE_NAME(a));
    print_dir(a->dr);
}

// Functie care afiseaza atat continutul arborelui binar de cautare pentru
// fisiere, cat si pentru cel de directoare
void ls(TArb curr_dir)
{
    print_dir(DIR_DIRROOT(curr_dir));
    print_file(DIR_FILEROOT(curr_dir));
    printf("\n");
}

// Functie care returneaza adresa elementului cautat si modifica
// adresa lui parent sa pointeze catre parintele elementului (pentru fisiere)
TArb give_file(TArb file, TArb *parent, char *name)
{
    while (file && strcmp(FILE_NAME(file), name) != 0) {
        *parent = file;

        if (strcmp(name, FILE_NAME(file)) < 0) {
            file = file->st;
        } else {
            file = file->dr;
        }
    }

    return file;
}

// Functia rm. Sterge elementul cu numele name (in cazul in care acesta exista)
void rm(TArb curr_dir, char *name)
{
    // Caut fisierul denumit name in arborele de fisiere, iar adaca acesta nu 
    // exista inchei executia functiei 
    if (!search_file(DIR_FILEROOT(curr_dir), name)) {
        printf("File %s doesn't exist!\n", name);
        return;
    }

    // Initializez parent cu NULL ca sa acopere situatia in care nodul care
    // trebuie sters este radacina
    TArb parent = NULL;
    // root este radacina arborelui de fisiere
    TArb root = DIR_FILEROOT(curr_dir);
    // delete este nodul care trebuie sters
    TArb delete = give_file(root, &parent, name);

    // Acest algoritm se bazeaza pe trei cazuri:
    // 1) Nodul care trebuie sters este o frunza(st si dr = NULL
    // 2) Nodul care trebuie sters are doi copii
    // 3) Nodul care trebuie sters are un singur copil
    if (delete->st == NULL && delete->dr == NULL) { // 1)
        // Daca nodul care trebuie sters nu e radacina atunci tai legatura 
        // dintre parinte si nod
        if (delete != root) {
            if (parent->st == delete) {
                parent->st = NULL;
            } else {
                parent->dr = NULL;
            }
        } else {
            // Altfel, pointerul care pointeaza catre arborele de fisiere este
            // egalat cu NULL
            DIR_FILEROOT(curr_dir) = NULL;
        }

        // Eliberez memoria alocata pentru nodul care a fost eliminat din
        // arbore
        free_cell(delete, free_file);
    } else if (delete->st && delete->dr) { // 2)
        // Caut cel mai mic nod de pe partea dreapta a nodului care trebuie 
        // sa fie sters si salvez si parintele acestuia
        TArb min = delete->dr;
        TArb min_parent = delete;
        while (min->st) {
            min_parent = min;
            min = min->st;
        }

        // Inversez informatiile continute de nodul care trebui sters si
        // minimul precizat anterior
        void *info = delete->info;
        delete->info = min->info;
        min->info = info;

        // Acum, tinand cont de faptul ca nodul minim contine informatia
        // nodului ce trebuie sters, pot sa elimin nodul minin ca pe nodul
        // din cazul anterior
        if (min_parent->st == min) {
            min_parent->st = NULL;
        } else {
            min_parent->dr = NULL;
        }
        
        // Eliberez memoria alocata nodului
        free_cell(min, free_file);
    } else { // 3)
        // Verific daca nodul ce trebuie sters este nodul radacina sau nu
        if (root == delete) {
            // Verific daca fiul pe care il are nodul este cel stang sau cel 
            // drept si il fac sa devina noua radacina
            if (delete->st) {
                DIR_FILEROOT(curr_dir) = delete->st;
            } else {
                DIR_FILEROOT(curr_dir) = delete->dr;
            }

            // Eliberez memoria alocata nodului de sters
            free_cell(delete, free_file);
        } else {
            // Verific daca fiul pe care il are nodul este cel stang sau cel
            // drept. Mai departe, fiul nodului care trebuie sters ocupa locul
            // parintelui sau
            if (delete->st) {
                if (parent->st == delete) {
                    parent->st = delete->st;
                } else {
                    parent->dr = delete->st;
                }
            } else {
                if (parent->st == delete) {
                    parent->st = delete->dr;
                } else {
                    parent->dr = delete->dr;
                }
            }

            // Eliberez memoria alocata nodului care trebuie sters
            free_cell(delete, free_file);
        }
    }
}

// Functie care returneaza adresa elementului cautat si modifica
// adresa lui parent sa pointeze catre parintele elementului (pentru directoare)
TArb give_dir(TArb a, TArb *parent, char *name)
{
    while (a && strcmp(DIR_NAME(a), name) != 0) {
        *parent = a;

        if (strcmp(name, DIR_NAME(a)) < 0) {
            a = a->st;
        } else {
            a = a->dr;
        }
    }

    return a;
}

// Functie care elibereaza memoria unui director si a nodului care il contine
void free_dir_root(TArb dir)
{
    free_root(DIR_DIRROOT(dir));
    free_dir(dir->info);
    free(dir->info);
    free(dir);
}

// Functia echivalenta a functiei "rm", doar ca pentru directoare. Nu o sa mai
// trec prin pasii acestui algoritm pentru ca sunt la fel cu cei ai functiei
// mentionate anterior
void rmdir(TArb curr_dir, char *name)
{
    if (!search_dir(DIR_DIRROOT(curr_dir), name)) {
        printf("Directory %s doesn't exist!\n", name);
        return;
    }

    TArb parent = NULL;
    TArb root = DIR_DIRROOT(curr_dir);
    TArb delete = give_dir(root, &parent, name);

    if (delete->st == NULL && delete->dr == NULL) {
        if (delete != root) {
            if (parent->st == delete) {
                parent->st = NULL;
            } else {
                parent->dr = NULL;
            }
        } else {
            DIR_DIRROOT(curr_dir) = NULL;
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
                DIR_DIRROOT(curr_dir) = delete->st;
            } else {
                DIR_DIRROOT(curr_dir) = delete->dr;
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

// Functia asociata instructiunii cd
void cd(TArb *curr_dir, char *name)
{
    // Verific daca instructiunea este ".."
    if (strcmp(name, "..") == 0) {
        // Daca directorul curent este root-ul, atunci nu il mai modific
        if (DIR_PARENT(*curr_dir) == NULL) {
            return;
        }
        // Altfel, directorul va pointa catre parintele sau
        *curr_dir = DIR_PARENT(*curr_dir);
        return;
    } else if (!search_dir(DIR_DIRROOT(*curr_dir), name)) {
        // Daca este alta instructiune in afara de "..", se cauta directorul
        // cu numele dat si daca nu este gasit, se incheie apelul functiei
        printf("Directory not found!\n");
        return;
    }

    TArb parent = NULL;
    // Directorul curent devine directorul pe care l-am cautat si gasit
    *curr_dir = give_dir(DIR_DIRROOT(*curr_dir), &parent, name);
}

// Functia asociata instructiunii pwd
void pwd(TArb curr_dir)
{
    if (!curr_dir) {
        return;
    }
    // Afisez in ordine inversa directorul curent, dupa care trec la parintele
    // sau
    pwd(DIR_PARENT(curr_dir));
    printf("/%s", DIR_NAME(curr_dir));
}

// Functie care cauta un fisier dupa nume si returneaza adresa acestuia. In 
// cazul in care nu s-a gasit, returneaza NULL
TArb search_in_file_tree(TArb file, char *name)
{
    if (!file) {
        return NULL;
    }
    if (strcmp(FILE_NAME(file), name) == 0) {
        return FILE_PARENT(file);
    }
    if (strcmp(FILE_NAME(file), name) > 0) {
        return search_in_file_tree(file->st, name);
    }
    return search_in_file_tree(file->dr, name);
}

// Functie care cauta recursiv in fiecare arbore de fisiere al fiecarui director
// fisierul cu un nume dat
void search_in_dir_tree(TArb dir, TArb *found, char *name)
{
    if (!dir) {
        return;
    }
    TArb rez = search_in_file_tree(DIR_FILEROOT(dir), name);
    if (rez) {
        *found = rez;
        return;
    }
    search_in_dir_tree(dir->st, found, name);
    search_in_dir_tree(dir->dr, found, name);
    search_in_dir_tree(DIR_DIRROOT(dir), found, name);
}

// Functia specifica instructiunii "find -f"
void find_f(TArb curr_dir, char *name)
{
    TArb search = NULL;
    // Caut fisierul cu numele "name"
    search_in_dir_tree(curr_dir, &search, name);
    // Daca a fost gasit, afisez locatia acestuia
    if (search) {
        printf("File %s found!\n", name);
        pwd(search);
        printf("\n");
        return;
    }

    // Altfel, afisez ca nu a fost gasit
    printf("File %s not found!\n", name);
}

// Functie care cauta recursiv in fiecare arbore de directoare al fiecarui
// director pe directorul cu numele "name"
void search_dir_only(TArb a, TArb *found, char *name)
{
    if (!a) {
        return;
    }
    if (strcmp(DIR_NAME(a), name) == 0) {
        *found = a;
        return;
    }
    search_dir_only(a->st, found, name);
    search_dir_only(a->dr, found, name);
    search_dir_only(DIR_DIRROOT(a), found, name);
}

// Functia specifica instructiunii "find -d"
void find_d(TArb curr_dir, char *name)
{
    TArb rez = NULL;
    // Caut directorul cu numele "name"
    search_dir_only(DIR_DIRROOT(curr_dir), &rez, name);
    // Daca nu l-am gasit, afisez acest rezultat si inchei apelul functiei
    if (!rez) {
        printf("Directory %s not found!\n", name);
        return;
    }

    // Daca l-am gasit, afisez calea catre acesta si faptul ca l-am gasit
    printf("Directory %s found!\n", name);
    pwd(rez);
    printf("\n");
}
