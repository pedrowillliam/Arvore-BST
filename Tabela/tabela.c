#include "tabela.h" 
#include "../BST/bst.c"
#include <stdio.h>
#include <string.h>

int inicializarTabela(tabela *tab) {
	inicializarBST(&tab->indice_bst);	
	tab->arquivo_dados = fopen("dados.dat", "a+");
	carregar_arquivo_BST("indicesBST.dat", &tab->indice_bst);
	if(tab->arquivo_dados != NULL)
		return 1;
	else
		return 0;
}

void finalizar (tabela *tab) {
	fclose(tab->arquivo_dados);
	salvar_arquivo_BST("indicesBST.dat", tab->indice_bst);
}


void adicionarRoupa(tabela *tab, dado *roupa){
	
    int posicaoNovoRegistro;
    if(tab->arquivo_dados != NULL) {
			
			fseek(tab->arquivo_dados, 0L, SEEK_END);
			posicaoNovoRegistro = ftell(tab->arquivo_dados);
			
            dado * novo = (dado *) malloc(sizeof(dado));
			novo->codigo = roupa->codigo;
			
			tab->indice_bst = inserirBST(tab->indice_bst, roupa->codigo, posicaoNovoRegistro);

			novo->codigo = roupa->codigo;

			fprintf(tab->arquivo_dados, "%s\n", roupa->codigo);
			
    }
}

void salvar_arquivo_BST(char *nome, arvoreBST a){
	FILE *arq;
	arq = fopen(nome, "w+");
	if(arq != NULL) {

		salvar_auxiliar_BST(a, arq);
		
		fclose(arq);
	}
}

void salvar_auxiliar_BST(arvoreBST raiz, FILE *arq){
    if(raiz != NULL) {
        fprintf(arq, "%d;%s\n", raiz->indice, raiz->codigo);
        salvar_auxiliar_BST(raiz->esq, arq);
        salvar_auxiliar_BST(raiz->dir, arq);
		
    }
}



void carregar_arquivo_BST(char *nome, arvoreBST* a){
    FILE* arq;
	
    arq = fopen(nome, "r+");
    size_t len = 50;
    char* linha = malloc(len);
    char delim[] = ";"; 
    if(arq!= NULL){
        while(getline(&linha, &len, arq)>0){   			
            char* var = malloc(len);
            var = linha;
            char* ptr = strtok(var, delim);
            int indice = atoi(ptr);
            ptr = strtok(NULL, delim);
            char* nome = (char*) malloc(sizeof(ptr));
            strcpy(nome, ptr);
            tirar_enter(nome);
            *a = inserirBST(*a, nome, indice);
			
        }
        fclose(arq);
    }
}

void tirar_enter(char *string){

	string[strlen(string) -1] = '\0';
}


dado * ler_dados() {
	dado *novo = (dado *) malloc(sizeof(dado));
	char * buffer = (char *) malloc(256 * sizeof(char));
	
	getchar();
	printf("Codigo: \n");
	fgets(buffer, 80,  stdin);
	novo->codigo = strdup(buffer);
	tirar_enter(novo->codigo);

	return novo;
}

void imprimir_elemento_BST(arvoreBST raiz, tabela * tab) {
	dado * temp = (dado *) malloc (sizeof(dado));
	fseek(tab->arquivo_dados, raiz->indice, SEEK_SET);
	fread(temp, sizeof(dado), 1, tab->arquivo_dados);
	printf("[%s]\n", temp->codigo);
	free(temp);
}

void removerRoupaIndiceBST(tabela *tab, char* codigo){
	

	arvoreBST raiz = tab->indice_bst;
	arvoreBST pai = NULL;



	while(raiz != NULL){
		if(strcmp(raiz->codigo, codigo) == 0){
			if(raiz->esq == NULL && raiz->dir == NULL){
				if(pai == NULL){
					tab->indice_bst = NULL;
				}
				else if(pai->esq == raiz){
					pai->esq = NULL;
				}
				else{
					pai->dir = NULL;
				}
				free(raiz);
			}
			else if(raiz->esq == NULL){
				if(pai == NULL){
					tab->indice_bst = raiz->dir;
				}
				else if(pai->esq == raiz){
					pai->esq = raiz->dir;
				}
				else{
					pai->dir = raiz->dir;
				}
				free(raiz);
			}
			else if(raiz->dir == NULL){
				if(pai == NULL){
					tab->indice_bst = raiz->esq;
				}
				else if(pai->esq == raiz){
					pai->esq = raiz->esq;
				}
				else{
					pai->dir = raiz->esq;
				}
				free(raiz);
			}
			else{
				arvoreBST aux = raiz->dir;
				arvoreBST auxpai = raiz;
				while(aux->esq != NULL){
					auxpai = aux;
					aux = aux->esq;
				}
				raiz->indice = aux->indice;
				strcpy(raiz->codigo, aux->codigo);
				if(auxpai->esq == aux){
					auxpai->esq = aux->dir;
				}
				else{
					auxpai->dir = aux->dir;
				}
				free(aux);
			}
			return;
		}
		else if(strcmp(raiz->codigo, codigo) > 0){
			pai = raiz;
			raiz = raiz->esq;
		}
		else{
			pai = raiz;
			raiz = raiz->dir;
		}
	}
}


void procurarRoupaIndiceBST(tabela *tab, char* codigo) {
	arvoreBST p = tab->indice_bst;
	
	
	

	while (p != NULL) {
		if (strcmp(p->codigo, codigo) == 0) {
			imprimirRoupa(p->indice, tab);
			return;
		}

		else if (strcmp(p->codigo, codigo) < 0) {
			p = p->dir;
			
			
		}

		else {
			p = p->esq;
			
			
		}

	}
}
   

void imprimirRoupa(int indice, tabela* tab){
	fseek(tab->arquivo_dados, indice, SEEK_SET);
    char codigo[50] = {0};
    
    
    if (fscanf(tab->arquivo_dados, "%[^|]\n", codigo) == 1) {

        printf("%s\n", codigo);

    }
    else {
        printf("Erro na leitura do registro.\n");
    }
}

void listarTodos(tabela *tab){
	fseek(tab->arquivo_dados, 0, SEEK_SET);
	char codigo[50] = {0};
	printf("listando todas as roupa da tabela dados.dat\n");
	while (fscanf(tab->arquivo_dados, "%[^|]\n", codigo) == 1) {		
		printf("%s\n", codigo);
	}
}    

