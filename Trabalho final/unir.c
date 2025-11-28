#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "unir.h"
#include "banco.h"

// --- FUNÇÕES VISUAIS (TUI) ---

// Desenha o cabeçalho com o logo do C-Bank
void cabecalho(const char *titulo) {
    limparTela();
    
    // Cor Verde (Identidade do C-Bank)
    printf("%s", COR_VERDE); 
    printf("   ______      ____              _    \n");
    printf("  / ____/     / __ )____ _____  | | __\n");
    printf(" / /   ______/ __  / __ `/ __ \\ | |/ /\n");
    printf("/ /___/_____/ /_/ / /_/ / / / / |   < \n");
    printf("\\____/     /_____/\\__,_/_/ /_/  |_|\\_\\\n");
    printf("%s", COR_RESET); 
    
    printf("  %s\n", SLOGAN_BANCO); // Constante do banco.h
    
    // Linhas divisórias verdes
    printf("%s==================================================\n", COR_VERDE);
    printf("   >> %s <<   \n", titulo);
    printf("==================================================%s\n", COR_RESET);
}

// --- TRATAMENTO DE TEXTO ---

// Remove caracteres não numéricos
void apenasDigitos(const char *src, char *dst) {
    if (!src || !dst) return;
    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) {
        if (isdigit((unsigned char)src[i])) {
            dst[j++] = src[i];
            if (j >= 11) break; // Limita ao tamanho do CPF
        }
    }
    dst[j] = '\0';
}

// Remove o \n do final (Wrapper para manter compatibilidade de nomes)
void trimNewline(char *s) {
    if (!s) return;
    size_t i = strlen(s);
    if (i == 0) return;
    if (s[i-1] == '\n') s[i-1] = '\0';
}

// Alias para a mesma função (caso algum código use esse nome)
void removerQuebraLinha(char *s) {
    trimNewline(s);
}

// --- INTERFACE DE USUÁRIO ---

void limparTela(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pausarTela(void) {
    printf("\nPressione ENTER para continuar...");
    // Consome buffer até achar enter ou fim
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// --- VALIDAÇÕES ---

int validarCPF(const char *cpf) {
    if (!cpf) return 0;
    char digitos[12];
    apenasDigitos(cpf, digitos);
    if (strlen(digitos) != 11) return 0;

    // Verificar dígitos iguais (111.111.111-11)
    int all_equal = 1;
    for (int i = 1; i < 11; i++) {
        if (digitos[i] != digitos[0]) { all_equal = 0; break; }
    }
    if (all_equal) return 0;

    int nums[11];
    for (int i = 0; i < 11; i++) nums[i] = digitos[i] - '0';

    // Primeiro dígito verificador
    int sum = 0;
    for (int i = 0; i < 9; i++) sum += nums[i] * (10 - i);
    int rem = sum % 11;
    int d1 = (rem < 2) ? 0 : 11 - rem;
    if (d1 != nums[9]) return 0;

    // Segundo dígito verificador
    sum = 0;
    for (int i = 0; i < 10; i++) sum += nums[i] * (11 - i);
    rem = sum % 11;
    int d2 = (rem < 2) ? 0 : 11 - rem;
    if (d2 != nums[10]) return 0;

    return 1;
}

int senhaValidaNDigitos(const char *senha, int n) {
    if (!senha || n <= 0) return 0;
    int len = (int)strlen(senha);
    if (len != n) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit((unsigned char)senha[i])) return 0;
    }
    return 1;
}

// --- LÓGICA DE NEGÓCIO CENTRALIZADA ---

int buscarIndicePorId(Conta *contas, int total, int idBusca) {
    if (!contas) return -1;
    for (int i = 0; i < total; i++) {
        if (contas[i].id == idBusca) return i;
    }
    return -1;
}

int buscarIndicePorCPF(Conta *contas, int total, const char *cpf) {
    if (!contas || !cpf) return -1;
    for (int i = 0; i < total; i++) {
        if (strcmp(contas[i].cpf, cpf) == 0) return i;
    }
    return -1;
}

int gerarProximoId(Conta *contas, int total) {
    int max = 1000;
    if (!contas || total <= 0) return max + 1;
    for (int i = 0; i < total; i++) {
        if (contas[i].id > max) max = contas[i].id;
    }
    return max + 1;
}