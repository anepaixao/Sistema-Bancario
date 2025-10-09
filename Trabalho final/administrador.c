#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "banco.h"

// Remove caracteres não numéricos (mantém apenas os dígitos) — saída em dst (terminada em '\0')
static void apenasDigitos(const char *src, char *dst) {
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
static int validarCPF(const char *cpf) {
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

// Autentica o administrador: usuário e senha fixos por enquanto
int autenticarAdministrador(void) {
    char user[64];
    char pass[64];

    // Consumir newline pendente (caso venha de scanf)
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }

    printf("\n=== Autenticacao Administrador ===\n");
    printf("Usuario: ");
    if (fgets(user, sizeof(user), stdin) == NULL) return 0;
    user[strcspn(user, "\n")] = '\0';

    printf("Senha: ");
    if (fgets(pass, sizeof(pass), stdin) == NULL) return 0;
    pass[strcspn(pass, "\n")] = '\0';

    if (strcmp(user, "ane") == 0 && strcmp(pass, "admin") == 0) {
        printf("Autenticado com sucesso!\n");
        return 1;
    }
    printf("Usuario ou senha incorretos.\n");
    return 0;
}

// Função auxiliar para criar uma nova conta
void criarConta(Conta *contas, int *total) {
    printf("\n=== Criar Nova Conta ===\n");

    // Gera automaticamente o número da conta.
    // Padrão: sequencial começando em 1001. Evita duplicatas verificando as contas já criadas.
    static int proximaConta = 1001;
    int candidato = proximaConta;
    int duplicado;
    do {
        duplicado = 0;
        for (int i = 0; i < *total; i++) {
            if (contas[i].numeroConta == candidato) {
                duplicado = 1;
                candidato++;
                break;
            }
        }
    } while (duplicado);
    contas[*total].numeroConta = candidato;
    proximaConta = candidato + 1;

    // Consumir newline pendente do scanf do menu anterior
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }

    printf("Nome completo: ");
    fgets(contas[*total].nome, 50, stdin);
    contas[*total].nome[strcspn(contas[*total].nome, "\n")] = '\0';

    // Ler e validar CPF
    char buffer[64];
    while (1) {
        printf("CPF (somente numeros ou com pontuacao): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro na leitura do CPF. Tente novamente.\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarCPF(buffer)) {
            // armazenar (mantemos o formato digitado, truncando se necessário)
            strncpy(contas[*total].cpf, buffer, sizeof(contas[*total].cpf)-1);
            contas[*total].cpf[sizeof(contas[*total].cpf)-1] = '\0';
            break;
        } else {
            printf("CPF invalido. Por favor, verifique e tente novamente.\n");
        }
    }

    // Ler e validar senha: somente números,com 6 dígitos
    while (1) {
        printf("Senha (somente números, com 6 digitos): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro na leitura da senha. Tente novamente.\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        int len = (int)strlen(buffer);
        int ok = 1;
        if (len == 0 || len > 6) ok = 0;
        for (int k = 0; k < len && ok; k++) if (!isdigit((unsigned char)buffer[k])) ok = 0;
        if (!ok) {
            printf("Senha invalida. Deve conter apenas numeros e 6 caracteres.\n");
            continue;
        }
        // armazenar na conta (termina com '\0')
        strncpy(contas[*total].senha, buffer, sizeof(contas[*total].senha)-1);
        contas[*total].senha[sizeof(contas[*total].senha)-1] = '\0';
        break;
    }
        // Ler e validar senha: exatamente 6 dígitos e confirmação
        while (1) {
            char senha1[16];
            char senha2[16];

            printf("Senha (exatamente 6 digitos numericos): ");
            if (fgets(senha1, sizeof(senha1), stdin) == NULL) {
                printf("Erro na leitura da senha. Tente novamente.\n");
                continue;
            }
            senha1[strcspn(senha1, "\n")] = '\0';

            printf("Confirme a senha: ");
            if (fgets(senha2, sizeof(senha2), stdin) == NULL) {
                printf("Senha nao confere. Tente novamente.\n");
                continue;
            }
            senha2[strcspn(senha2, "\n")] = '\0';

            // verificar comprimento exatamente 6 e apenas dígitos
            int len1 = (int)strlen(senha1);
            int len2 = (int)strlen(senha2);
            if (len1 != 6 || len2 != 6) {
                printf("Senha invalida. A senha e a confirmacao devem ter exatamente 6 dígitos.\n");
                continue;
            }
            int ok = 1;
            for (int k = 0; k < 6; k++) if (!isdigit((unsigned char)senha1[k]) || !isdigit((unsigned char)senha2[k])) { ok = 0; break; }
            if (!ok) {
                printf("Senha invalida. Deve conter apenas digitos.\n");
                continue;
            }
            if (strcmp(senha1, senha2) != 0) {
                printf("As senhas nao coincidem. Tente novamente.\n");
                continue;
            }

            // armazenar na conta (termina com '\0')
            strncpy(contas[*total].senha, senha1, sizeof(contas[*total].senha)-1);
            contas[*total].senha[sizeof(contas[*total].senha)-1] = '\0';
            break;
        }

    contas[*total].saldo = 0.0;
    // Definir agência padrão como código de 4 dígitos
    strncpy(contas[*total].agencia, "0001", sizeof(contas[*total].agencia)-1);
    contas[*total].agencia[sizeof(contas[*total].agencia)-1] = '\0';
    contas[*total].status = 1;

    (*total)++;

    printf("\nConta criada com sucesso! Numero da conta: %d\n", contas[*total - 1].numeroConta);
}

// Função para listar todas as contas
void listarContas(Conta *contas, int total) {
    printf("\n=== Lista de Contas ===\n");

    if (total == 0) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
     printf("Conta: %d | Agência: %s | Nome: %s | CPF: %s | Saldo: %.2f | Status: %s\n",
         contas[i].numeroConta,
         contas[i].agencia,
         contas[i].nome,
         contas[i].cpf,
         contas[i].saldo,
         contas[i].status ? "Ativa" : "Bloqueada");
    }
}

