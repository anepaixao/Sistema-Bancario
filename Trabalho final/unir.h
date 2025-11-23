#ifndef UNIR_H
#define UNIR_H

#include <stddef.h>
#include "banco.h"

int validarCPF(const char *cpf);

void apenasDigitos(const char *src, char *dst);

void limparTela(void);

void pausarTela(void);

// Remove qualquer entrada pendente até o fim da linha (stdin)
void limparEntrada(void);

// Verifica se a senha tem exatamente `n` digitos numéricos
int senhaValidaNDigitos(const char *senha, int n);

// Remove '\n' final de uma string (se presente)
void trimNewline(char *s);

// Buscas utilitárias sobre o vetor de contas
int buscarIndicePorId(Conta *contas, int total, int idBusca);

int buscarIndicePorCPF(Conta *contas, int total, const char *cpf);

// Gera próximo id baseado no maior id existente (assume 1000 se não houver contas)
int gerarProximoId(Conta *contas, int total);

#endif // UNIR_H
