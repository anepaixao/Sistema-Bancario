#ifndef BANCO_H
#define BANCO_H

// ============================
// Estrutura base
// ============================
typedef struct {
    int numeroConta;
    char nome[50];
    char cpf[15];
    char senha[20];
    float saldo;
    int status; // 1 = ativa, 0 = bloqueada
} Conta;

// ============================
// Funções do Administrador
// ============================

// Exibe o menu do administrador
void menuAdministrador();

// Cria uma nova conta bancária
void criarConta(Conta *contas, int *total);

// Lista todas as contas cadastradas
void listarContas(Conta *contas, int total);

// Bloqueia uma conta específica
void bloquearConta(Conta *contas, int total);

// Calcula o saldo total do banco de forma recursiva
float calcularSaldoRecursivo(Conta *contas, int indice, int total);

#endif
