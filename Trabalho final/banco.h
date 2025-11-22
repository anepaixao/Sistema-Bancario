#ifndef BANCO_H
#define BANCO_H

// Estrutura base (conforme especificado)
typedef struct {
    int id;
    char nome[50];
    char cpf[15];
    char senha[20];
    float saldo;
    unsigned char flags; // Bitwise para status
} Conta;

// Flags para `Conta.flags`
#define FLAG_BLOQUEADA      (1u << 0)
#define FLAG_PREMIUM        (1u << 1)
#define FLAG_EMAIL_VERIFIED (1u << 2)

// Protótipos básicos (os módulos do sistema podem declarar mais)
int adminAutenticar(void);
void adminMenu(Conta **contas, int *total, int *capacidade);

// Persistência e logging (implementadas em main.c)
int salvarDados(const Conta *contas, int total, const char *arquivo);
int carregarDados(Conta **contas, int *total, const char *arquivo);
void registrarLog(int idConta, const char *descricao);

#endif