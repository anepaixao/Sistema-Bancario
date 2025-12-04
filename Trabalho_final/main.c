#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"
#include "administrador.h"
#include "cliente.h"
#include "unir.h"

int main(void) {
    // Preferi não depender de locale aqui (acentos desabilitados)

    Conta *contas = NULL;
    int totalContas = 0;
    int capacidadeContas = 0;
    int opcao = 0;

    // PERSISTÊNCIA — aqui eu tento carregar os dados do disco
    barraCarregamento("Carregando base de dados", 500);
    if (carregarDados(&contas, &totalContas, &capacidadeContas, "dados.bin")) {
        // Se carregou, a própria carregarDados já atualiza a capacidade
        if (capacidadeContas <= 0) capacidadeContas = totalContas > 0 ? totalContas : 2;
        char buf[128]; snprintf(buf, sizeof(buf), "Sistema iniciou com %d contas carregadas.", totalContas);
        msgSucesso(buf);
    } else {
        msgErro("Base de dados nao encontrada ou vazia.");
        printInfo("Iniciando sistema...");
        capacidadeContas = 2; // Começo pequeno e vou crescendo conforme necessário
    }
    
    // Dou uma pausa rápida para a pessoa ver o feedback
    pausarTela(); 

    // MENU PRINCIPAL — admin ou cliente
    do {
        // Uso meu visual padronizado (unir.c)
        cabecalho("SISTEMA PRINCIPAL");
        
        printf("1. Acesso Administrativo\n");
        printf("2. Acesso Cliente (Caixa Eletronico)\n");
        printf("0. Sair e Salvar\n");
        printf("--------------------------------------------------\n");
        printf("Escolha: ");
        
        // Minha leitura segura de input (fgets + strtol)
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
                // Só entro no admin se autenticar primeiro
                if (adminAutenticar()) {
                    adminMenu(&contas, &totalContas, &capacidadeContas);
                }
                break;
            case 2:
                // Entro no fluxo do cliente
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

    // PERSISTÊNCIA — ao sair, salvo os dados de volta no disco
    barraCarregamento("Salvando dados", 500);
    printInfo("Salvando dados em disco...");
    if (salvarDados(contas, totalContas, "dados.bin")) { // Passa o nome do arquivo aqui ou define no banco.c
        char buf2[128]; snprintf(buf2, sizeof(buf2), "Sucesso! %d contas salvas.", totalContas);
        msgSucesso(buf2);
    } else {
        msgErro("Falha ao salvar dados!");
    }

    // LIMPEZA — libero a memória antes de encerrar
    if (contas) free(contas);
    
    return 0;
}