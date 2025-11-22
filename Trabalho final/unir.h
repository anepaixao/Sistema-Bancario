#ifndef UNIR_H
#define UNIR_H

#include <stddef.h>

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

#endif // UNIR_H
