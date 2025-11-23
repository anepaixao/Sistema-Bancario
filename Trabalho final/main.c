#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"
#include "cliente.h"
#include "unir.h"
#include <time.h>

// Persistência: protótipos
int salvarDados(const Conta *contas, int total, const char *arquivo);
int carregarDados(Conta **contas, int *total, const char *arquivo);
void registrarLog(int idConta, const char *descricao);

// Vetor de contas e total (carregado no inicio)
    Conta *contas = NULL;
    int totalContas = 0;
    int capacidadeContas = 0;

int main(void) {
    int opcao;

    // carregar dados antes do menu
    if (carregarDados(&contas, &totalContas, "dados.bin")) {
        capacidadeContas = totalContas > 0 ? totalContas : 2;
        printf("%d contas carregadas de dados.bin\n", totalContas);
    } else {
        printf("Nenhum arquivo de dados encontrado ou vazio. Iniciando com 0 contas.\n");
        capacidadeContas = 2;
    }

    do {
        printf("\n===== %s =====\n", NOME_BANCO);
        printf("Agencia: %s\n", AGENCIA_PADRAO);
        printf("1. Acessar modulo do Administrador\n");
        printf("2. Acessar modulo do Cliente\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        {
            char buf[64];
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                opcao = -1;
            } else {
                trimNewline(buf);
                char *endptr = NULL;
                long v = strtol(buf, &endptr, 10);
                if (endptr == buf || *endptr != '\0') opcao = -1;
                else opcao = (int)v;
            }
        }

        switch (opcao) {
            case 1:
                if (adminAutenticar()) adminMenu(&contas, &totalContas, &capacidadeContas);
                break;
            case 2: {
                // chama menu do cliente, passando o vetor de contas (sem variáveis globais)
                clienteMenu(&contas, &totalContas, &capacidadeContas);
                break;
            }
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    // salvar dados antes de encerrar
    if (salvarDados(contas, totalContas, "dados.bin")) {
        printf("Dados salvos em dados.bin (total=%d)\n", totalContas);
    } else {
        printf("Erro ao salvar dados em dados.bin\n");
    }

    free(contas);
    return 0;
}


// Implementação das funções de persistência
int salvarDados(const Conta *contas, int total, const char *arquivo) {
    if (!arquivo) return 0;
    FILE *f = fopen(arquivo, "wb");
    if (!f) return 0;
    if (fwrite(&total, sizeof(int), 1, f) != 1) { fclose(f); return 0; }
    if (total > 0) {
        if (fwrite(contas, sizeof(Conta), (size_t)total, f) != (size_t)total) { fclose(f); return 0; }
    }
    fclose(f);
    return 1;
}

int carregarDados(Conta **outContas, int *outTotal, const char *arquivo) {
    if (!outContas || !outTotal || !arquivo) return 0;
    FILE *f = fopen(arquivo, "rb");
    if (!f) return 0; // arquivo não existe
    int n = 0;
    if (fread(&n, sizeof(int), 1, f) != 1) { fclose(f); return 0; }
    if (n < 0) { fclose(f); return 0; }
    if (n == 0) {
        *outContas = NULL;
        *outTotal = 0;
        fclose(f);
        return 1;
    }
    Conta *tmp = (Conta *)malloc((size_t)n * sizeof(Conta));
    if (!tmp) { fclose(f); return 0; }
    if (fread(tmp, sizeof(Conta), (size_t)n, f) != (size_t)n) { free(tmp); fclose(f); return 0; }
    fclose(f);
    *outContas = tmp;
    *outTotal = n;
    return 1;
}

// Registrar log em arquivo texto (append)
void registrarLog(int idConta, const char *descricao) {
    if (!descricao) return;
    FILE *f = fopen("log.txt", "a");
    if (!f) return;
    time_t t = time(NULL);
    struct tm tmv;
#if defined(_MSC_VER)
    localtime_s(&tmv, &t);
#else
    struct tm *tmp = localtime(&t);
    if (tmp) tmv = *tmp;
#endif
    char buf[64];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmv) == 0) strcpy(buf, "0000-00-00 00:00:00");
    fprintf(f, "[%s] Conta %d: %s\n", buf, idConta, descricao);
    fclose(f);
}
