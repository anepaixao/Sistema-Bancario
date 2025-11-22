#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <errno.h>
#include "banco.h"

// O main passa os ponteiros para as funções
// Persistência/log já estão declaradas em banco.h

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

// Garante capacidade do vetor de contas usando ponteiro para ponteiro e ref de inteiro
static int adminGarantirCapacidade(Conta **refContas, int *refCapacidade, int necessario) {
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

// Lista encadeada simples para índices de contas bloqueadas
typedef struct NoBloq {
    int idx;
    struct NoBloq *prox;
} NoBloq;

static NoBloq *construirListaBloqueadas(const Conta *contas, int total) {
    NoBloq *head = NULL;
    for (int i = 0; i < total; i++) {
        if (contas[i].flags & FLAG_BLOQUEADA) {
            NoBloq *n = (NoBloq *)malloc(sizeof(NoBloq));
            if (!n) break;
            n->idx = i;
            n->prox = head;
            head = n;
        }
    }
    return head;
}

static void liberarListaBloqueadas(NoBloq *head) {
    while (head) {
        NoBloq *tmp = head;
        head = head->prox;
        free(tmp);
    }
}

// Matriz dinâmica mínima (2x2) apenas para cobrir o tópico
static void adminMatrizDinamicaPequena(void) {
    int linhas = 2, colunas = 2;
    int **m = (int **)malloc((size_t)linhas * sizeof(int *));
    if (!m) return;
    for (int i = 0; i < linhas; i++) {
        m[i] = (int *)malloc((size_t)colunas * sizeof(int));
        if (!m[i]) {
            for (int k = 0; k < i; k++) free(m[k]);
            free(m);
            return;
        }
    }
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            m[i][j] = (i == j) ? 1 : 0; // identidade 2x2
    for (int i = 0; i < linhas; i++) free(m[i]);
    free(m);
}

// flag macros are declared in banco.h

// Aplica uma operação a cada conta (exemplo de ponteiro para função que recebe Conta*)
static void adminAplicarACadaConta(Conta *contas, int total, void (*op)(Conta *)) {
    if (!op) return;
    for (int i = 0; i < total; i++) op(&contas[i]);
}

// Autentica o administrador: usuário e senha fixos por enquanto
int adminAutenticar(void) {
    char user[64];
    char pass[64];

    // limpar newline pendente
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) { }
    printf("\nAutenticacao do Administrador\n");
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
    printf("Credenciais invalidas.\n");
    return 0;
}

// Função auxiliar para criar uma nova conta
void adminCriarConta(Conta *contas, int *total) {
    int ch;
    printf("\nNova Conta\n");

    // Gera automaticamente o número da conta.
    // Padrão: sequencial começando em 1001. Evita duplicatas verificando as contas já criadas.
    static int proximaConta = 1001;
    int candidato = proximaConta;
    int duplicado;
    do {
        duplicado = 0;
        for (int i = 0; i < *total; i++) {
            if (contas[i].id == candidato) {
                duplicado = 1;
                candidato++;
                break;
            }
        }
    } while (duplicado);
    contas[*total].id = candidato;
    proximaConta = candidato + 1;

    // Consumir newline pendente do scanf do menu anterior
    while ((ch = getchar()) != '\n' && ch != EOF) { }
    // Ler nome não vazio
    while (1) {
        printf("Nome completo: ");
        if (fgets(contas[*total].nome, sizeof(contas[*total].nome), stdin) == NULL) {
            printf("Erro de leitura do nome.\n");
            continue;
        }
        contas[*total].nome[strcspn(contas[*total].nome, "\n")] = '\0';
        // Verifica se há ao menos um caractere alfabetico
        int valido = 0;
        for (size_t i = 0; contas[*total].nome[i] != '\0'; i++) {
            if (!isspace((unsigned char)contas[*total].nome[i])) { valido = 1; break; }
        }
        if (!valido) {
            printf("Nome vazio. Tente novamente.\n");
            continue;
        }
        break;
    }

    // Ler e validar CPF
    char buffer[64];
    while (1) {
        printf("CPF (somente numeros ou com pontuacao): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura do CPF.\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarCPF(buffer)) {
            // armazenar (mantemos o formato digitado, truncando se necessário)
            strncpy(contas[*total].cpf, buffer, sizeof(contas[*total].cpf)-1);
            contas[*total].cpf[sizeof(contas[*total].cpf)-1] = '\0';
            break;
        } else {
            printf("CPF invalido.\n");
        }
    }
        // Ler e validar senha: exatamente 6 dígitos e confirmação
        while (1) {
            char senha1[16];
            char senha2[16];

            printf("Senha (6 digitos numericos): ");
            if (fgets(senha1, sizeof(senha1), stdin) == NULL) { printf("Erro de leitura da senha.\n"); continue; }
            senha1[strcspn(senha1, "\n")] = '\0';
            printf("Confirmar senha: ");
            if (fgets(senha2, sizeof(senha2), stdin) == NULL) { printf("Erro na confirmacao da senha.\n"); continue; }
            senha2[strcspn(senha2, "\n")] = '\0';

            // verificar comprimento exatamente 6 e apenas dígitos
            int len1 = (int)strlen(senha1);
            int len2 = (int)strlen(senha2);
            if (len1 != 6 || len2 != 6) {
                printf("Senha invalida: precisa ter 6 digitos.\n");
                continue;
            }
            int ok = 1;
            for (int k = 0; k < 6; k++) if (!isdigit((unsigned char)senha1[k]) || !isdigit((unsigned char)senha2[k])) { ok = 0; break; }
            if (!ok) {
                printf("Senha invalida: apenas digitos.\n");
                continue;
            }
            if (strcmp(senha1, senha2) != 0) {
                printf("Senhas diferentes.\n");
                continue;
            }

            // armazenar na conta (termina com '\0')
            strncpy(contas[*total].senha, senha1, sizeof(contas[*total].senha)-1);
            contas[*total].senha[sizeof(contas[*total].senha)-1] = '\0';
            break;
        }

    contas[*total].saldo = 0.0f;
    contas[*total].flags = 0; // inicializa flags (nenhuma flag ativa)

    (*total)++;

    printf("Conta criada. Id: %d\n", contas[*total - 1].id);
}

