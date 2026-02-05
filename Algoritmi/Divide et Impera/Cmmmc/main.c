#include <stdio.h>
#include <stdlib.h>

/*
 * Obs, pentru cel mai mic multiplu comun:  cmmmc(a,b,c)=cmmmc(cmmmc(a,b),c)
 * iar a * b = cmmmdc(a, b) * cmmmc(a, b)
 */


int cmmdc(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int cmmmc(int a, int b) {
    return (a * b) / cmmdc(a, b);
}

int Divide(int a[], int st, int dr) {
    if (st == dr) {
        return a[st];
    }
    else {
        int m = (st + dr) / 2;
        int x = Divide(a, st, m);
        int y = Divide(a, m+1, dr);
        return cmmmc(x, y);
    }
}

int main(void) {

    int n;
    scanf("%d", &n);
    int *a = malloc(n * sizeof(int));

    for (int i = 0; i < n; i ++) {
        scanf("%d", &a[i]);
    }
    int rez = Divide(a, 0, n-1);
    printf("%d\n", rez);
    free(a);
    return 0;
}