#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"

// Função auxiliar para criar uma nova conta
void criarConta(Conta *contas, int *total) {
    printf("\n=== Criar Nova Conta ===\n");

    printf("Número da conta: ");
    scanf("%d", &contas[*total].numeroConta);
    getchar();

    printf("Nome do cliente: ");
    fgets(contas[*total].nome, 50, stdin);
    contas[*total].nome[strcspn(contas[*total].nome, "\n")] = '\0';

    printf("CPF: ");
    fgets(contas[*total].cpf, 15, stdin);
    contas[*total].cpf[strcspn(contas[*total].cpf, "\n")] = '\0';

    printf("Senha: ");
    fgets(contas[*total].senha, 20, stdin);
    contas[*total].senha[strcspn(contas[*total].senha, "\n")] = '\0';

    contas[*total].saldo = 0.0;
    contas[*total].status = 1;

    (*total)++;

    printf("\nConta criada com sucesso!\n");
}

// Função para listar todas as contas
void listarContas(Conta *contas, int total) {
    printf("\n=== Lista de Contas ===\n");

    if (total == 0) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("Conta: %d | Nome: %s | CPF: %s | Saldo: %.2f | Status: %s\n",
               contas[i].numeroConta,
               contas[i].nome,
               contas[i].cpf,
               contas[i].saldo,
               contas[i].status ? "Ativa" : "Bloqueada");
    }
}

// Função para bloquear conta
void bloquearConta(Conta *contas, int total) {
    int numero;
    printf("\n=== Bloquear Conta ===\n");
    printf("Digite o número da conta: ");
    scanf("%d", &numero);

    for (int i = 0; i < total; i++) {
        if (contas[i].numeroConta == numero) {
            contas[i].status = 0;
            printf("Conta %d bloqueada!\n", numero);
            return;
        }
    }

    printf("Conta não encontrada!\n");
}

// Função calcular o saldo total
float calcularSaldoRecursivo(Conta *contas, int indice, int total) {
    if (indice == total)
        return 0;
    return contas[indice].saldo + calcularSaldoRecursivo(contas, indice + 1, total);
}

// Função principal do menu do administrador
void menuAdministrador() {
    Conta *contas = NULL;
    int total = 0;
    int capacidade = 2; // capacidade inicial
    int opcao;

   
    contas = (Conta *)malloc(capacidade * sizeof(Conta));

    if (contas == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }

    do {
        printf("\n==== MENU ADMINISTRADOR ====\n");
        printf("1 - Criar Conta\n");
        printf("2 - Listar Contas\n");
        printf("3 - Bloquear Conta\n");
        printf("4 - Mostrar Saldo Total (Recursivo)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (total == capacidade) {
                    capacidade *= 2;
                    contas = (Conta *)realloc(contas, capacidade * sizeof(Conta));
                    if (contas == NULL) {
                        printf("Erro ao realocar memória!\n");
                        return;
                    }
                }
                criarConta(contas, &total);
                break;
            case 2:
                listarContas(contas, total);
                break;
            case 3:
                bloquearConta(contas, total);
                break;
            case 4:
                printf("Saldo total no banco: R$ %.2f\n",
                       calcularSaldoRecursivo(contas, 0, total));
                break;
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

   
    free(contas);
}
