
#include "cliente.h"
#include "banco.h"
#include "unir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 100

int login(Conta *contas, int total, int *outIdx) {
    if (!contas || total <= 0) return 0;
    // Ler e validar CPF
    char cpfbuf[64];
    char senhabuf[32];
    while (1) {
        printf("Digite seu CPF: ");
        if (fgets(cpfbuf, sizeof(cpfbuf), stdin) == NULL) return 0;
        trimNewline(cpfbuf);
        if (validarCPF(cpfbuf)) break;
        printf("CPF invalido. Tente novamente.\n");
    }

    // Ler e validar senha (6 digitos numericos)
    while (1) {
        printf("Digite sua senha (6 digitos numericos): ");
        if (fgets(senhabuf, sizeof(senhabuf), stdin) == NULL) return 0;
        trimNewline(senhabuf);
        if (senhaValidaNDigitos(senhabuf, 6)) break;
        printf("Senha invalida. Deve conter exatamente 6 digitos numericos.\n");
    }

    // procurar conta correspondente
    if (!contas || total <= 0) return 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(contas[i].cpf, cpfbuf) == 0 && strcmp(contas[i].senha, senhabuf) == 0) {
            if (outIdx) *outIdx = i;
            return 1;
        }
    }
    return 0;
}

int tela_principal(Conta *contas, int total, int idx) {
    if (!contas || idx < 0 || idx >= total) return 0;
    Conta *c = &contas[idx];
    char agencia[128];
    snprintf(agencia, sizeof(agencia), "%s - %s", NOME_BANCO, AGENCIA_PADRAO);

    while (1) {
        printf("\nAGENCIA: %s\t  SALDO: %.2f\t  CPF: %s\n\n", agencia, c->saldo, c->cpf);
        printf("1. Consultar Saldo e Status\n");
        printf("2. Depositar\n");
        printf("3. Sacar (Com validacao de Saldo + Status Ativo)\n");
        printf("4. Transferencia entre Contas (Usa busca em vetor)\n");
        printf("5. Simulacao de Rendimento (Usa Recursividade)\n");
        printf("6. Bloquear/Desbloquear Cartao (Usa Bitwise)\n");
        printf("7. Alterar Senha (Usa manipulacao de strings do unir.h)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        char opcbuf[16];
        if (fgets(opcbuf, sizeof(opcbuf), stdin) == NULL) break;
        trimNewline(opcbuf);
        char *endptr_opc = NULL;
        long lopc = strtol(opcbuf, &endptr_opc, 10);
        int opc;
        if (endptr_opc == opcbuf || *endptr_opc != '\0') { printf("Opcao invalida.\n"); continue; }
        opc = (int)lopc;
        if (opc == 0) break;
        else if (opc == 1) {
            // Consultar saldo e status
            printf("Extrato - Conta %d | Nome: %s | CPF: %s | Saldo: R$ %.2f\n", c->id, c->nome, c->cpf, c->saldo);
            printf("Status:");
            if (c->flags & FLAG_BLOQUEADA) printf(" Bloqueado"); else printf(" Ativo");
            if (c->flags & FLAG_PREMIUM) printf(" | Premium");
            if (c->flags & FLAG_EMAIL_VERIFIED) printf(" | Email Verificado");
            printf("\n");
        } else if (opc == 2) {
            // Depositar
            printf("Valor a depositar: ");
            char vbuf[64]; if (fgets(vbuf, sizeof(vbuf), stdin) == NULL) continue;
            double v = atof(vbuf);
            if (v <= 0) { printf("Valor invalido.\n"); continue; }
            clienteDepositar(c, v);
            printf("Deposito efetuado. Saldo atual: R$ %.2f\n", c->saldo);
        } else if (opc == 3) {
            // Sacar (valida saldo e status)
            if (c->flags & FLAG_BLOQUEADA) { printf("Conta/cartao bloqueado. Operacao nao permitida.\n"); continue; }
            printf("Valor a sacar: ");
            char vbuf[64]; if (fgets(vbuf, sizeof(vbuf), stdin) == NULL) continue;
            double v = atof(vbuf);
            if (v <= 0) { printf("Valor invalido.\n"); continue; }
            if (c->saldo < (float)v) { printf("Saldo insuficiente.\n"); continue; }
            if (clienteSacar(c, v)) {
                printf("Saque realizado. Saldo atual: R$ %.2f\n", c->saldo);
            } else {
                printf("Saque falhou.\n");
            }
        } else if (opc == 4) {
            // Transferência entre contas
            if (c->flags & FLAG_BLOQUEADA) { printf("Conta/cartao bloqueado. Operacao nao permitida.\n"); continue; }
            printf("Numero da conta destino: ");
            char idbuf[32]; if (fgets(idbuf, sizeof(idbuf), stdin) == NULL) continue;
            trimNewline(idbuf);
            char *endptr_id = NULL; long ldest = strtol(idbuf, &endptr_id, 10);
            if (endptr_id == idbuf || *endptr_id != '\0') { printf("Conta destino invalida.\n"); continue; }
            int destId = (int)ldest;
            int destIdx = -1;
            for (int i = 0; i < total; i++) if (contas[i].id == destId) { destIdx = i; break; }
            if (destIdx < 0) { printf("Conta destino nao encontrada.\n"); continue; }
            printf("Valor a transferir: "); char vbuf2[64]; if (fgets(vbuf2, sizeof(vbuf2), stdin) == NULL) continue;
            trimNewline(vbuf2);
            char *endptr_v2 = NULL; double v = strtod(vbuf2, &endptr_v2);
            if (endptr_v2 == vbuf2 || *endptr_v2 != '\0') { printf("Valor invalido.\n"); continue; }
            if (v <= 0) { printf("Valor invalido.\n"); continue; }
            if (clienteTransferir(contas, total, idx, destIdx, v)) {
                printf("Transferencia realizada. Saldo atual: R$ %.2f\n", c->saldo);
            } else {
                printf("Falha na transferencia.\n");
            }
        } else if (opc == 5) {
            // Simulação de rendimento
            printf("Taxa mensal (ex: 0.01 = 1%%): "); char tbuf[32]; if (fgets(tbuf, sizeof(tbuf), stdin) == NULL) continue;
            trimNewline(tbuf);
            char *endptr_taxa = NULL; double taxa = strtod(tbuf, &endptr_taxa);
            if (endptr_taxa == tbuf || *endptr_taxa != '\0') { printf("Taxa invalida.\n"); continue; }
            printf("Meses: "); char mbuf[16]; if (fgets(mbuf, sizeof(mbuf), stdin) == NULL) continue;
            trimNewline(mbuf);
            char *endptr_m = NULL; long lmeses = strtol(mbuf, &endptr_m, 10);
            if (endptr_m == mbuf || *endptr_m != '\0') { printf("Meses invalidos.\n"); continue; }
            int meses = (int)lmeses;
            double futuro = simularRendimentoRecursivo(c->saldo, taxa, meses);
            printf("Saldo atual: R$ %.2f -> apos %d meses (taxa %.4f): R$ %.2f\n", c->saldo, meses, taxa, futuro);
        } else if (opc == 6) {
            // Bloquear/Desbloquear cartão (bitwise)
            if (clienteToggleBloqueio(c)) {
                if (c->flags & FLAG_BLOQUEADA) printf("Conta/cartao agora BLOQUEADO.\n"); else printf("Conta/cartao agora DESBLOQUEADO.\n");
            } else {
                printf("Operacao de bloqueio falhou.\n");
            }
        } else if (opc == 7) {
            // Alterar senha
            printf("Digite sua senha atual: ");
            char cur[32]; if (fgets(cur, sizeof(cur), stdin) == NULL) continue; trimNewline(cur);
            if (strcmp(cur, c->senha) != 0) { printf("Senha atual incorreta.\n"); continue; }
            if (clienteAlterarSenha(c)) {
                printf("Senha alterada com sucesso.\n");
            } else {
                printf("Falha ao alterar senha.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }

    return 0;
}

// Legacy PIX / boleto / extrato / opcoes removed — client now uses Conta-based APIs

// Operacoes sobre Conta
int clienteDepositar(Conta *c, double valor) {
    if (!c || valor <= 0.0) return 0;
    c->saldo += (float)valor;
    return 1;
}

int clienteSacar(Conta *c, double valor) {
    if (!c || valor <= 0.0) return 0;
    if (c->saldo < (float)valor) return 0;
    c->saldo -= (float)valor;
    return 1;
}

int clienteTransferir(Conta *contas, int total, int fromIdx, int toIdx, double valor) {
    if (!contas || fromIdx < 0 || toIdx < 0 || fromIdx >= total || toIdx >= total) return 0;
    Conta *from = &contas[fromIdx];
    Conta *to = &contas[toIdx];
    if (from->flags & FLAG_BLOQUEADA) return 0;
    if (to->flags & FLAG_BLOQUEADA) return 0;
    if (valor <= 0.0 || from->saldo < (float)valor) return 0;
    from->saldo -= (float)valor;
    to->saldo += (float)valor;
    return 1;
}

double simularRendimentoRecursivo(double saldo, double taxa, int meses) {
    if (meses <= 0) return saldo;
    double novo = saldo + saldo * taxa;
    return simularRendimentoRecursivo(novo, taxa, meses - 1);
}

int clienteBloquearConta(Conta *c) {
    if (!c) return 0;
    c->flags |= FLAG_BLOQUEADA;
    return 1;
}

int clienteToggleBloqueio(Conta *c) {
    if (!c) return 0;
    if (c->flags & FLAG_BLOQUEADA) {
        c->flags &= (unsigned char)(~FLAG_BLOQUEADA);
    } else {
        c->flags |= FLAG_BLOQUEADA;
    }
    return 1;
}

int clienteAlterarSenha(Conta *c) {
    if (!c) return 0;
    // senha atual validated by caller (menu) — prompt for new senha
    char nova[32];
    printf("Digite a nova senha (6 digitos numericos): ");
    if (fgets(nova, sizeof(nova), stdin) == NULL) return 0;
    trimNewline(nova);
    if (!senhaValidaNDigitos(nova, 6)) {
        printf("Senha invalida. Deve conter exatamente 6 digitos numericos.\n");
        return 0;
    }
    // copy to account
    strncpy(c->senha, nova, sizeof(c->senha));
    c->senha[sizeof(c->senha)-1] = '\0';
    return 1;
}

// Menu do cliente: login / criar conta (sem variáveis globais)
void clienteMenu(Conta **contas, int *total, int *capacidade) {
    if (!contas || !total || !capacidade) return;
    while (1) {
        printf("\n--- Menu Cliente - %s (%s) ---\n", NOME_BANCO, AGENCIA_PADRAO);
        printf("1. Login\n2. Criar conta\n0. Voltar\n");
        printf("Escolha: ");
        char buf[16]; if (fgets(buf, sizeof(buf), stdin) == NULL) break;
        int opc = atoi(buf);
        if (opc == 0) break;
        else if (opc == 1) {
            int idx = -1;
            if (*contas == NULL || *total <= 0) {
                printf("Nenhuma conta cadastrada. Crie uma conta primeiro.\n");
            } else if (login(*contas, *total, &idx)) {
                tela_principal(*contas, *total, idx);
            } else {
                printf("Login falhou. CPF ou senha invalidos.\n");
            }
        } else if (opc == 2) {
            clienteCriarConta(contas, total, capacidade);
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

void clienteCriarConta(Conta **contas, int *total, int *capacidade) {
    if (!contas || !total || !capacidade) return;
    if (*capacidade < 1) *capacidade = 2;
    if (*contas == NULL) {
        *contas = (Conta *)malloc((size_t)(*capacidade) * sizeof(Conta));
        if (!*contas) { *capacidade = 0; return; }
    }
    if (*total + 1 > *capacidade) {
        int nova = (*capacidade) * 2;
        Conta *tmp = (Conta *)realloc(*contas, (size_t)nova * sizeof(Conta));
        if (!tmp) return;
        *contas = tmp;
        *capacidade = nova;
    }

    Conta *c = &(*contas)[*total];
    // Ler nome
    printf("Nome: ");
    if (fgets(c->nome, sizeof(c->nome), stdin) == NULL) return;
    trimNewline(c->nome);
    // Ler CPF
    while (1) {
        printf("CPF: ");
        if (fgets(c->cpf, sizeof(c->cpf), stdin) == NULL) return;
        trimNewline(c->cpf);
        if (validarCPF(c->cpf)) break;
        printf("CPF invalido. Tente novamente.\n");
    }
    // Ler senha
    while (1) {
        printf("Senha (6 digitos numericos): ");
        char senhabuf[32]; if (fgets(senhabuf, sizeof(senhabuf), stdin) == NULL) return;
        trimNewline(senhabuf);
        if (senhaValidaNDigitos(senhabuf, 6)) { strncpy(c->senha, senhabuf, sizeof(c->senha)); c->senha[sizeof(c->senha)-1]='\0'; break; }
        printf("Senha invalida. Deve conter exatamente 6 digitos numericos.\n");
    }
    c->saldo = 0.0f;
    c->flags = 0;
    // Id gerado centralmente para manter consistência
    c->id = gerarProximoId(*contas, *total);
    (*total)++;
    printf("Conta criada com id %d\n", c->id);
}