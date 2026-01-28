#include <stdio.h>
#include <stdlib.h>
#define maxi 50

/*
 Se dă un număr natural n.
 Determinaţi, în ordine lexicografică, toate modalităţile de a-l scrie pe n ca sumă de numere naturale.

 n <= 50
 */

/*
   Modificari enunt:

   - pentru nr disticte naturale, modifica in valid: x[k-1] >= x[k]
   - pentru nr pare, in Bk for(int i = 2; i <= n; i += 2)
   - pentru nr impare distincte, modifica in valid x[k-1] >= x[k] si in Bk, for(int i = 1; i <= n; i += 2)
 */

void afisare(int x[], int k, FILE *out) {
    for (int i = 0; i <= k; i ++) {
        fprintf(out, "%d ", x[i]);
    }
    fprintf(out, "\n");
}

int valid(int x[], int k, int s, int n) {
    if ( ((k > 0) && (x[k-1] > x[k])) || (s + x[k] > n)) { //verificam ordinea elementelor si daca suma pana acum e ce trebuie
        return 0;
    }
    return 1;
}

int solutie(int x[], int k, int s, int n) { // am ajuns la suma dorita
    return ((s + x[k]) == n);
}

void Bk(int x[], int k, int s, int n, FILE *out) {
    for (int i = 1; i <= n; i ++) {
        x[k] = i;
        if (valid(x, k, s, n)) {
            if (solutie(x, k, s, n)) {
                afisare(x, k, out);
            }
            else {
                Bk(x, k + 1, s + x[k], n, out);
            }
        }
    }
}




int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Eroare la citirea din fisier\n");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        perror("Eroare fisier citire");
        exit(1);
    }

    FILE *out = fopen(argv[2], "w");
    if (out == NULL) {
        perror("Eroare fisier output");
        exit(1);
    }

    int n, x[maxi];

    fscanf(f, "%d", &n);

    Bk(x, 0, 0, n, out);
    fclose(f);
    fclose(out);
}