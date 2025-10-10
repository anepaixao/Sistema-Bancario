#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"
#include "cliente.h"

int main(void) {
    int opcao;

    do {
        printf("\n===== SISTEMA BANCARIO =====\n");
        printf("1. Acessar modulo do Administrador\n");
        printf("2. Acessar modulo do Cliente\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                if (autenticarAdministrador()) menuAdministrador();
                break;
            case 2: {
                char escolha[64] = "";
                // limpar newline pendente
                int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                login();
                tela_principal();
                printf("DIGITE A OPCAO QUE DESEJA: ");
                if (fgets(escolha, sizeof(escolha), stdin) != NULL) {
                    escolha[strcspn(escolha, "\n")] = '\0';
                    opcoes(escolha);
                }
                break;
            }
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

