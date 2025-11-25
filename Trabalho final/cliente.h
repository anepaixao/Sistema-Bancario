#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

#define TAM 100

// Definições de configurações
#define NOTIFICACOES_ATIVAS 0x01
#define TRANSACOES_NOTURNAS 0x02
#define CONFIRMACAO_VALORES 0x04
#define SEGURANCA_REFORCADA 0x08

// Estruturas
typedef struct {
    char cpf[15];
    char senha[20];
    char agencia[50];
    double saldo;
    char pix[TAM];
    unsigned char configuracoes;
} Usuario;

// Tipos de função
typedef void (*FuncaoCallback)(const char*);
typedef double (*OperacaoFinanceira)(double, int);
typedef int (*FuncaoValidacao)(const char*);

// Variável global
extern Usuario usuario;

// Declarações de funções
// Autenticação
int login(void);
int tela_principal(void);

// Operações financeiras
double deposito(double n, int interacoes);
double saque(double valor, int max_tentativas);
int extrato(void);
void transferencia_bancaria(void);

// PIX
void pix_aleatorio(void);
int pix_cadastro(void);
int pix_cadastrados(void);
void qr_code(void);
void boleto(void);

// Planos de segurança
void plano_protecaoCONTRATO(void);
void plano_protecaoCOMPROVANTE(void);
void plano_Limitehorarioparatransacoes(void);
void plano_Limitehorarioparatransacoescomprovante(void);
void plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(void);
void plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(void);

// Configurações e utilidades
void outras_opcoes(void);
int opcoes(char *escolha);

// Funções de configuração
void ativar_configuracao(unsigned char *config, unsigned char flag);
void desativar_configuracao(unsigned char *config, unsigned char flag);
int verificar_configuracao(unsigned char config, unsigned char flag);
void exibir_configuracoes_recursivo(unsigned char config, int bit);

// Funções de callback
void notificar_sucesso(const char* mensagem);
void notificar_erro(const char* mensagem);
void notificar_alerta(const char* mensagem);
void executar_com_callback(FuncaoCallback callback, const char* mensagem);

// Processamento de transações
void processar_transacoes_array(double *transacoes, int tamanho, OperacaoFinanceira operacao);
void exibir_historico_recursivo(double *historico, int index, int tamanho);

// Validações
int validar_cpf(const char* cpf);
int validar_senha(const char* senha);
int validar_chave_pix(const char* pix);
int executar_validacao(FuncaoValidacao validacao, const char* dado);
char* formatar_cpf_ponteiro(const char* cpf, char* buffer);

#endif

