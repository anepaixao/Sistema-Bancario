#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"
#include "administrador.h"
#include "cliente.h"
#include "unir.h"
#include <time.h>


// Vetor de contas e total (carregado no inicio)
    Conta *contas = NULL;
    int totalContas = 0;
    int capacidadeContas = 0;

int main(void) {
    int opcao;

    // carregar dados antes do menu
    if (carregarDados(&contas, &totalContas, "dados.bin")) {
        capacidadeContas = totalContas > 0 ? totalContas : 2;
        printf("%d contas carregadas de dados.bin\n", totalContas);
    } else {
        printf("Nenhum arquivo de dados encontrado ou vazio. Iniciando com 0 contas.\n");
        capacidadeContas = 2;
    }

    do {
        printf("\n===== %s =====\n", NOME_BANCO);
        printf("Agencia: %s\n", AGENCIA_PADRAO);
        printf("1. Acessar modulo do Administrador\n");
        printf("2. Acessar modulo do Cliente\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        {
            char buf[64];
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                opcao = -1;
            } else {
                trimNewline(buf);
                char *endptr = NULL;
                long v = strtol(buf, &endptr, 10);
                if (endptr == buf || *endptr != '\0') opcao = -1;
                else opcao = (int)v;
            }
        }

        switch (opcao) {
            case 1:
                if (adminAutenticar()) adminMenu(&contas, &totalContas, &capacidadeContas);
                break;
            case 2: {
                // chama menu do cliente, passando o vetor de contas (sem variáveis globais)
                clienteMenu(&contas, &totalContas, &capacidadeContas);
                break;
            }
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    // salvar dados antes de encerrar
    if (salvarDados(contas, totalContas, "dados.bin")) {
        printf("Dados salvos em dados.bin (total=%d)\n", totalContas);
    } else {
        printf("Erro ao salvar dados em dados.bin\n");
    }

    free(contas);
    return 0;
}


// (As funções de persistencia e gerenciamento de memoria foram movidas para banco.c)
