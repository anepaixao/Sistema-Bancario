#ifndef UNIR_H
#define UNIR_H

#include <stddef.h>
#ifdef _WIN32
#include <conio.h>
#endif
#include "banco.h" 

// Paleta de cores ANSI que uso para padronizar a TUI
#define COR_RESET   "\033[0m"
#define COR_VERDE   "\033[1;32m"
#define COR_VERMELHO "\033[1;31m"
#define COR_AZUL    "\033[1;34m"
#define COR_AMARELO "\033[1;33m"
#define COR_CIANO   "\033[1;36m"
// Texto branco (informações gerais)
#define COR_BRANCO  "\033[1;37m"

// Interface visual básica que eu reutilizo nas telas
void limparTela(void);
void pausarTela(void);
void limparEntrada(void);

// Desenho o logo do C-Bank e um título amigável
void cabecalho(const char *titulo); 

// Utilitários simples de texto que uso o tempo todo
void apenasDigitos(const char *src, char *dst);
void trimNewline(char *s);
void removerQuebraLinha(char *str); // Alias para trimNewline (usado no cliente.c)

// Validações que centralizei para não repetir código
int validarCPF(const char *cpf);
int senhaValidaNDigitos(const char *senha, int n);

// Leitura mascarada do CPF: deixo só o primeiro e o último visíveis
void lerCPFmascarado(char *out, size_t size);
// Leitura mascarada da senha: exibo asteriscos (no Windows dá pra alternar com TAB)
void lerSenhamascarada(char *out, size_t size);


// Wrappers de leitura que escrevi para evitar erros com fgets/scanf
// Le uma linha do prompt para `out` (inclui trim). Retorna 1 em sucesso, 0 em EOF/erro.
int lerLinha(const char *prompt, char *out, size_t size);

// Le um inteiro a partir do prompt; retorna 1 em sucesso, 0 em erro/EOF.
int lerInteiro(const char *prompt, int *out);

// Le um double a partir do prompt; retorna 1 em sucesso, 0 em erro/EOF.
int lerDouble(const char *prompt, double *out);

// Helpers visuais para padronizar mensagens e progresso
// Mensagens coloridas
void printSuccess(const char *fmt, ...);
void printError(const char *fmt, ...);
void printInfo(const char *fmt, ...);

// Barra de progresso simples (0-100)
void progressBar(int percent);

// Caixa de mensagem com título
void boxMessage(const char *title, const char *msg);

// Funções TUI em português que uso nas telas
// Exibe uma mensagem de sucesso verde com o marcador [V]
void msgSucesso(const char *mensagem);
// Exibe uma mensagem de erro vermelha com o marcador [X]
void msgErro(const char *mensagem);
// Barra de carregamento — apenas um wrapper para showLoading
void barraCarregamento(const char *rotulo, int duracaoMs);

// Animação de carregamento (interno)
void showLoading(const char *label, int durationMs);

// Geração de HTML (comprovantes/boletos/contratos) para deixar o projeto mais rico
// Gera um comprovante HTML de transferencia em `Trabalho_final/output/comprovante.html`
// Parametros: idOrigem, idDestino, valor e descricao opcional
int gerarComprovanteHTML(int idOrigem, int idDestino, double valor, const char *descricao);
// Gera um boleto HTML com valor e descricao em `output/boleto.html`
int gerarBoletoHTML(double valor, const char *descricao);
// Gera um contrato simples HTML com titulo e opcionalmente valor em `output/contrato.html`
int gerarContratoHTML(const char *titulo, double valor, int incluirValor);

// Versão com corpo completo: gera contrato HTML com título, valor opcional e texto detalhado
int gerarContratoHTMLComTexto(const char *titulo, double valor, int incluirValor, const char *texto);

// Centralizei aqui buscas e geração de IDs para reaproveitar nos módulos
int buscarIndicePorId(Conta *contas, int total, int idBusca);
int buscarIndicePorCPF(Conta *contas, int total, const char *cpf);
int gerarProximoId(Conta *contas, int total);

// Formato um CPF (somente dígitos) deixando início e fim visíveis
void mascararCPF(const char *cpf, char *out, size_t size);

#endif // UNIR_H