// Função para bloquear conta
void bloquearConta(Conta *contas, int total) {
    int numero;
    printf("\n=== Bloquear Conta ===\n");
    printf("Digite o numero da conta: ");
    scanf("%d", &numero);

    for (int i = 0; i < total; i++) {
        if (contas[i].numeroConta == numero) {
            contas[i].status = 0;
            printf("Conta %d bloqueada!\n", numero);
            return;
        }
    }

    printf("Conta nao encontrada!\n");
}

// Função calcular o saldo total
float calcularSaldoRecursivo(Conta *contas, int indice, int total) {
    if (indice == total)
        return 0;
    return contas[indice].saldo + calcularSaldoRecursivo(contas, indice + 1, total);
}

// Função principal do menu do administrador
void menuAdministrador() {
    Conta *contas = NULL;
    int total = 0;
    int capacidade = 2; // capacidade inicial
    int opcao;

    contas = (Conta *)malloc(capacidade * sizeof(Conta));

    if (contas == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    do {
        printf("\n==== MENU ADMINISTRADOR ====\n");
        printf("1 - Criar Conta\n");
        printf("2 - Listar Contas\n");
        printf("3 - Bloquear Conta\n");
        printf("4 - Mostrar Saldo Total\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (total == capacidade) {
                    capacidade *= 2;
                    contas = (Conta *)realloc(contas, capacidade * sizeof(Conta));
                    if (contas == NULL) {
                        printf("Erro ao realocar memoria!\n");
                        return;
                    }
                }
                criarConta(contas, &total);
                break;
            case 2:
                listarContas(contas, total);
                break;
            case 3:
                bloquearConta(contas, total);
                break;
            case 4:
                printf("Saldo total no banco: R$ %.2f\n",
                       calcularSaldoRecursivo(contas, 0, total));
                break;
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            default:
                printf("Opção invalida!\n");
        }

    } while (opcao != 0);

    free(contas);
}

