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
                if (adminAutenticar()) adminMenu();
                break;
            case 2:
                // delega fluxo do cliente para o modulo cliente
                // limpa newline pendente do scanf do menu
                { int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} }
                clienteExecutar();
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    /*Main Cliente
    #include "banco.h"

int main(void) {
    setlocale(LC_ALL, "Portuguese_Brazil.UTF-8");

    char escolhar[20];

    usuario.configuracoes = NOTIFICACOES_ATIVAS | CONFIRMACAO_VALORES;

    int resultado_login = login();

    if(resultado_login == -1) {
        printf("Falha no login. Encerrando programa.\n");
        system("PAUSE");
        return 1;
    }

    tela_principal();

    printf("DIGITE A OPÇÃO QUE DESEJA: ");
    scanf(" %19[^\n]", escolhar);

    opcoes(escolhar);

    printf("Obrigada por usar o banco!\n");

    return 0;
}*/

    return 0;
}

