#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "unir.h"
#include "banco.h"

// Leitura mascarada do CPF com toggle (TAB):
// Optei por mostrar só o primeiro e o último dígito; TAB alterna visibilidade
void lerCPFmascarado(char *out, size_t size) {
#ifdef _WIN32
    char buf[32] = {0};
    int len = 0;
    int show = 0; // 0: mascarado, 1: visivel
    printf("Digite seu CPF [TAB mostra/oculta]: ");
    while (len < 11) {
        int ch = _getch();
        if (ch == 13 || ch == '\n') break;
        // TAB (ASCII 9) alterna visibilidade
        if (ch == 9) {
            show = !show;
            // Reimprime linha atual conforme estado
            printf("\r");
            printf("Digite seu CPF [TAB mostra/oculta]: ");
            for (int i = 0; i < len; i++) {
                if (show) putchar(buf[i]);
                else {
                    if (i == 0 || i == len-1) putchar(buf[i]);
                    else putchar('*');
                }
            }
            continue;
        }
        if (ch == 8 && len > 0) { // backspace
            len--; buf[len] = '\0';
            printf("\b \b");
            continue;
        }
        if (ch >= '0' && ch <= '9') {
            buf[len++] = (char)ch;
            if (show) {
                printf("%c", ch);
            } else {
                if (len == 1) printf("%c", ch);
                else if (len == 11) printf("%c", ch);
                else printf("*");
            }
        }
    }
    buf[len] = '\0';
    strncpy(out, buf, size-1); out[size-1] = '\0';
    printf("\n");
#else
    // POSIX: leitura char-a-char com termios e toggle via TAB
    #include <termios.h>
    #include <unistd.h>
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char buf[32] = {0};
    int len = 0;
    int show = 0;
    printf("Digite seu CPF [TAB mostra/oculta]: ");
    while (len < 11) {
        int ch = getchar();
        if (ch == '\n' || ch == '\r') break;
        if (ch == 9) { // TAB
            show = !show;
            printf("\r");
            printf("Digite seu CPF [TAB mostra/oculta]: ");
            for (int i = 0; i < len; i++) {
                if (show) putchar(buf[i]);
                else {
                    if (i == 0 || i == len-1) putchar(buf[i]);
                    else putchar('*');
                }
            }
            fflush(stdout);
            continue;
        }
        if (ch == 127 || ch == 8) { // backspace
            if (len > 0) {
                len--; buf[len] = '\0';
                printf("\b \b");
            }
            continue;
        }
        if (ch >= '0' && ch <= '9') {
            buf[len++] = (char)ch;
            if (show) putchar(ch);
            else {
                if (len == 1) putchar(ch);
                else if (len == 11) putchar(ch);
                else putchar('*');
            }
            fflush(stdout);
        }
    }
    buf[len] = '\0';
    strncpy(out, buf, size-1); out[size-1] = '\0';
    printf("\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

// Leitura mascarada da senha com toggle (TAB) — uso asteriscos por padrão
void lerSenhamascarada(char *out, size_t size) {
#ifdef _WIN32
    char buf[64] = {0};
    int len = 0;
    int show = 0; // 0: mascarado, 1: visivel
    printf("Digite sua senha [TAB mostra/oculta]: ");
    while (len < (int)size-1) {
        int ch = _getch();
        if (ch == 13 || ch == '\n') break;
        // TAB alterna visibilidade
        if (ch == 9) {
            show = !show;
            printf("\r");
            printf("Digite sua senha [TAB mostra/oculta]: ");
            for (int i = 0; i < len; i++) {
                putchar(show ? buf[i] : '*');
            }
            continue;
        }
        if (ch == 8 && len > 0) { // backspace
            len--; buf[len] = '\0';
            printf("\b \b");
            continue;
        }
        if (ch >= 32 && ch <= 126) {
            buf[len++] = (char)ch;
            putchar(show ? (char)ch : '*');
        }
    }
    buf[len] = '\0';
    strncpy(out, buf, size-1); out[size-1] = '\0';
    printf("\n");
#else
    // POSIX: leitura char-a-char com termios e toggle via TAB
    #include <termios.h>
    #include <unistd.h>
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char buf[64] = {0};
    int len = 0;
    int show = 0;
    printf("Digite sua senha [TAB mostra/oculta]: ");
    while (len < (int)size-1) {
        int ch = getchar();
        if (ch == '\n' || ch == '\r') break;
        if (ch == 9) { // TAB
            show = !show;
            printf("\r");
            printf("Digite sua senha [TAB mostra/oculta]: ");
            for (int i = 0; i < len; i++) {
                putchar(show ? buf[i] : '*');
            }
            fflush(stdout);
            continue;
        }
        if (ch == 127 || ch == 8) { // backspace
            if (len > 0) {
                len--; buf[len] = '\0';
                printf("\b \b");
            }
            continue;
        }
        if (ch >= 32 && ch <= 126) {
            buf[len++] = (char)ch;
            putchar(show ? (char)ch : '*');
            fflush(stdout);
        }
    }
    buf[len] = '\0';
    strncpy(out, buf, size-1); out[size-1] = '\0';
    printf("\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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

// Minhas funções de TUI (visual) para deixar tudo padronizado

// Desenho o cabeçalho com o logo do C-Bank
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

// Removo o \n do final (mantive este nome por compatibilidade)
void trimNewline(char *s) {
    if (!s) return;
    size_t i = strlen(s);
    if (i == 0) return;
    if (s[i-1] == '\n') s[i-1] = '\0';
}

// Alias para a mesma função (mantive por retrocompatibilidade)
void removerQuebraLinha(char *s) {
    trimNewline(s);
}

// Interface de usuário (limpar tela, pausar, limpar entrada)

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
    printf("\n%sPressione ENTER para continuar...%s", COR_AMARELO, COR_RESET);
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

// Validações que uso em vários pontos (CPF e senha)

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

// Exibo CPF mascarado na tela: começo e fim visíveis, meio com '*'
void mascararCPF(const char *cpf, char *out, size_t size) {
    if (!out || size == 0) return;
    if (!cpf) { out[0] = '\0'; return; }
    size_t n = strlen(cpf);
    if (n == 0) { out[0] = '\0'; return; }
    // Garantir espaço suficiente
    size_t limit = size - 1;
    if (limit == 0) return;
    size_t write = n < limit ? n : limit;
    for (size_t i = 0; i < write; i++) {
        if (i == 0 || i == write - 1) out[i] = cpf[i];
        else out[i] = '*';
    }
    out[write] = '\0';
}

// Lógica centralizada: buscas e geração de IDs

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

// Wrappers de leitura para facilitar o uso de fgets/strtol/strtod
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

// Implementação dos helpers visuais (mensagens, barra, caixas)
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
    printf("%s", COR_BRANCO);
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
    printf("%s[", COR_VERDE);
    for (int i = 0; i < filled; ++i) printf("#");
    for (int i = filled; i < width; ++i) printf("-");
    printf("] %3d%%%s\r", percent, COR_RESET);
    fflush(stdout);
    if (percent == 100) printf("\n");
}

void boxMessage(const char *title, const char *msg) {
    int w = 60;
    // top border
    printf("%s+", COR_VERDE);
    for (int i = 0; i < w; i++) printf("-");
    printf("+%s\n", COR_RESET);
    // title
    printf("%s| %s%*s |%s\n", COR_VERDE, COR_AZUL, - (w-2), title, COR_RESET);
    // separator
    printf("%s|", COR_VERDE);
    for (int i = 0; i < w; i++) printf("-");
    printf("|%s\n", COR_RESET);
    // message (wrap simple)
    size_t len = strlen(msg);
    for (size_t i = 0; i < len; i += (w - 2)) {
        char buf[128] = {0};
        strncpy(buf, msg + i, (size_t)(w - 2));
        printf("%s| %s%*s |%s\n", COR_VERDE, COR_RESET, - (w-2), buf, COR_RESET);
    }
    // bottom border
    printf("%s+", COR_VERDE);
    for (int i = 0; i < w; i++) printf("-");
    printf("+%s\n", COR_RESET);
}

// Animação de carregamento com barra de progresso (verde)
void showLoading(const char *label, int durationMs) {
    if (!label) label = "Carregando";
    int steps = 30;
    int stepMs = durationMs > 0 ? durationMs / steps : 30;
    printf("%s%s...%s\n", COR_VERDE, label, COR_RESET);
    for (int i = 0; i <= steps; ++i) {
        int percent = (i * 100) / steps;
        progressBar(percent);
#if defined(_WIN32) || defined(_WIN64)
        Sleep(stepMs);
#else
        struct timespec ts; ts.tv_sec = stepMs / 1000; ts.tv_nsec = (long)(stepMs % 1000) * 1000000L;
        nanosleep(&ts, NULL);
#endif
    }
}

// Implementações em português para manter a experiência consistente
// Exibe uma mensagem de sucesso verde com marcador [V]
void msgSucesso(const char *mensagem) {
    if (!mensagem) return;
    printf("%s[V] SUCESSO: %s%s\n", COR_VERDE, mensagem, COR_RESET);
}

// Exibe uma mensagem de erro vermelha com marcador [X]
void msgErro(const char *mensagem) {
    if (!mensagem) return;
    printf("%s[X] ERRO: %s%s\n", COR_VERMELHO, mensagem, COR_RESET);
}

// Barra de carregamento — wrapper em português sobre showLoading
void barraCarregamento(const char *rotulo, int duracaoMs) {
    showLoading(rotulo, duracaoMs);
}

// Geração de HTML (comprovante, boleto e contrato) com auto-abertura no navegador
// Gera um comprovante HTML de transferencia em output/comprovante.html
int gerarComprovanteHTML(int idOrigem, int idDestino, double valor, const char *descricao) {
    const char *path = "output/comprovante.html";
    FILE *f = fopen(path, "w");
    if (!f) { msgErro("Falha ao criar comprovante HTML."); return 0; }
    const char *desc = (descricao && descricao[0]) ? descricao : "Transferencia TED";
    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    char datahora[64];
    if (tmv) strftime(datahora, sizeof(datahora), "%d/%m/%Y %H:%M:%S", tmv); else strncpy(datahora, "--/--/---- --:--:--", sizeof(datahora));
    fprintf(f,
"<!DOCTYPE html>\n"
"<html lang=\"pt-BR\">\n"
"<head>\n"
"  <meta charset=\"utf-8\">\n"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"  <title>Comprovante C-Bank</title>\n"
"  <style>\n"
"    body{background:#000;color:#fff;font-family:Arial,Helvetica,sans-serif;padding:20px;}\n"
"    .card{max-width:640px;margin:0 auto;border:2px solid #00ff66;border-radius:8px;padding:16px;}\n"
"    .logo{color:#00ff66;font-weight:bold;margin-bottom:8px;}\n"
"    .title{color:#00ff66;font-size:20px;margin:8px 0;}\n"
"    .row{display:flex;justify-content:space-between;margin:6px 0;}\n"
"    .label{color:#ffeb3b;}\n"
"    .success{color:#00ff66;margin-top:12px;}\n"
"    .footer{margin-top:16px;font-size:12px;color:#aaa;}\n"
"  </style>\n"
"</head>\n"
"<body>\n"
"  <div class=\"card\">\n"
"    <div class=\"logo\">C-Bank</div>\n"
"    <div class=\"title\">[V] SUCESSO — Comprovante de Transferência</div>\n"
"    <div class=\"row\"><div class=\"label\">Origem</div><div>Conta %d</div></div>\n"
"    <div class=\"row\"><div class=\"label\">Destino</div><div>Conta %d</div></div>\n"
"    <div class=\"row\"><div class=\"label\">Valor</div><div>R$ %.2f</div></div>\n"
"    <div class=\"row\"><div class=\"label\">Descrição</div><div>%s</div></div>\n"
"    <div class=\"success\">Transação concluída com êxito.</div>\n"
"    <div class=\"footer\">Autenticidade: C-Bank • %s</div>\n"
"  </div>\n"
"</body>\n"
"</html>\n",
            idOrigem, idDestino, valor, desc, datahora);
    fclose(f);
    msgSucesso("Comprovante gerado em output/comprovante.html");
    // Abre automaticamente no navegador padrão
#if defined(_WIN32) || defined(_WIN64)
    // Use 'start' para abrir com o navegador padrão no Windows
    system("start \"\" \"output/comprovante.html\"");
#else
    // Tentativa em ambientes Linux/macOS
    // Linux: xdg-open; macOS: open
    int rc = system("xdg-open \"output/comprovante.html\" >/dev/null 2>&1 &");
    if (rc != 0) {
        system("open \"output/comprovante.html\" >/dev/null 2>&1 &");
    }
#endif
    return 1;
}

// Gera um boleto HTML com valor e descricao
int gerarBoletoHTML(double valor, const char *descricao) {
    const char *path = "output/boleto.html";
    FILE *f = fopen(path, "w");
    if (!f) { msgErro("Falha ao criar boleto HTML."); return 0; }
    const char *desc = (descricao && descricao[0]) ? descricao : "Boleto";
    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    char datahora[64];
    if (tmv) strftime(datahora, sizeof(datahora), "%d/%m/%Y %H:%M:%S", tmv); else strncpy(datahora, "--/--/---- --:--:--", sizeof(datahora));
    fprintf(f,
"<!DOCTYPE html>\n"
"<html lang=\"pt-BR\">\n"
"<head>\n"
"  <meta charset=\"utf-8\">\n"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"  <title>Boleto C-Bank</title>\n"
"  <style>body{background:#000;color:#fff;font-family:Arial;padding:20px;} .card{max-width:640px;margin:0 auto;border:2px solid #00ff66;border-radius:8px;padding:16px;} .logo{color:#00ff66;font-weight:bold;margin-bottom:8px;} .title{color:#00ff66;font-size:20px;margin:8px 0;} .row{display:flex;justify-content:space-between;margin:6px 0;} .label{color:#ffeb3b;} .footer{margin-top:16px;font-size:12px;color:#aaa;} .barcode{margin-top:14px;background:#fff;color:#000;padding:8px;border-radius:6px;font-family:monospace;letter-spacing:2px} .barcode .blk{display:inline-block;background:#000;height:24px;width:4px;margin-right:2px} </style>\n"
"</head><body>\n"
"  <div class=\"card\">\n"
"    <div class=\"logo\">C-Bank</div>\n"
"    <div class=\"title\">Boleto</div>\n"
"    <div class=\"row\"><div class=\"label\">Descrição</div><div>%s</div></div>\n"
"    <div class=\"row\"><div class=\"label\">Valor</div><div>R$ %.2f</div></div>\n"
"    <div class=\"row\"><div class=\"label\">Código de Barras</div><div>\n"
"      <div class=\"barcode\">\n"
"        <span class=\"blk\"></span><span class=\"blk\" style=\"width:2px\"></span><span class=\"blk\"></span><span class=\"blk\" style=\"width:6px\"></span><span class=\"blk\"></span><span class=\"blk\" style=\"width:3px\"></span>\n"
"        <span class=\"blk\"></span><span class=\"blk\" style=\"width:5px\"></span><span class=\"blk\"></span><span class=\"blk\" style=\"width:2px\"></span><span class=\"blk\"></span><span class=\"blk\" style=\"width:4px\"></span>\n"
"      </div>\n"
"    </div></div>\n"
"    <div class=\"footer\">Autenticidade: C-Bank • %s</div>\n"
"  </div>\n"
"</body></html>\n",
            desc, valor, datahora);
    fclose(f);
    msgSucesso("Boleto gerado em output/boleto.html");
#if defined(_WIN32) || defined(_WIN64)
    system("start \"\" \"output/boleto.html\"");
#else
    int rc = system("xdg-open \"output/boleto.html\" >/dev/null 2>&1 &");
    if (rc != 0) { system("open \"output/boleto.html\" >/dev/null 2>&1 &"); }
#endif
    return 1;
}

// Gera um contrato simples HTML com titulo e opcionalmente valor
int gerarContratoHTML(const char *titulo, double valor, int incluirValor) {
    const char *path = "output/contrato.html";
    FILE *f = fopen(path, "w");
    if (!f) { msgErro("Falha ao criar contrato HTML."); return 0; }
    const char *tit = (titulo && titulo[0]) ? titulo : "Contrato";
    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    char datahora[64];
    if (tmv) strftime(datahora, sizeof(datahora), "%d/%m/%Y %H:%M:%S", tmv); else strncpy(datahora, "--/--/---- --:--:--", sizeof(datahora));
    char valorRow[128];
    if (incluirValor) {
        snprintf(valorRow, sizeof(valorRow), "<div class=\"row\"><div class=\"label\">Valor</div><div>R$ %.2f</div></div>", valor);
    } else {
        valorRow[0] = '\0';
    }
    fprintf(f,
"<!DOCTYPE html>\n"
"<html lang=\"pt-BR\">\n"
"<head>\n"
"  <meta charset=\"utf-8\">\n"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"  <title>%s</title>\n"
"  <style>body{background:#000;color:#fff;font-family:Arial;padding:20px;} .card{max-width:720px;margin:0 auto;border:2px solid #00ff66;border-radius:8px;padding:16px;} .logo{color:#00ff66;font-weight:bold;margin-bottom:8px;} .title{color:#00ff66;font-size:22px;margin:8px 0;} .row{display:flex;justify-content:space-between;margin:6px 0;} .label{color:#ffeb3b;} .footer{margin-top:16px;font-size:12px;color:#aaa;} .text{line-height:1.5;margin-top:8px;} </style>\n"
"</head><body>\n"
"  <div class=\"card\">\n"
"    <div class=\"logo\">C-Bank</div>\n"
"    <div class=\"title\">%s</div>\n"
"    %s\n"
"    <div class=\"text\">Este contrato foi emitido pelo C-Bank para confirmação da adesão ao serviço selecionado.</div>\n"
"    <div class=\"footer\">Autenticidade: C-Bank • %s</div>\n"
"  </div>\n"
"</body></html>\n",
            tit, tit,
            valorRow,
            datahora);
    fclose(f);
    msgSucesso("Contrato gerado em output/contrato.html");
#if defined(_WIN32) || defined(_WIN64)
    system("start \"\" \"output/contrato.html\"");
#else
    int rc = system("xdg-open \"output/contrato.html\" >/dev/null 2>&1 &");
    if (rc != 0) { system("open \"output/contrato.html\" >/dev/null 2>&1 &"); }
#endif
    return 1;
}

// Gera um contrato HTML incluindo um corpo de texto completo (parágrafos/itens)
int gerarContratoHTMLComTexto(const char *titulo, double valor, int incluirValor, const char *texto) {
    const char *path = "output/contrato.html";
    FILE *f = fopen(path, "w");
    if (!f) { msgErro("Falha ao criar contrato HTML."); return 0; }
    const char *tit = (titulo && titulo[0]) ? titulo : "Contrato";
    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    char datahora[64];
    if (tmv) strftime(datahora, sizeof(datahora), "%d/%m/%Y %H:%M:%S", tmv); else strncpy(datahora, "--/--/---- --:--:--", sizeof(datahora));

    // Se não veio texto, usa uma mensagem padrão
    const char *body = (texto && texto[0]) ? texto : "<p>Contrato sem corpo informado.</p>";
    char valorRow[128];
    if (incluirValor) {
        snprintf(valorRow, sizeof(valorRow), "<div class=\"row\"><div class=\"label\">Valor</div><div>R$ %.2f</div></div>", valor);
    } else {
        valorRow[0] = '\0';
    }

    fprintf(f,
"<!DOCTYPE html>\n"
"<html lang=\"pt-BR\">\n"
"<head>\n"
"  <meta charset=\"utf-8\">\n"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"  <title>%s</title>\n"
"  <style>body{background:#000;color:#fff;font-family:Arial;padding:20px;} .card{max-width:860px;margin:0 auto;border:2px solid #00ff66;border-radius:8px;padding:16px;} .logo{color:#00ff66;font-weight:bold;margin-bottom:8px;} .title{color:#00ff66;font-size:22px;margin:8px 0;} .row{display:flex;justify-content:space-between;margin:6px 0;} .label{color:#ffeb3b;} .footer{margin-top:16px;font-size:12px;color:#aaa;} .text{line-height:1.6;margin-top:10px;white-space:pre-wrap} .section{margin-top:8px} .muted{color:#bbb;font-size:13px} </style>\n"
"</head><body>\n"
"  <div class=\"card\">\n"
"    <div class=\"logo\">C-Bank</div>\n"
"    <div class=\"title\">%s</div>\n"
"    %s\n"
"    <div class=\"text\">%s</div>\n"
"    <div class=\"footer\">Autenticidade: C-Bank • %s</div>\n"
"  </div>\n"
"</body></html>\n",
            tit, tit,
            valorRow,
            body, datahora);
    fclose(f);
    msgSucesso("Contrato gerado em output/contrato.html");
#if defined(_WIN32) || defined(_WIN64)
    system("start \"\" \"output/contrato.html\"");
#else
    int rc = system("xdg-open \"output/contrato.html\" >/dev/null 2>&1 &");
    if (rc != 0) { system("open \"output/contrato.html\" >/dev/null 2>&1 &"); }
#endif
    return 1;
}