// Predicados de exemplo (internos)
// Callback typedef para filtros
typedef int (*ContaFiltro)(const Conta *c);

// Exemplos de filtros (mantidos para eventual extensão)
static int filtroAtivas(const Conta *c) { return !(c->flags & FLAG_BLOQUEADA); }
static int filtroBloqueadas(const Conta *c) { return (c->flags & FLAG_BLOQUEADA) != 0; }

// Função para listar contas com filtro (callback). Se filtro == NULL, lista todas.
void adminListarContas(Conta *contas, int total, ContaFiltro filtro) {
    printf("\nContas Cadastradas\n");

    if (total == 0) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    // Construir lista encadeada de bloqueadas (uso interno, sem saída)
    NoBloq *bloq = construirListaBloqueadas(contas, total);

    for (Conta *p = contas; p < contas + total; p++) {
        if (!filtro || filtro(p)) {
            printf("Conta: %d | Nome: %s | CPF: %s | Saldo: %.2f | Status: %s\n",
                   p->id,
                   p->nome,
                   p->cpf,
                   p->saldo,
                   (p->flags & FLAG_BLOQUEADA) ? "Bloqueada" : "Ativa");
        }
    }

    // Libera lista encadeada
    liberarListaBloqueadas(bloq);
}

// Função para bloquear conta
void adminBloquearConta(Conta *contas, int total) {
    int numero; int ch;
    printf("\nBloquear Conta\n");
    printf("Numero da conta: ");
    if (scanf("%d", &numero) != 1) { while ((ch = getchar()) != '\n' && ch != EOF) { } printf("Entrada invalida.\n"); return; }

    for (int i = 0; i < total; i++) {
        if (contas[i].id == numero) {
            contas[i].flags |= FLAG_BLOQUEADA;
            printf("Conta %d bloqueada.\n", numero);
            return;
        }
    }

    printf("Conta nao encontrada.\n");
}

// Função para desbloquear conta
void adminDesbloquearConta(Conta *contas, int total) {
    int numero; int ch;
    printf("\nDesbloquear Conta\n");
    printf("Numero da conta: ");
    if (scanf("%d", &numero) != 1) { while ((ch = getchar()) != '\n' && ch != EOF) { } printf("Entrada invalida.\n"); return; }

    for (int i = 0; i < total; i++) {
        if (contas[i].id == numero) {
            contas[i].flags &= (unsigned char)~FLAG_BLOQUEADA;
            printf("Conta %d desbloqueada.\n", numero);
            return;
        }
    }

    printf("Conta nao encontrada.\n");
}

