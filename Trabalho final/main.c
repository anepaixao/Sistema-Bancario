#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "banco.h"
#include "administrador.h"
#include "cliente.h"
#include "unir.h"

int main(void) {
    // Configura acentos (opcional, depende do terminal)
    setlocale(LC_ALL, ""); 

    Conta *contas = NULL;
    int totalContas = 0;
    int capacidadeContas = 0;
    int opcao = 0;

    // --- PERSISTÊNCIA: CARREGAR DADOS ---
    if (carregarDados(&contas, &totalContas, "dados.bin")) {
        // Se carregou, ajusta a capacidade para não quebrar o realloc futuro
        capacidadeContas = totalContas > 0 ? totalContas : 2;
        printf("Sistema iniciou com %d contas carregadas.\n", totalContas);
    } else {
        printf("Base de dados vazia ou nova. Iniciando sistema...\n");
        capacidadeContas = 2; // Começa pequeno
    }
    
    // Pequena pausa para ler a mensagem de carregamento
    pausarTela(); 

    // --- MENU PRINCIPAL ---
    do {
        // Usa nosso visual padronizado (unir.c)
        cabecalho("SISTEMA PRINCIPAL");
        
        printf("1. Acesso Administrativo\n");
        printf("2. Acesso Cliente (Caixa Eletronico)\n");
        printf("0. Sair e Salvar\n");
        printf("--------------------------------------------------\n");
        printf("Escolha: ");
        
        // Sua leitura segura de input (Excelente!)
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
                // Só entra se acertar usuário/senha do admin
                if (adminAutenticar()) {
                    adminMenu(&contas, &totalContas, &capacidadeContas);
                }
                break;
            case 2:
                // Passa os ponteiros para o módulo cliente
                clienteMenu(&contas, &totalContas, &capacidadeContas);
                break;
            case 0:
                printf("\nEncerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                pausarTela();
        }
    } while (opcao != 0);

    // --- PERSISTÊNCIA: SALVAR E SAIR ---
    printf("Salvando dados em disco...\n");
    if (salvarDados(contas, totalContas, "dados.bin")) { // Passa o nome do arquivo aqui ou define no banco.c
        printf("Sucesso! %d contas salvas.\n", totalContas);
    } else {
        printf("ERRO CRITICO: Falha ao salvar dados!\n");
    }

    // ---  LIMPEZA DE MEMÓRIA  ---
    if (contas) free(contas);
    
    return 0;
}