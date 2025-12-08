#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banco.h"

// Rotina de gestão da capacidade do vetor de contas (utilizada por cliente/admin)
int garantirCapacidade(Conta **refContas, int *refCapacidade, int necessario) {
    if (necessario <= *refCapacidade) return 1;
    int nova = *refCapacidade;
    while (nova < necessario) {
        if (nova < 1) nova = 1;
        else nova *= 2;
    }
    void *tmp = realloc(*refContas, (size_t)nova * sizeof(Conta));
    if (!tmp) return 0;
    *refContas = (Conta *)tmp;
    *refCapacidade = nova;
    return 1;
}


// Persistência: salvamento dos dados em arquivo binário
int salvarDados(const Conta *contas, int total, const char *arquivo) {
    if (!arquivo) return 0;
    FILE *f = fopen(arquivo, "wb");
    if (!f) return 0;
    
    // Grava o total primeiro
    if (fwrite(&total, sizeof(int), 1, f) != 1) { fclose(f); return 0; }
    
    // Grava o vetor se houver contas
    if (total > 0) {
        if (fwrite(contas, sizeof(Conta), (size_t)total, f) != (size_t)total) { 
            fclose(f); 
            return 0; 
        }
    }
    fclose(f);
    return 1;
}

// Carregamento: leitura do binário e reconstrução do vetor em memória
int carregarDados(Conta **outContas, int *outTotal, int *outCapacidade, const char *arquivo) {
    if (!outContas || !outTotal || !arquivo) return 0;
    
    FILE *f = fopen(arquivo, "rb");
    if (!f) return 0; // arquivo não existe (primeira execução)

    int n = 0;
    // Lê o total de contas
    if (fread(&n, sizeof(int), 1, f) != 1) { fclose(f); return 0; }
    
    if (n < 0) { fclose(f); return 0; } // Proteção contra arquivo corrompido

    if (n == 0) {
        *outContas = NULL;
        *outTotal = 0;
        if(outCapacidade) *outCapacidade = 0;
        fclose(f);
        return 1;
    }

    // Aloca memória exata para o que estava no disco
    Conta *tmp = (Conta *)malloc((size_t)n * sizeof(Conta));
    if (!tmp) { fclose(f); return 0; }

    // Lê o vetor
    if (fread(tmp, sizeof(Conta), (size_t)n, f) != (size_t)n) { 
        free(tmp); 
        fclose(f); 
        return 0; 
    }

    fclose(f);
    
    // Atualiza os ponteiros da main
    *outContas = tmp;
    *outTotal = n;
    
    // Atualiza a capacidade para ser igual ao total carregado,
    // permitindo que o próximo realloc tenha referência correta
    if(outCapacidade) *outCapacidade = n; 

    return 1;
}

// Registro de log: anotação de operações com data/hora em arquivo texto (append)
void registrarLog(int idConta, const char *descricao) {
    if (!descricao) return;
    FILE *f = fopen("log.txt", "a");
    if (!f) return;
    
    time_t t = time(NULL);
    struct tm tmv;
    
    // Tratamento de hora cross-platform (Windows/Linux)
    #if defined(_MSC_VER)
        localtime_s(&tmv, &t);
    #else
        struct tm *tmp = localtime(&t);
        if (tmp) tmv = *tmp;
    #endif

    char buf[64];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmv) == 0) 
        strcpy(buf, "0000-00-00 00:00:00");
        
    fprintf(f, "[%s] Conta %d: %s\n", buf, idConta, descricao);
    fclose(f);
}