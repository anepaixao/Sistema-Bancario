#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas principais do módulo cliente
typedef struct Usuario {
	char cpf[15];
	char senha[8];
	char agencia[50];
	double saldo;
	char pix[100];
	struct Usuario *prox; // reserva para possíveis extensões
} Usuario;

typedef struct Transacao {
	char tipo[20];
	double valor;
	char descricao[50];
	struct Transacao *prox;
} Transacao;

typedef struct ListaChavesPix {
	char chave[100];
	struct ListaChavesPix *prox;
} ListaChavesPix;

// Ponto de entrada do fluxo do cliente
int clienteExecutar(void);

// Criação e login
Usuario* criar_usuario(void);
int login(Usuario *usuario);
int tela_principal(Usuario *usuario);

// Operações bancárias recursivas / utilitárias
double deposito_recursivo(Usuario *usuario, double valor, int interacoes);
double saque_recursivo(Usuario *usuario, double valor, int tentativas);

// PIX
void pix_aleatorio(Usuario *usuario);
ListaChavesPix* cadastrar_pix_recursivo(ListaChavesPix *lista, Usuario *usuario, int tentativas);
void exibir_chaves_pix_recursivo(ListaChavesPix *lista, int contador);
void liberar_chaves_pix(ListaChavesPix *lista);

// Transações / extrato
Transacao* adicionar_transacao(Transacao *lista, const char *tipo, double valor, const char *descricao);
void extrato_recursivo(Transacao *transacao, Usuario *usuario, int numero);
void liberar_transacoes(Transacao *lista);

// Interface de opções
int opcoes(Usuario *usuario, Transacao **transacoes, ListaChavesPix **chaves, char *escolha);

// Visual
void qr_code(void);
void boleto(void);

#endif // CLIENTE_H
