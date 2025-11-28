#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"

// --- FUNÇÕES PRINCIPAIS (Chamadas pela Main) ---
void clienteMenu(Conta **contas, int *total, int *capacidade);
void clienteCriarConta(Conta **contas, int *total, int *capacidade);

// --- FUNÇÕES INTERNAS E TELAS ---

int login(Conta *contas, int total, int *outIdx);
int tela_principal(Conta *contas, int total, int idx);

// --- OPERAÇÕES FINANCEIRAS ---
void deposito(Conta *c, double valor);
void saque(Conta *c, double valor);
void transferencia_bancaria(Conta *contas, int total, int idx);

// --- FUNCIONALIDADES EXTRAS ---
void outras_opcoes(Conta *c);
double deposito_recursivo_simulado(double saldo, double taxa, int meses);

// Funções visuais e interativas
void boleto(void);
void qr_code(void);

#endif