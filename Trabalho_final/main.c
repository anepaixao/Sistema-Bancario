#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"
#include "administrador.h"
#include "cliente.h"
#include "unir.h"

int main(void) {

    Conta *contas = NULL;
    int totalContas = 0;
    int capacidadeContas = 0;
    int opcao = 0;

    // PERSISTÊNCIA — carregamento inicial dos dados em disco
    barraCarregamento("Carregando base de dados", 500);
    if (carregarDados(&contas, &totalContas, &capacidadeContas, "dados.bin")) {
        // Em caso de carregamento bem-sucedido, a função atualizará a capacidade
        if (capacidadeContas <= 0) capacidadeContas = totalContas > 0 ? totalContas : 2;
        char buf[128]; snprintf(buf, sizeof(buf), "Sistema iniciou com %d contas carregadas.", totalContas);
        msgSucesso(buf);
    } else {
        msgErro("Base de dados nao encontrada ou vazia.");
        printInfo("Iniciando sistema...");
        capacidadeContas = 2; // Inicialização mínima; cresce conforme necessário
    }
    
    // Pausa breve para exibir o feedback ao usuário
    pausarTela(); 

    // MENU PRINCIPAL — admin ou cliente
    do {
        // Visual padronizado (unir.c)
        cabecalho("SISTEMA PRINCIPAL");
        
        printf("1. Acesso Administrativo\n");
        printf("2. Acesso Cliente (Caixa Eletronico)\n");
        printf("0. Sair e Salvar\n");
        printf("--------------------------------------------------\n");
        printf("Escolha: ");
        
        // Leitura segura de entrada (fgets + strtol)
        char buf[64];
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            opcao = -1;
        } else {
            trimNewline(buf); // Do unir.h
            char *endptr = NULL;
            long v = strtol(buf, &endptr, 10);
            if (endptr == buf || *endptr != '\0') opcao = -1;
            else opcao = (int)v;
        }

        switch (opcao) {
            case 1:
                // Acesso administrativo condicionado à autenticação prévia
                if (adminAutenticar()) {
                    adminMenu(&contas, &totalContas, &capacidadeContas);
                }
                break;
            case 2:
                // Entrada no fluxo do cliente
                clienteMenu(&contas, &totalContas, &capacidadeContas);
                break;
            case 0:
                barraCarregamento("Encerrando o sistema", 400);
                printf("\nEncerrando o sistema...\n");
                break;
            default:
                msgErro("Opcao invalida. Tente novamente.");
                pausarTela();
        }
    } while (opcao != 0);

    // PERSISTÊNCIA — salvamento dos dados em disco ao encerrar
    barraCarregamento("Salvando dados", 500);
    printInfo("Salvando dados em disco...");
    if (salvarDados(contas, totalContas, "dados.bin")) { // Passa o nome do arquivo aqui ou define no banco.c
        char buf2[128]; snprintf(buf2, sizeof(buf2), "Sucesso! %d contas salvas.", totalContas);
        msgSucesso(buf2);
    } else {
        msgErro("Falha ao salvar dados!");
    }

    // LIMPEZA — liberação de memória antes do encerramento
    if (contas) free(contas);
    
    return 0;
}