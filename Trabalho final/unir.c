#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "unir.h"
#include "banco.h"

// Leitura mascarada do CPF: mostra só o primeiro e último dígito, os demais como '*'
void lerCPFmascarado(char *out, size_t size) {
#ifdef _WIN32
    char buf[32] = {0};
    int len = 0;
    printf("Digite seu CPF: ");
    while (len < 11) {
        int ch = _getch();
        if (ch == 13 || ch == '\n') break;
        if (ch == 8 && len > 0) { // backspace
            len--; buf[len] = '\0';
            printf("\b \b");
            continue;
        }
        if (ch >= '0' && ch <= '9') {
            buf[len++] = (char)ch;
            if (len == 1) printf("%c", ch);
            else if (len == 11) printf("%c", ch);
            else printf("*");
        }
    }
    buf[len] = '\0';
    strncpy(out, buf, size-1); out[size-1] = '\0';
    printf("\n");
#else
    // Fallback: leitura normal
    fgets(out, (int)size, stdin); trimNewline(out);
#endif
}

// Leitura mascarada da senha: mostra só asteriscos
void lerSenhamascarada(char *out, size_t size) {
#ifdef _WIN32
    char buf[32] = {0};
    int len = 0;
    printf("Digite sua senha: ");
    while (len < (int)size-1) {
        int ch = _getch();
        if (ch == 13 || ch == '\n') break;
        if (ch == 8 && len > 0) { // backspace
            len--; buf[len] = '\0';
            printf("\b \b");
            continue;
        }
        if (ch >= 32 && ch <= 126) {
            buf[len++] = (char)ch;
            printf("*");
        }
    }
    buf[len] = '\0';
    strncpy(out, buf, size-1); out[size-1] = '\0';
    printf("\n");
#else
    // Fallback: leitura normal
    fgets(out, (int)size, stdin); trimNewline(out);
#endif
}

// Detecta se stdin é um terminal interativo
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <unistd.h>
#endif

static int stdin_is_tty(void) {
#if defined(_WIN32) || defined(_WIN64)
    // On Windows, assume interactive in typical usage (avoids toolchain-specific fileno/_fileno issues)
    return 1;
#else
    return isatty(fileno(stdin));
#endif
}

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
    // Se stdin não é um terminal (ex.: redirecionado), não pausar
    if (!stdin_is_tty()) return;
    printf("\nPressione ENTER para continuar...");
    char buf[64];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        clearerr(stdin);
    }
}

void limparEntrada(void) {
    // Se stdin não é um terminal, nada a limpar
    if (!stdin_is_tty()) return;
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

// --- WRAPPERS DE LEITURA ---
int lerLinha(const char *prompt, char *out, size_t size) {
    if (prompt) printf("%s", prompt);
    if (!out || size == 0) return 0;
    if (fgets(out, size, stdin) == NULL) return 0;
    trimNewline(out);
    return 1;
}

int lerInteiro(const char *prompt, int *out) {
    if (!out) return 0;
    char buf[64];
    if (prompt) printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    trimNewline(buf);
    char *endptr = NULL;
    long v = strtol(buf, &endptr, 10);
    if (endptr == buf || *endptr != '\0') return 0;
    *out = (int)v;
    return 1;
}

int lerDouble(const char *prompt, double *out) {
    if (!out) return 0;
    char buf[128];
    if (prompt) printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    trimNewline(buf);
    char *endptr = NULL;
    double v = strtod(buf, &endptr);
    if (endptr == buf || *endptr != '\0') return 0;
    *out = v;
    return 1;
}

// --- TUI HELPERS IMPLEMENTATION ---
void printSuccess(const char *fmt, ...) {
    va_list ap;
    printf("%s", COR_VERDE);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("%s\n", COR_RESET);
}

void printError(const char *fmt, ...) {
    va_list ap;
    printf("%s", COR_VERMELHO);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("%s\n", COR_RESET);
}

void printInfo(const char *fmt, ...) {
    va_list ap;
    printf("%s", COR_CIANO);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("%s\n", COR_RESET);
}

void progressBar(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    int width = 30;
    int filled = (percent * width) / 100;
    printf("[");
    for (int i = 0; i < filled; ++i) printf("#");
    for (int i = filled; i < width; ++i) printf("-");
    printf("] %3d%%\r", percent);
    fflush(stdout);
    if (percent == 100) printf("\n");
}

void boxMessage(const char *title, const char *msg) {
    int w = 60;
    // top border
    printf("%s+", COR_CIANO);
    for (int i = 0; i < w; i++) printf("-");
    printf("+%s\n", COR_RESET);
    // title
    printf("%s| %s%*s |%s\n", COR_CIANO, COR_AZUL, - (w-2), title, COR_RESET);
    // separator
    printf("%s|", COR_CIANO);
    for (int i = 0; i < w; i++) printf("-");
    printf("|%s\n", COR_RESET);
    // message (wrap simple)
    size_t len = strlen(msg);
    for (size_t i = 0; i < len; i += (w - 2)) {
        char buf[128] = {0};
        strncpy(buf, msg + i, (size_t)(w - 2));
        printf("%s| %s%*s |%s\n", COR_CIANO, COR_RESET, - (w-2), buf, COR_RESET);
    }
    // bottom border
    printf("%s+", COR_CIANO);
    for (int i = 0; i < w; i++) printf("-");
    printf("+%s\n", COR_RESET);
}