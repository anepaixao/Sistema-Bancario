#ifndef cliente_h
#define cliente_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"

// Prototipos das funcoes implementadas em cliente.c
// autentica cliente e retorna índice da conta autenticada via outIdx (retorna 1=ok,0=falha)
int login(Conta *contas, int total, int *outIdx);
// Tela principal do cliente — recebe o vetor de contas e o índice da conta autenticada
int tela_principal(Conta *contas, int total, int idx);
// Funções que atuam diretamente sobre uma conta
int clienteDepositar(Conta *c, double valor);
int clienteSacar(Conta *c, double valor);

// Transferencia entre contas pelo índice (retorna 1 se ok)
int clienteTransferir(Conta *contas, int total, int fromIdx, int toIdx, double valor);

// Simula rendimento recursivamente: aplica `taxa` mensal sobre `saldo` por `meses` meses
double simularRendimentoRecursivo(double saldo, double taxa, int meses);

// Bloqueio de segurança: seta a flag de bloqueio na conta (1=sucesso)
int clienteBloquearConta(Conta *c);

// Alterna bloqueio (bloquear/desbloquear) — usa operações bitwise
int clienteToggleBloqueio(Conta *c);

// Alterar senha da conta (valida com unir.h)
int clienteAlterarSenha(Conta *c);

// Novas funções que recebem o vetor de contas (encapsulamento sem globais)
void clienteMenu(Conta **contas, int *total, int *capacidade);
void clienteCriarConta(Conta **contas, int *total, int *capacidade);

#endif