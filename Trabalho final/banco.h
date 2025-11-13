#ifndef BANCO_H
#define BANCO_H


// Estrutura base

typedef struct {
    int numeroConta;
    char nome[50];
    char cpf[15];
    char senha[7]; // somente números, 6 dígitos + '\0'
    char agencia[20]; // código da agência
    float saldo;
    int status; // 1 = ativa, 0 = bloqueada
} Conta;


// Funções do Administrador


// Autentica o administrador (retorna 1 se ok, 0 caso contrário)
int adminAutenticar(void);

// Exibe o menu principal do módulo administrador
void adminMenu(void);

// Cria uma nova conta bancária
void adminCriarConta(Conta *contas, int *total);

// Callback de filtro: retorna 1 se deve exibir a conta, 0 caso contrário
typedef int (*ContaFiltro)(const Conta *c);

// Lista contas (se filtro NULL, exibe todas)
void adminListarContas(Conta *contas, int total, ContaFiltro filtro);

// Bloqueia uma conta (status=0)
void adminBloquearConta(Conta *contas, int total);

// Desbloqueia uma conta (status=1)
void adminDesbloquearConta(Conta *contas, int total);

// Calcula saldo total (versão recursiva)
float adminCalcularSaldoTotalRecursivo(Conta *contas, int indice, int total);

#endif
