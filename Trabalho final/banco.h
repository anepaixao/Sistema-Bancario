#ifndef BANCO_H
#define BANCO_H

// ============================
// Estrutura base
// ============================
typedef struct {
    int numeroConta;
    char nome[50];
    char cpf[15];
    char senha[7]; // somente números, até 6 dígitos + '\0'
    char agencia[20]; // nome/código da agência
    float saldo;
    int status; // 1 = ativa, 0 = bloqueada
} Conta;

// ============================
// Funções do Administrador
// ============================

// Exibe o menu do administrador
void menuAdministrador();

// Autenticação do administrador
// Retorna 1 se autenticado, 0 caso contrário
int autenticarAdministrador(void);

// Cria uma nova conta bancária
void criarConta(Conta *contas, int *total);

// Lista todas as contas cadastradas
void listarContas(Conta *contas, int total);

// Bloqueia uma conta específica
void bloquearConta(Conta *contas, int total);

// Calcula o saldo total do banco
float calcularSaldoRecursivo(Conta *contas, int indice, int total);

#endif
