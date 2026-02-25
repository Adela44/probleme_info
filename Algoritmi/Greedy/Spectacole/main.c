#include <stdio.h>
#include <stdlib.h>

typedef struct Ora {
    int start, end;
}Ora;

void swap(Ora* a, Ora* b) {
    Ora t = *a;
    *a = *b;
    *b = t;
}

//ordonare crescatoare dupa sfarsit ul intervalelor
void QuickSort(Ora arr[], int st, int dr) {
    int i = st;
    int j = dr;

    Ora pivot = arr[(st + dr)/2];

    while (i <= j) {
        while (arr[i].end < pivot.end) {
            i++;
        }
        while (arr[j].end > pivot.end) {
            j--;
        }
        if (i <= j) {
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }
    if (st < j)
        QuickSort(arr, st, j);
    if (i < dr)
        QuickSort(arr, i, dr);
}

void afisare(Ora arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d %d\n", arr[i].start, arr[i].end);
    }
    printf("\n");
}

int interval_maxim(Ora arr[], int n) {
    int aux = arr[0].end;
    int cnt = 1;
    for (int i = 0; i < n; i ++) {
        if (aux <= arr[i].start) { //daca sfarsitul primului interval este mai mic sau egal decat inceputul urmatorului
            aux = arr[i].end;
            cnt++;
        }
    }
    return cnt;
}

int main(int argc, char ** argv) {
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
    int n;
    fscanf(f, "%d", &n);
    Ora v[n];
    for (int i = 0; i < n; i ++) {
        fscanf(f,"%d %d", &v[i].start, &v[i].end);
    }
    QuickSort(v, 0, n-1);
   // afisare(v, n);
    int nr = interval_maxim(v, n);
    fprintf(out,"%d\n", nr);
    fclose(f);
    fclose(out);
    return 0;
}