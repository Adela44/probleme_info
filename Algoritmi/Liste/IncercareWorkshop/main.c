#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Exemplu retinere valori:
 *
 *         14(zi) - 6 (id)
 *           |
 * W001 -> 12(zi) - 7 (id) - 8 (id)
 *   |
 * W002 -> 13(zi) - 10 (id)
 *
 */

typedef struct IDParticipant {
    struct IDParticipant *next;
    int id;
}IDParticipant;

typedef struct ZiWorkShop {
    struct ZiWorkShop *next;
    IDParticipant *ID;
    int zi;
}ZiWorkShop;

typedef struct CodWorkshop{
    char cod[50];
    struct CodWorkshop* next;
    ZiWorkShop *z;
} CodWorkshop;

void eliberare_cod(CodWorkshop *head);
void eliberare_zi(ZiWorkShop *z);
void eliberare_id(IDParticipant *i);

/*
 Avem nevoie de gaseste_zi (returneaza *z) si exista_id(1 daca exista, 0 daca nu)
 pentru a verifica ca valoarea acestora nu e deja adaugata
 */


ZiWorkShop* gaseste_zi(ZiWorkShop *z, int zi) {
    while (z != NULL) {
        if (z->zi == zi)
            return z;
        z = z->next;
    }
    return NULL;
}
int exista_id(IDParticipant *idp, int id) {
    while (idp != NULL) {
        if (idp->id == id)
            return 1;
        idp = idp->next;
    }
    return 0;
}


void adauga_id(ZiWorkShop *zi, int id) {
    if (!exista_id(zi->ID, id)) { //adaugam id-ul doar daca acesta nu exista inca
        IDParticipant *p = zi->ID;
        while (p->next != NULL)
            p = p->next; //mergem la capatul listei cu resp cu id-uri, unde vom aloca memorie si crea noul id

        p->next = malloc(sizeof(IDParticipant));
        p->next->id = id;
        p->next->next = NULL;
    }
}

int sterge_id(IDParticipant **head, int id) {
    IDParticipant *p = *head, *ant = NULL;

    while (p && p->id != id) { //cautam pana dam de id-ul cerut, salvam pe parcurs si anteriorul
        ant = p;
        p = p->next;
    }
    if (!p) return 0; //daca nu gasit id ul, nu stergem nimic

    if (!ant) //daca avem doar un element
        *head = p->next;
    else //altfel
        ant->next = p->next;

    free(p);
    return 1;
}

int sterge_zi(ZiWorkShop **head, int zi, int id) {
    ZiWorkShop *p = *head, *ant = NULL;

    while (p != NULL && p->zi != zi) { //cautam zi
        ant = p;
        p = p->next;
    }
    if (!p) return 0; //daca nu exista ziua, nu facem nimic

    if (!sterge_id(&p->ID, id)) //daca id ul nu exista, nu stergem nimic
        return 0;

    if (p->ID == NULL) { //daca deja am sters id-ul
        if (!ant) //daca avem doar un elem
            *head = p->next;
        else //altfel
            ant->next = p->next;

        free(p);
    }
    return 1;
}

void stergere(CodWorkshop **head, char *cod, int zi, int id) {
    CodWorkshop *p = *head, *ant = NULL;

    while (p && strcmp(p->cod, cod) != 0) { //cautam pana gasim codul
        ant = p;
        p = p->next;
    }
    if (!p) return; //daca codul nu exista, nu facem nimic

    sterge_zi(&p->z, zi, id); //stergem ziua

    if (p->z == NULL) { //daca am sters ziua
        if (!ant)
            *head = p->next;
        else
            ant->next = p->next;

        free(p);
    }
}

void adaugare(CodWorkshop **head, char cod[50], int zi, int id) {
    CodWorkshop *p = *head;
    CodWorkshop *last = NULL;
    ZiWorkShop *noua_zi = NULL;

    // căutăm codul
    while (p != NULL) {
        if (strcmp(p->cod, cod) == 0) { //daca codul exista, cautam ziua

            ZiWorkShop *z = gaseste_zi(p->z, zi);

            if (z != NULL) { //daca ziua exista, adaugam id
                adauga_id(z, id);
            } else {
                // ziua nu exista -> o cream
                noua_zi = malloc(sizeof(ZiWorkShop));
                noua_zi->zi = zi;
                noua_zi->next = NULL;

                noua_zi->ID = malloc(sizeof(IDParticipant));
                noua_zi->ID->id = id;
                noua_zi->ID->next = NULL;

                // o legam la final
                z = p->z;
                while (z->next != NULL)
                    z = z->next;
                z->next = noua_zi;
                z->next->ID = noua_zi->ID;
            }
            return;
        }
        last = p;
        p = p->next;
    }

    // daca nu exista cod-ul
    CodWorkshop *nou = malloc(sizeof(CodWorkshop));
    strcpy(nou->cod, cod); // cod
    nou->next = NULL;

    nou->z = malloc(sizeof(ZiWorkShop)); //zi
    nou->z->zi = zi;
    nou->z->next = NULL;

    nou->z->ID = malloc(sizeof(IDParticipant)); //id
    nou->z->ID->id = id;
    nou->z->ID->next = NULL;

    if (*head == NULL) //daca nu avem niciun cod, cel nou cread e primul
        *head = nou;
    else //daca avem coduri in lista, atunci cel nou creat va fi trecut la final
        last->next = nou;
}

void printID(IDParticipant *i) {
    while (i != NULL) {
        printf(" %d ", i->id);
        i = i->next;
    }
}

void printZi(ZiWorkShop *z) {
    while (z != NULL) {
        printf(" zi: %d si id: ", z->zi);
        printID(z->ID);
        z = z->next;
    }
}


void printCod(CodWorkshop *c) {
    int cnt = 0;
    while (c != NULL) {
        printf("%d: %s", cnt, c->cod);
        printZi(c->z);
        printf("\n");
        c = c->next;
        cnt++;
    }
}

void eliberare_id(IDParticipant *i) {
    while (i != NULL) {
        IDParticipant *aux = i;
        i = i -> next;
        free(aux);
    }
}

void eliberare_zi(ZiWorkShop *z) {
    while (z != NULL) {
        ZiWorkShop *aux = z;
        z = z->next;
        eliberare_id(aux->ID);
        free(aux);
    }
}

void eliberare_cod(CodWorkshop *head) {
    while (head != NULL) {
        CodWorkshop *p = head;
        head = head->next;
        eliberare_zi(p->z);
        free(p);
    }
}


int main(int argc, const char * argv[]) {
    if (argc != 2) {
        printf("Nr incorect de argumente\n");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Eroare la deschiderea fisierului\n");
        exit(1);
    }
    CodWorkshop *head = NULL;
    int nr_op, id, zi;
    char cod[50];
    char cod_final[50];
    fscanf(f, "%d", &nr_op);
    for (int i = 0; i < nr_op; i++) {
        int n = fscanf(f, "%49s %d %d", cod, &zi, &id);
        if (n != EOF) {
            printf("%s %d %d\n", cod, zi, id);
            strcpy(cod_final, cod+1);
            if (cod[0] == '+') {
                adaugare(&head, cod_final, zi, id);
            }
            else {
                stergere(&head, cod_final, zi, id);
            }
        }
    }
    printCod(head);
    eliberare_cod(head);
    fclose(f);
    return 0;
}