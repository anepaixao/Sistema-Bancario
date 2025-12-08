#ifndef BANCO_H
#define BANCO_H

// Identidade do banco utilizada nas telas e mensagens
#define NOME_BANCO   "C-Bank"
#define SLOGAN_BANCO "Compilando o seu futuro."
#define AGENCIA_PADRAO "0001-C"

// Estrutura central da conta definida para o sistema
typedef struct {
    int id;
    char nome[50];
    char cpf[15];
    char senha[20];
    float saldo;
    unsigned char flags; // Bitwise (1 byte)
} Conta;

// Flags de status manipuladas com bitwise (bloqueio e premium)
#define FLAG_BLOQUEADA      (1u << 0) // 0000 0001
#define FLAG_PREMIUM        (1u << 1) // 0000 0010

// Protótipos das rotinas de persistência implementadas em banco.c

// Salva todas as contas no disco (binário)
int salvarDados(const Conta *contas, int total, const char *arquivo);

// Carrega as contas do disco para a memória
int carregarDados(Conta **contas, int *total, int *outCapacidade, const char *arquivo);

// Log simples para auditoria das operações
void registrarLog(int idConta, const char *descricao);

// Gestão de capacidade do vetor (usado em admin/cliente para crescer com segurança)
int garantirCapacidade(Conta **refContas, int *refCapacidade, int necessario);

#endif