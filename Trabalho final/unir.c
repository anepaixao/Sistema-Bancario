#include "unir.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Remove non-digit characters from src, write digits to dst (null-terminated).
// Stops after collecting 11 digits (CPF length) to match previous behavior.
void apenasDigitos(const char *src, char *dst) {
    if (!src || !dst) return;
    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) {
        if (isdigit((unsigned char)src[i])) {
            dst[j++] = src[i];
            if (j >= 11) break; // CPF tem 11 dígitos
        }
    }
    dst[j] = '\0';
}

// Valida CPF (aceita com ou sem pontuação)
int validarCPF(const char *cpf) {
    if (!cpf) return 0;
    char digitos[12];
    apenasDigitos(cpf, digitos);
    if (strlen(digitos) != 11) return 0;

    // verificar se todos os dígitos são iguais
    int all_equal = 1;
    for (int i = 1; i < 11; i++) {
        if (digitos[i] != digitos[0]) { all_equal = 0; break; }
    }
    if (all_equal) return 0;

    int nums[11];
    for (int i = 0; i < 11; i++) nums[i] = digitos[i] - '0';

    // primeiro dígito verificador
    int sum = 0;
    for (int i = 0; i < 9; i++) sum += nums[i] * (10 - i);
    int rem = sum % 11;
    int d1 = (rem < 2) ? 0 : 11 - rem;
    if (d1 != nums[9]) return 0;

    // segundo dígito verificador
    sum = 0;
    for (int i = 0; i < 10; i++) sum += nums[i] * (11 - i);
    rem = sum % 11;
    int d2 = (rem < 2) ? 0 : 11 - rem;
    if (d2 != nums[10]) return 0;

    return 1;
}

// Limpa a tela de forma portátil
void limparTela(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

// Pausa a execução esperando ENTER
void pausarTela(void) {
    int c;
    printf("Pressione ENTER para continuar...");
    // consumir qualquer caracter pendente até newline
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Remove qualquer entrada pendente até o fim da linha (stdin)
void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Verifica se a senha tem exatamente `n` digitos numéricos
int senhaValidaNDigitos(const char *senha, int n) {
    if (!senha || n <= 0) return 0;
    int len = (int)strlen(senha);
    if (len != n) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit((unsigned char)senha[i])) return 0;
    }
    return 1;
}

// Remove '\n' final de uma string (se presente)
void trimNewline(char *s) {
    if (!s) return;
    size_t i = strlen(s);
    if (i == 0) return;
    if (s[i-1] == '\n') s[i-1] = '\0';
}
