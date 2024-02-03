#ifndef TABELA_H
#define TABELA_H
#include "../BST/bst.h"
#include <stdlib.h>
#include <stdio_ext.h>


typedef struct roupa{
    char* codigo;
}dado;

typedef struct tabela{
    FILE* arquivo_dados;
    arvoreBST indice_bst;

}tabela;

int inicializarTabela(tabela *tab);

void finalizar (tabela *tab);

void adicionarRoupa(tabela *tab, dado *roupa);

void carregar_arquivo_BST(char *nome, arvoreBST* a);
void salvar_arquivo_BST(char *nome, arvoreBST a);
void salvar_auxiliar_BST(arvoreBST raiz, FILE *arq);


void imprimirRoupa(int indice, tabela *tab);

void carregar_arquivo_BST(char *nome, arvoreBST* a);
void salvar_arquivo_BST(char *nome, arvoreBST a);
void salvar_auxiliar_BST(arvoreBST raiz, FILE *arq);
void imprimir_elemento_BST(arvoreBST raiz, tabela * tab);

void listarTodos(tabela *tab);

void removerRoupaIndiceBST(tabela *tab, char* codigo);
void procurarRoupaIndiceBST(tabela *tab, char* codigo);
dado * ler_dados();

void tirar_enter(char *string);
#endif