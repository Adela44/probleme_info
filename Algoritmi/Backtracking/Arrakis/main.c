#include <stdio.h>
#include <stdlib.h>

typedef struct Matrice{
	int **a, **b;
	int n, m;
	int K, E, B;
}Matrice;

const int di[] = {-1, 0, 1, 0};
const int dj[] = {0, 1, 0, -1};

int x[1000], y[1000]; //salvam coord prin care trece pasul
int nr_trasee =  0; //contor globar si var globale sa nu incarcam functia cu parametrii

void afisare_traseu(Matrice *mat, int pas, int E_final);
int este_valid(Matrice *mat, int i, int j, int cnt_vechi, int E_vechi, int *nextCnt, int *nextE);


void afisare_traseu(Matrice *mat, int pas, int E_final)
{
	for(int p = 1; p <= pas; p ++)
	{
		printf("(%d,%d)", x[p], y[p]);
		if(p != pas)
		{
            printf(" -> ");
		}
	}
	printf("\n");
	printf("Energia ramasa: %d\n", E_final);
	printf("Nr total de trasee: %d\n", nr_trasee);
}

int este_valid(Matrice *mat, int i, int j, int cnt_vechi, int E_vechi, int *nextCnt, int *nextE)
{
	if(i < 0 || j < 0 || i >= mat->n || j >= mat->m)
	{
		return 0;
	}

	if(mat->a[i][j] == 1 || mat->b[i][j] != 0)
	{
		return 0;
	}

	//Orice celula care nu e 0, reseteaza contorul
	int cnt_aux;
	if(mat->a[i][j] == 0)
	{
        cnt_aux = cnt_vechi + 1;
	}
	else
	{
		cnt_aux = 0;
	}

	//daca e mai mare cnt ca si K, return
	if(cnt_aux > mat->K)
	{
		return 0;
	}

	//Calcul energie (Consum + Bonus)
	int cost, E_aux;
    if(mat->a[i][j] == 4)
	{
         cost = 2;
	}
	else
	{
		 cost = 1;
	}
	E_aux = E_vechi - cost;

	//daca e melanj, crestem cu B, cat timp nu depasete mat->E, energia initiala
	if(mat->a[i][j] == 5)
	{
        E_aux += mat->B;
		if(E_aux > mat->E)
		{
            E_aux = mat->E;
		}
	}

	//verificam acum daca energia la care am ajuns ne mai permite sa ne miscam
	if(E_aux < 0)
	{
		return 0;
	}
	if(E_aux == 0 && mat->a[i][j] != 3)
	{
		return 0;
	}

	//daca a trecut de toate datele, "exportam" noile valori
    *nextCnt = cnt_aux;
	*nextE = E_aux;
	return 1;
}


void Bk(Matrice *mat, int i, int j, int pas, int cnt0, int E)
{
	mat->b[i][j] = pas;
    x[pas] = i;
	y[pas] = j;

    if(mat->a[i][j] == 3)
	{
		nr_trasee++;
		afisare_traseu(mat, pas, E);
	}
	else
	{
        for(int k = 0; k < 4; k ++)
		{
            int new_cnt0, newE;
			if(este_valid(mat, i + di[k], j + dj[k], cnt0, E, &new_cnt0, &newE))
			{
				Bk(mat, i + di[k], j + dj[k], pas + 1, new_cnt0, newE);
			}

		}
	}

	mat->b[i][j] = 0;
}

void eliberare(Matrice *mat)
{
	for(int i = 0; i < mat->n; i ++)
	{
		free(mat->a[i]);
		free(mat->b[i]);
	}
	free(mat->a);
	free(mat->b);
}

void cauta(Matrice *mat, int *is, int *js)
{
	for(int i = 0; i < mat->n; i ++)
	{
		for(int j = 0; j < mat->m; j ++)
		{
			 if(mat->a[i][j] == 2)
			 {
				  *is = i;
				  *js = j;
				  return;
			 }
		}
	}
}

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Nr incorect de argumente");
		exit(1);
	}


	FILE *f = fopen(argv[1], "r");
	if(f == NULL)
	{
		perror("Fisier citire:");
		exit(1);
	}
	Matrice mat;
	fscanf(f, "%d %d %d %d %d", &mat.n, &mat.m, &mat.K, &mat.E, &mat.B);

	mat.a = (int **)malloc(sizeof(int *) * mat.n); //ar trebui verificat si malloc
	mat.b = (int **)calloc(mat.n, sizeof(int *));
	for(int i = 0; i < mat.n; i ++)
	{
		mat.a[i] = (int *)malloc(sizeof(int) * mat.m);
		mat.b[i] = (int *)calloc(mat.n, sizeof(int));
	}

    for(int i = 0; i < mat.n; i ++)
	{
		for(int j = 0; j < mat.m; j ++)
		{
			fscanf(f, "%d", &mat.a[i][j]);
		}
	}

    int is, js;
    cauta(&mat, &is, &js);
	//printf("%d %d\n", is, js);
	Bk(&mat, is, js, 1, 0, mat.E);
	eliberare(&mat);
	fclose(f);
	 return 0;
}