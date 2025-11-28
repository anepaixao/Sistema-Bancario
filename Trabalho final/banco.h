#ifndef BANCO_H
#define BANCO_H

// --- IDENTIDADE VISUAL ---
#define NOME_BANCO   "C-Bank"
#define SLOGAN_BANCO "Compilando o seu futuro."
#define AGENCIA_PADRAO "0001-C"

// --- ESTRUTURA DE DADOS ---
typedef struct {
    int id;
    char nome[50];
    char cpf[15];
    char senha[20];
    float saldo;
    unsigned char flags; // Bitwise (1 byte)
} Conta;

// --- FLAGS DE STATUS ---
#define FLAG_BLOQUEADA      (1u << 0) // 0000 0001
#define FLAG_PREMIUM        (1u << 1) // 0000 0010
#define FLAG_EMAIL_VERIFIED (1u << 2) // 0000 0100

// --- PROTÓTIPOS DE PERSISTÊNCIA (banco.c) ---

// Salva tudo no disco
int salvarDados(const Conta *contas, int total, const char *arquivo);

// Carrega do disco 
int carregarDados(Conta **contas, int *total, int *outCapacidade, const char *arquivo);

// Log de auditoria
void registrarLog(int idConta, const char *descricao);

// Gestão de memória (usado por admin e cliente)
int garantirCapacidade(Conta **refContas, int *refCapacidade, int necessario);

#endif