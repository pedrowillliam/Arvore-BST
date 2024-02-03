#include <stdio.h>
#include <stdlib.h>
#include "Tabela/tabela.c" 


int main() {
	tabela tab;
	int opcao;
	
	printf("Inicializando tabela...\n");

    inicializarTabela(&tab);

    printf("Tabela inicializada com sucesso!\n");

	printf("Digite a opcao desejada:\n");
	printf("1 - Adicionar Roupa\n");
	printf("2 - Remover Roupa pelo codigo\n");
	printf("5 - inordem BST \n");
	printf("8 - Buscar Roupa por codigo\n");
	printf("11 - listar todos\n"); 
    printf("99 - Sair\n");

    while(1) {
		scanf("%d", &opcao);

		switch(opcao) {
            case 1:	
                
                adicionarRoupa(&tab, ler_dados());
                puts("Roupa adicionada com sucesso!");
                break;

            case 2:
						printf("Digite o codigo da Roupa:\n");
						char codigo[50];					
						scanf("%s", codigo);
						removerRoupaIndiceBST(&tab, codigo);  
						salvar_arquivo_BST("indicesBST.dat", tab.indice_bst);	
						puts("Roupa removida em indicesBST.dat com sucesso!");						
						break;
            case 5:
						inorderBST(tab.indice_bst);
						printf("\n");
						break;			
            case 8:
						printf("Digite o codigo da Roupa:\n");				
						scanf("%s", codigo);
						procurarRoupaIndiceBST(&tab, codigo);  
						break;             
            case 11:
						listarTodos(&tab);
                        break;
            case 99:
                printf("Saindo...\n");
                exit(0);
                break;            
        }   
    }
    return 0;
}
