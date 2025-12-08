#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"

// Cabeçalho do módulo do cliente — funções expostas para acesso pela main
void clienteMenu(Conta **contas, int *total, int *capacidade);
void clienteCriarConta(Conta **contas, int *total, int *capacidade);

// Internas do módulo (telas/fluxo), utilizadas dentro do próprio cliente

int login(Conta *contas, int total, int *outIdx);
int tela_principal(Conta *contas, int total, int idx);

// Operações financeiras básicas
void deposito(Conta *c, double valor);
void saque(Conta *c, double valor);
void transferencia_bancaria(Conta *contas, int total, int idx);

// Extras para enriquecer o projeto (configurações, simulação)
void outras_opcoes(Conta *c);
double deposito_recursivo_simulado(double saldo, double taxa, int meses);

// Funções visuais para o terminal (apoio/ilustração)
void boleto(void);
void qr_code(void);

#endif