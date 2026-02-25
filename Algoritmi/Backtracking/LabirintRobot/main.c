#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int di[] = {0, -1, 0, 1};
const int dj[] = {-1, 0, 1, 0};

typedef struct Matrice {
    char a[21][21];
    int b[20][20];
    char sol[21][21];
    int n, m;
}Matrice;

void initializare_matrice(Matrice *mat) {
    for (int i = 0; i < mat->n; i ++) {
        for (int j = 0; j < mat->m; j ++) {
            mat->b[i][j] = 0; //traseul inainte sa parcurga robotul
            mat->sol[i][j] = '.';
        }
    }
}

void copiere_solutie(Matrice *mat) {
    for (int i = 0; i < mat->n; i ++) {
        for (int j = 0; j < mat->m; j ++) {
            if (mat->b[i][j] != 0)
                mat->sol[i][j] = '#';
            else
                mat->sol[i][j] = '.';
        }
    }
}

void afisare_matriceA(Matrice *mat) {
    for (int i = 0; i < mat->n; i ++) {
        for (int j = 0; j < mat->m; j ++) {
            printf("%c", mat->a[i][j]);
        }
        printf("\n");
    }
}

void date_start(Matrice *mat, int *is, int *js) {
    for (int i = 0; i < mat->n; i ++) {
        for (int j = 0; j < mat->m; j ++) {
            if (mat->a[i][j] == 'S') {
                *is = i;
                *js = j;
                return;
            }
        }
    }
}

void date_teleportare(Matrice *mat, int ti[][20],int tj[][20]) {
    int ap_i[10], ap_j[10];
    for (int i = 0; i < 10; i ++) {
        ap_i[i] = -1;
        ap_j[i] = -1;
    }

    for (int i = 0; i < mat->n; i ++) {
        for (int j = 0; j < mat->m; j ++) {
             if (isdigit(mat->a[i][j])) {
                 int val = (int)(mat->a[i][j] - '0');
                 if (ap_i[val] == -1) {
                     ap_i[val] = i;
                     ap_j[val] = j;
                 }
                 else {
                     int i1 = ap_i[val];
                     int j1 = ap_j[val];
                                    //Exemplu de valori:
                     ti[i][j] = i1; ///ti[0][3] = 3;
                     tj[i][j] = j1; //tj[0][3] = 2;

                     ti[i1][j1] = i; //ti[3][2] = 0;
                     tj[i1][j1] = j; //tj[3][2] = 3
                 }
             }
        }
    }
}

void update_baterie(Matrice *mat, int i, int j, int baterie, int *baterie_noua) {
    baterie = baterie - 1;
    if (mat->a[i][j] == '+') {
        baterie += 10;
    }
    if (baterie < 0) {
        *baterie_noua = -1;
        return;
    }
    if (baterie == 0 && mat->a[i][j] != 'E') { //daca bateria e zero dar noi nu suntem la final
        *baterie_noua = -1;
        return;
    }
    *baterie_noua = baterie;
}

int valid(Matrice *mat, int i, int j, int baterie, int *baterie_noua) {
    if (i < 0 || i >= mat->n || j < 0 || j >= mat->m) { //sa fie in limitele matrice
        return 0;
    }
    if (mat->a[i][j] == '*' || mat->b[i][j] != 0) { //sa fie pe poz valida
        return 0;
    }
    update_baterie(mat, i, j, baterie, baterie_noua);
    if (*baterie_noua < 0) {
        return 0;
    }
    return 1;
}

void Bk(Matrice *mat, int i, int j, int baterie, int pas, int ti[][20], int tj[][20]) {

    static int mini = 500;
    mat->b[i][j] = pas; //marcare pozitie

    // teleportare instanta
    if (isdigit(mat->a[i][j])) {
        int it = ti[i][j];
        int jt = tj[i][j];

        if (mat->b[it][jt] == 0) { //daca nu am pasit inca pe acest pas
            pas++;
            mat->b[it][jt] = pas; //marcam pasul
            i = it; //update la coordonate
            j = jt;
        }
    }

    if (mat->a[i][j] == 'E') {
        if (pas < mini) { //solutia cu minimul de pasi
            copiere_solutie(mat);
            mini = pas;
        }
    }
    else {
        for (int k = 0; k < 4; k++) {
            int i_nou = i + di[k];
            int j_nou = j + dj[k];

            int baterie_noua;
            if (valid(mat, i_nou, j_nou, baterie, &baterie_noua)) {
                Bk(mat, i_nou, j_nou, baterie_noua, pas + 1, ti, tj);
            }
        }
    }

    mat->b[i][j] = 0; //revenire
}

void afis_sol(Matrice *mat, FILE *out) {
    for (int i = 0; i < mat->n; i ++) {
        for (int j = 0; j < mat->m; j ++) {
            fprintf(out, "%c", mat->sol[i][j]);
        }
        fprintf(out, "\n");
    }
}


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Nr incorect de argumente");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        perror("Fisier intrare");
        exit(1);
    }
    FILE *out = fopen(argv[2], "w");
    if (out == NULL) {
        perror("Fisier iesire");
        exit(1);
    }

    Matrice mat;
    int i = 0;
    char buffer[21];
    while (!feof(f)) {
        if (fgets(buffer, 21, f) != NULL) {
            if (buffer[strlen(buffer)-1] == '\n') {
                buffer[strlen(buffer)-1] = '\0';
            }
            strcpy(mat.a[i], buffer);
            i++;
        }
        else {
            fprintf(stderr,"Eroare la citirea datelor\n");
        }
    }
    mat.n = i;
    mat.m = strlen(mat.a[0]);

    int ti[20][20], tj[20][20];// le vom folosi pentru teleportare


    initializare_matrice(&mat);
   // afisare_matriceA(&mat);

    int is, js;
    date_start(&mat, &is, &js);
    date_teleportare(&mat, ti, tj);
    Bk(&mat, is, js, 5, 1, ti, tj);

    afis_sol(&mat, out);
    fclose(out);
    fclose(f);
    return 0;
}