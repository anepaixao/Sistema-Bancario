#ifndef cliente_h
#define cliente_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototipos das funcoes implementadas em cliente.c
int login(void);
int tela_principal(void);
double deposito(double valor);
void pix_aleatorio(void);
int pix_cadastro(void);
int pix_cadastrados(void);
void qr_code(void);
void boleto(void);
int extrato(void);
double saque(double valor);
int opcoes(char *escolha);

// Novas funções que recebem o vetor de contas (encapsulamento sem globais)
void clienteMenu(Conta **contas, int *total, int *capacidade);
void clienteCriarConta(Conta **contas, int *total, int *capacidade);

#endif
