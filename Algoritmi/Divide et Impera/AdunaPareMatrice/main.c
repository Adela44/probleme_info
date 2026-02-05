#include <stdio.h>


/*
 *  !! Verifica ca nu depasesti stanga nu depaseste dreapta, cum tot adun mij + 1 e posibil sa se intample
 *
 */


int Divide(int a[][100], int stl, int drl, int stc, int drc) {
           if (stl > drl || stc > drc) {
               return 0;
           }
          if (stl == drl && stc == drc) {
              return a[stl][drc] % 2 == 0 ? a[stl][drc] : 0;
          }
          else {
              //printf("stl: %d  drl: %d, stc: %d, drc: %d\n", stl, drl, stc, drc);
              int ml = (stl + drl) / 2;
              int mc = (stc + drc) / 2;
              int s1 =  Divide(a, stl, ml, stc, mc);
              int s2 = Divide(a, stl, ml, mc+1, drc);
              int s3 = Divide(a, ml+1, drl, stc, mc);
              int s4 = Divide(a, ml+1, drl, mc + 1, drc);
              return s1 + s2 + s3 + s4;
          }
      }




int main(void) {
    int n, m;
    scanf("%d %d", &n, &m);
    int a[100][100];

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            scanf("%d", &a[i][j]);
        }
    }

    int rez = Divide(a, 0, n-1, 0, m-1);
    printf("%d", rez);
    return 0;
}