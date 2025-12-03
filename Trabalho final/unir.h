#ifdef _WIN32
#include <conio.h>
#endif

// Leitura mascarada do CPF: mostra só o primeiro e último dígito, os demais como '*'
void lerCPFmascarado(char *out, size_t size);
// Leitura mascarada da senha: mostra só asteriscos
void lerSenhamascarada(char *out, size_t size);

#ifndef UNIR_H
#define UNIR_H

#include <stddef.h>
#include "banco.h" 

// --- CORES ANSI ---
#define COR_RESET   "\033[0m"
#define COR_VERDE   "\033[1;32m"
#define COR_VERMELHO "\033[1;31m"
#define COR_AZUL    "\033[1;34m"
#define COR_AMARELO "\033[1;33m"
#define COR_CIANO   "\033[1;36m"

// --- INTERFACE VISUAL ---
void limparTela(void);
void pausarTela(void);
void limparEntrada(void);

// Desenha o logo do C-Bank e o título formatado
void cabecalho(const char *titulo); 

// --- TRATAMENTO DE TEXTO ---
void apenasDigitos(const char *src, char *dst);
void trimNewline(char *s);
void removerQuebraLinha(char *str); // Alias para trimNewline (usado no cliente.c)

// --- VALIDAÇÕES ---
int validarCPF(const char *cpf);
int senhaValidaNDigitos(const char *senha, int n);

// --- WRAPPERS DE LEITURA (I/O) ---
// Le uma linha do prompt para `out` (inclui trim). Retorna 1 em sucesso, 0 em EOF/erro.
int lerLinha(const char *prompt, char *out, size_t size);

// Le um inteiro a partir do prompt; retorna 1 em sucesso, 0 em erro/EOF.
int lerInteiro(const char *prompt, int *out);

// Le um double a partir do prompt; retorna 1 em sucesso, 0 em erro/EOF.
int lerDouble(const char *prompt, double *out);

// --- TUI HELPERS ---
// Mensagens coloridas
void printSuccess(const char *fmt, ...);
void printError(const char *fmt, ...);
void printInfo(const char *fmt, ...);

// Barra de progresso simples (0-100)
void progressBar(int percent);

// Caixa de mensagem com título
void boxMessage(const char *title, const char *msg);

// --- LÓGICA DE NEGÓCIO CENTRALIZADA ---
int buscarIndicePorId(Conta *contas, int total, int idBusca);
int buscarIndicePorCPF(Conta *contas, int total, const char *cpf);
int gerarProximoId(Conta *contas, int total);

#endif // UNIR_H