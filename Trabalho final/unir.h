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

// --- LÓGICA DE NEGÓCIO CENTRALIZADA ---
int buscarIndicePorId(Conta *contas, int total, int idBusca);
int buscarIndicePorCPF(Conta *contas, int total, const char *cpf);
int gerarProximoId(Conta *contas, int total);

#endif // UNIR_H