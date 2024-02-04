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


void adicionarRoupa(tabela *tab, dado *roupa) {
    int posicaoNovoRegistro;
    if (tab->arquivo_dados != NULL) {
        fseek(tab->arquivo_dados, 0L, SEEK_END);
        posicaoNovoRegistro = ftell(tab->arquivo_dados);
        
        // Alocar memória para o novo registro
        dado *novo = (dado *) malloc(sizeof(dado));
        if (novo == NULL) {
            printf("Erro ao alocar memória para novo registro.\n");
            return;
        }

        // Alocar memória para o código da roupa e copiá-lo
        novo->codigo = strdup(roupa->codigo);
        if (novo->codigo == NULL) {
            printf("Erro ao alocar memória para código da roupa.\n");
            free(novo);
            return;
        }
        
        // Inserir na árvore de busca binária
        tab->indice_bst = inserirBST(tab->indice_bst, novo->codigo, posicaoNovoRegistro);

        // Escrever o código da roupa no arquivo de dados
        fprintf(tab->arquivo_dados, "%s\n", novo->codigo);
        
        // Liberar memória alocada para o novo registro
        free(novo->codigo);
        free(novo);
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
    if (novo == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para novo dado\n");
        return NULL;
    }
    
    char *buffer = (char *) malloc(256 * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para buffer\n");
        free(novo);
        return NULL;
    }
    
    getchar();
    printf("Codigo: \n");
    if (fgets(buffer, 256, stdin) == NULL) {
        fprintf(stderr, "Erro: Falha ao ler o código\n");
        free(novo);
        free(buffer);
        return NULL;
    }
    
    novo->codigo = strdup(buffer);
    if (novo->codigo == NULL) {
        fprintf(stderr, "Erro: Falha ao duplicar o código\n");
        free(novo);
        free(buffer);
        return NULL;
    }
    
    tirar_enter(novo->codigo);
    free(buffer); // Liberar memória alocada para o buffer
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
    int encontrou = 0;

    while(raiz != NULL){
        if(strcmp(raiz->codigo, codigo) == 0){
            encontrou = 1; // Marca que a roupa foi encontrada

            // Lógica para remover a roupa
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

            printf("Roupa removida em indicesBST.dat com sucesso!\n");
            return;
        } else if(strcmp(raiz->codigo, codigo) > 0){
            pai = raiz;
            raiz = raiz->esq;
        } else {
            pai = raiz;
            raiz = raiz->dir;
        }
    }

    // Se o loop terminou e a roupa não foi encontrada
    if (encontrou == 0) {
        printf("Roupa não encontrada. Impossível remover.\n");
    }
}



void procurarRoupaIndiceBST(tabela *tab, char* codigo) {
    arvoreBST p = tab->indice_bst;
    int encontrou = 0; 

    while (p != NULL) {
        if (strcmp(p->codigo, codigo) == 0) {
            encontrou = 1; // Marca que a roupa foi encontrada
            imprimirRoupa(p->indice, tab);
            return;
        } else if (strcmp(p->codigo, codigo) < 0) {
            p = p->dir;
        } else {
            p = p->esq;
        }
    }

    // Se o loop terminou e a roupa não foi encontrada
    if (encontrou == 0) {
        printf("Roupa não encontrada.\n");
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

void listarTodos(tabela *tab) {
    fseek(tab->arquivo_dados, 0, SEEK_SET);
    char codigo[50]; // Não é necessário reinicializar

    printf("Listando todas as roupas da tabela dados.dat\n");

    // Verifica se o fscanf conseguiu ler um código de roupa com sucesso
    while (fscanf(tab->arquivo_dados, "%49[^\n]%*c", codigo) == 1) {
        printf("%s\n", codigo);
    }

    // Verifica explicitamente o fim do arquivo
    if (feof(tab->arquivo_dados)) {
        printf("Fim do arquivo alcançado.\n");
    } else {
        printf("Erro ao ler o arquivo.\n");
    }
}

    

