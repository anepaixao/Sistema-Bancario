#include <stdio.h>
#include <stdlib.h>
#include "banco.h" 

int main(void) {
    int opcao;

    do {
        printf("\n===== SISTEMA BANCARIO =====\n");
        printf("1. Acessar modulo do Administrador\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (autenticarAdministrador()) {
                    menuAdministrador();
                }
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