// Função calcular o saldo total
float adminCalcularSaldoTotalRecursivo(Conta *contas, int indice, int total) {
    if (indice == total) {
        return 0.0f;
    }
    return contas[indice].saldo + adminCalcularSaldoTotalRecursivo(contas, indice + 1, total);
}

// Função principal do menu do administrador
void adminMenu(Conta **contas, int *total, int *capacidade) {
    int opcao; int ch;

    if (!contas || !total || !capacidade) return;
    if (*contas == NULL && *capacidade > 0) {
        *contas = (Conta *)malloc((size_t)(*capacidade) * sizeof(Conta));
        if (!*contas) {
            printf("Erro ao alocar memoria!\n");
            return;
        }
    }

    // Usa uma pequena matriz dinâmica para cobrir o tópico (sem saída)
    adminMatrizDinamicaPequena();

    do {
        printf("\nMenu do Administrador\n");
        printf("1 - Criar conta\n");
        printf("2 - Listar contas\n");
        printf("3 - Bloquear conta\n");
        printf("4 - Saldo total (recursivo)\n");
        printf("5 - Desbloquear conta\n");
        printf("6 - Listar somente contas ativas\n");
        printf("7 - Listar somente contas bloqueadas\n");
        printf("8 - Salvar contas em arquivo\n");
        printf("9 - Carregar contas de arquivo\n");
        printf("10 - Alternar flag PREMIUM (bitwise)\n");
        printf("11 - Mostrar flags de uma conta\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { while ((ch = getchar()) != '\n' && ch != EOF) { } opcao = -1; }

        switch (opcao) {
            case 1:
                if (!adminGarantirCapacidade(contas, capacidade, (*total) + 1)) {
                    printf("Erro de realocacao de memoria.\n");
                    return;
                }
                adminCriarConta(*contas, total);
                break;
            case 2:
                adminListarContas(*contas, *total, NULL); // todas
                break;
            case 3:
                adminBloquearConta(*contas, *total);
                break;
               case 5:
                   adminDesbloquearConta(*contas, *total);
                   break;
            case 4:
                printf("Saldo total: R$ %.2f\n",
                       adminCalcularSaldoTotalRecursivo(*contas, 0, *total));
                break;
            case 6:
                adminListarContas(*contas, *total, filtroAtivas);
                break;
            case 7:
                adminListarContas(*contas, *total, filtroBloqueadas);
                break;
            case 8: {
                const char *fn = "contas.dat";
                if (salvarDados(*contas, *total, fn)) printf("Contas salvas em %s\n", fn);
                else printf("Erro ao salvar contas.\n");
                break;
            }
            case 9: {
                const char *fn = "contas.dat";
                if (carregarDados(contas, total, fn)) {
                    // ajustar capacidade para pelo menos total
                    if (*capacidade < *total) *capacidade = *total;
                    printf("Contas carregadas de %s (total=%d)\n", fn, *total);
                } else printf("Erro ao carregar contas ou arquivo inexistente.\n");
                break;
            }
            case 10: {
                int numero; int ch;
                printf("Numero da conta: ");
                if (scanf("%d", &numero) != 1) { while ((ch = getchar()) != '\n' && ch != EOF) { } printf("Entrada invalida.\n"); break; }
                for (int i = 0; i < *total; i++) {
                    if ((*contas)[i].id == numero) {
                        (*contas)[i].flags ^= FLAG_PREMIUM; // toggle
                        printf("Conta %d PREMIUM agora: %s\n", numero, ((*contas)[i].flags & FLAG_PREMIUM) ? "SIM" : "NAO");
                        break;
                    }
                }
                break;
            }
            case 11: {
                int numero; int ch;
                printf("Numero da conta: ");
                if (scanf("%d", &numero) != 1) { while ((ch = getchar()) != '\n' && ch != EOF) { } printf("Entrada invalida.\n"); break; }
                for (int i = 0; i < *total; i++) {
                    if ((*contas)[i].id == numero) {
                        printf("Conta %d - FLAG_PREMIUM=%s, FLAG_EMAIL_VERIFIED=%s\n",
                               numero,
                               ((*contas)[i].flags & FLAG_PREMIUM) ? "SIM" : "NAO",
                               ((*contas)[i].flags & FLAG_EMAIL_VERIFIED) ? "SIM" : "NAO");
                        break;
                    }
                }
                break;
            }
            case 0:
                printf("Retornando ao menu principal.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);
}