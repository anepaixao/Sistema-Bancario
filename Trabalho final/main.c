#include <stdio.h>
#include <stdlib.h>
#include "banco.h" 

int main(void) {
    int opcao;

    do {
        printf("\n===== SISTEMA BANCÁRIO =====\n");
        printf("1. Acessar módulo do Administrador\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menuAdministrador();
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

//Parte do usuario///

int main(void) {
    char escolhar[20];  
   
    login();
    tela_principal();
    
    
    puts("DIGITE A OPÇÃO QUE DESEJA: "); 
    scanf(" %[^\n]", escolhar);
  
    opcoes(escolhar);
  
    printf("Obrigada por usar o banco!");
    return 0;
}
