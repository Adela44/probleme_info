#include <stdio.h>
#include <stdlib.h>

typedef struct Rucsac {
    int val;
    int g;
    double profit;
}Rucsac;

void swap(Rucsac *a, Rucsac *b) {
    Rucsac tmp = *a;
    *a = *b;
    *b = tmp;
}

//QuickSort descrescator dupa profit
void QuickSort(Rucsac *arr, int st, int dr) {
    int i = st, j = dr;
    Rucsac pivot = arr[(st + dr) / 2]; //alegem elem din mijloc ca pivot

    while (i <= j) {
        while (arr[i].profit > pivot.profit) { //cautam un element care trebuia sa fie in stg pivotului
            i++;
        }
        while (arr[j].profit < pivot.profit) { //cautam un element care trebuia sa fie in dr pivotului
            j--;
        }
        if (i <= j) {
            swap(&arr[i], &arr[j]); //le interschimbam
            i++; //trecem la urm elem
            j--;
        }
    }
    if (st < j) { //daca au mai ramas elem in stg pivotului, le sortam
        QuickSort(arr, st, j);
    }
    if (i < dr) { //daca au mai ramas elem in dreapta pivotului, le sortam
        QuickSort(arr, i, dr);
    }
}

void afisare_verificare(Rucsac arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d %d %.2lf\n", arr[i].val, arr[i].g, arr[i].profit);
    }
    printf("\n");
}

void citire(Rucsac arr[], int n, FILE *f) {
    for (int i = 0; i < n; i ++) {
        fscanf(f, "%d %d", &arr[i].g, &arr[i].val);
        arr[i].profit = (double)arr[i].val / arr[i].g; //vom sorta descrescator dupa profit
    }
}

double Rucsac_max(Rucsac arr[], int n, int Gmax) {
    double sum = 0;
    int g = 0;
    for (int i = 0; i < n && g <= Gmax; i ++) {
        if (arr[i].g + g <= Gmax) { //cat timp putem aduna greutati, adunam direct si valoarea
            g += arr[i].g;
            sum += arr[i].val;
        }
        else {
            int g_aux = Gmax - g; //greutatea de care mai avem nevoie
            double val_aux = arr[i].val * ((double)g_aux / arr[i].g); //valoarea in functie de greutate
            sum += val_aux;
            break; //putem iesi, g va fi egal cu Gmax
        }
    }
    return sum;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Nr incorect de argumente\n");
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


    int n, Gmax;
    fscanf(f, "%d %d", &n, &Gmax);
   // printf("%d %d\n", n, Gmax);
    Rucsac arr[n];

    citire(arr, n, f);
    QuickSort(arr, 0, n - 1);
   // afisare_verificare(arr, n);

    double suma = Rucsac_max(arr, n, Gmax);
    fprintf(out, "%.2lf\n", suma);
    fclose(f);
    fclose(out);

    return 0;
}