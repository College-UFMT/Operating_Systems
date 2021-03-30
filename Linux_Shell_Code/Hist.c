#ifndef Hist_C
#define Hist_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lsComandos
{
	char param[50];
	struct lsComandos *prox;
} lsComandos;

typedef lsComandos *lista;

void inicializa(lista *ls)
{
	for (int i = 0; i < 1024; i++)
		ls[i] = NULL;
}

lsComandos *aloca(char *param)
{
	lsComandos *novo = (lsComandos *)malloc(sizeof(lsComandos));
	strcpy(novo->param, param);
	novo->prox = NULL;

	return novo;
}

void insereHist(lista *ls, char *param, int indice)
{

	lsComandos *p = ls[indice];
	lsComandos *ant = NULL;

	while (p != NULL)
	{
		ant = p;
		p = p->prox;
	}

	if (p)
		return;
	lsComandos *novo = aloca(param);

	if (!ant)
		ls[indice] = novo;
	else
		ant->prox = novo;
	puts(novo->param);
}

void separaArg(lista *ls, char *args, int indice)
{
	int j = 0;

	for (int i = 0; i < (int)strlen(args) - 1; i++)
	{
		if (args[i + 1] == ' ')
		{
			int tam = i - j + 1;
			char *param;
			param = (char *)malloc(tam * sizeof(char));
			strncpy(param, args + j, tam);

			insereHist(ls, param, indice);

			j = i + 2;
			free(param);
		}
	}
}

void imprime(lista *ls)
{
	int i;

	for (i = 0; i < 20; i++)
	{
		printf("%d", i);
		if (ls[i])
		{
			lsComandos *p = ls[i];
			printf("\n");
			while (p)
			{
				printf("\t%s\t%p\n", p->param, p->prox);
				p = p->prox;
			}
		}
		else
			printf("\n\tNULL\n");
	}
}

#endif