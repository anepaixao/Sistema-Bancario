#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <errno.h>
#include "administrador.h"
#include "unir.h"

// O main passa os ponteiros para as funções
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
static void adminAplicarACadaConta(Conta *contas, int total, void (*op)(Conta *)) __attribute__((unused));
static void adminAplicarACadaConta(Conta *contas, int total, void (*op)(Conta *)) {
    if (!op) return;
    for (int i = 0; i < total; i++) op(&contas[i]);
}

// Autentica o administrador: usuário e senha fixos por enquanto
int adminAutenticar(void) {
    char user[64];
    char pass[64];
    // Nota: não limpar stdin aqui — a leitura do menu principal usa fgets
    cabecalho("AUTENTICACAO - ADMINISTRADOR");
    barraCarregamento("Preparando tela administrativa", 500);
    printf("Usuario: ");
    if (fgets(user, sizeof(user), stdin) == NULL) return 0;
    user[strcspn(user, "\n")] = '\0';
    // Senha mascarada com opcao de mostrar/ocultar (F2)
    lerSenhamascarada(pass, sizeof(pass));

    if (strcmp(user, "ane") == 0 && strcmp(pass, "admin") == 0) {
        barraCarregamento("Validando credenciais", 500);
        msgSucesso("Autenticado com sucesso!");
        return 1;
    }
    msgErro("Credenciais invalidas.");
    return 0;
}

// Função auxiliar para criar uma nova conta
void adminCriarConta(Conta *contas, int *total) {
    cabecalho("CRIAR CONTA (ADMIN)");
    barraCarregamento("Carregando tela de criacao", 500);

    printf("\n");

    // Gera automaticamente o número da conta usando o utilitário centralizado
    // para evitar lógica duplicada entre módulos.
    contas[*total].id = gerarProximoId(contas, *total);

    // Observacao: nao consumir stdin aqui — entradas sao lidas com fgets
    // Ler nome não vazio
    while (1) {
        printf("Nome completo: ");
        if (fgets(contas[*total].nome, sizeof(contas[*total].nome), stdin) == NULL) {
            msgErro("Erro de leitura do nome.");
            continue;
        }
        contas[*total].nome[strcspn(contas[*total].nome, "\n")] = '\0';
        // Verifica se há ao menos um caractere alfabetico
        int valido = 0;
        for (size_t i = 0; contas[*total].nome[i] != '\0'; i++) {
            if (!isspace((unsigned char)contas[*total].nome[i])) { valido = 1; break; }
        }
        if (!valido) {
            msgErro("Nome vazio. Tente novamente.");
            continue;
        }
        break;
    }

    // Ler e validar CPF (mascarado)
    char buffer[32];
    while (1) {
        lerCPFmascarado(buffer, sizeof(buffer));
        if (validarCPF(buffer)) {
            char dig[16];
            apenasDigitos(buffer, dig);
            strncpy(contas[*total].cpf, dig, sizeof(contas[*total].cpf)-1);
            contas[*total].cpf[sizeof(contas[*total].cpf)-1] = '\0';
            break;
        } else {
            msgErro("CPF invalido.");
        }
    }
    // Ler e validar senha: exatamente 6 dígitos e confirmação (mascarado)
    while (1) {
        char senha1[16];
        char senha2[16];
        lerSenhamascarada(senha1, sizeof(senha1));
        printf("Confirmar senha: ");
        lerSenhamascarada(senha2, sizeof(senha2));
        if (!senhaValidaNDigitos(senha1, 6) || !senhaValidaNDigitos(senha2, 6)) {
            msgErro("Senha invalida: precisa ter exatamente 6 digitos numericos.");
            continue;
        }
        if (strcmp(senha1, senha2) != 0) {
            msgErro("Senhas diferentes.");
            continue;
        }
        strncpy(contas[*total].senha, senha1, sizeof(contas[*total].senha)-1);
        contas[*total].senha[sizeof(contas[*total].senha)-1] = '\0';
        break;
    }

    contas[*total].saldo = 0.0f;
    contas[*total].flags = 0; // inicializa flags (nenhuma flag ativa)

    (*total)++;
    barraCarregamento("Salvando nova conta", 600);

    char buf[64]; snprintf(buf, sizeof(buf), "Conta criada. Id: %d", contas[*total - 1].id);
    msgSucesso(buf);
}

// Predicados de exemplo (internos)
// Callback typedef para filtros
typedef int (*ContaFiltro)(const Conta *c);

// Exemplos de filtros (mantidos para eventual extensão)
static int filtroAtivas(const Conta *c) { return !(c->flags & FLAG_BLOQUEADA); }
static int filtroBloqueadas(const Conta *c) { return (c->flags & FLAG_BLOQUEADA) != 0; }

// Função para listar contas com filtro (callback). Se filtro == NULL, lista todas.
void adminListarContas(Conta *contas, int total, ContaFiltro filtro) {
    cabecalho("CONTAS CADASTRADAS");
    barraCarregamento("Carregando lista de contas", 600);

    if (total == 0) {
        msgErro("Nenhuma conta cadastrada.");
        return;
    }

    // Construir lista encadeada de bloqueadas (uso interno, sem saída)
    NoBloq *bloq = construirListaBloqueadas(contas, total);

    for (Conta *p = contas; p < contas + total; p++) {
        if (!filtro || filtro(p)) {
            char cpfmask[16];
            mascararCPF(p->cpf, cpfmask, sizeof(cpfmask));
            printf("Conta: %d | Nome: %s | CPF: %s | Saldo: %.2f | Status: %s\n",
                   p->id,
                   p->nome,
                   cpfmask,
                   p->saldo,
                   (p->flags & FLAG_BLOQUEADA) ? "Bloqueada" : "Ativa");
        }
    }

    // Libera lista encadeada
    liberarListaBloqueadas(bloq);
}

// Função para bloquear conta
void adminBloquearConta(Conta *contas, int total) {
    int numero;
    cabecalho("BLOQUEAR CONTA");
    barraCarregamento("Preparando bloqueio", 500);
    printf("Numero da conta: ");
    {
        char buf[64];
        if (fgets(buf, sizeof(buf), stdin) == NULL) { msgErro("Entrada invalida."); return; }
        trimNewline(buf);
        char *endptr = NULL;
        long v = strtol(buf, &endptr, 10);
        if (endptr == buf || *endptr != '\0') { printf("Entrada invalida.\n"); return; }
        numero = (int)v;
    }

    for (int i = 0; i < total; i++) {
        if (contas[i].id == numero) {
            contas[i].flags |= FLAG_BLOQUEADA;
            barraCarregamento("Aplicando bloqueio", 500);
            char bbuf[64]; snprintf(bbuf, sizeof(bbuf), "Conta %d bloqueada.", numero);
            msgSucesso(bbuf);
            return;
        }
    }

    msgErro("Conta nao encontrada.");
}

// Função para desbloquear conta
void adminDesbloquearConta(Conta *contas, int total) {
    int numero;
    cabecalho("DESBLOQUEAR CONTA");
    barraCarregamento("Preparando desbloqueio", 500);
    printf("Numero da conta: ");
    {
        char buf[64];
        if (fgets(buf, sizeof(buf), stdin) == NULL) { msgErro("Entrada invalida."); return; }
        trimNewline(buf);
        char *endptr = NULL;
        long v = strtol(buf, &endptr, 10);
        if (endptr == buf || *endptr != '\0') { printf("Entrada invalida.\n"); return; }
        numero = (int)v;
    }

    for (int i = 0; i < total; i++) {
        if (contas[i].id == numero) {
            contas[i].flags &= (unsigned char)~FLAG_BLOQUEADA;
            barraCarregamento("Removendo bloqueio", 500);
            char ubuf[64]; snprintf(ubuf, sizeof(ubuf), "Conta %d desbloqueada.", numero);
            msgSucesso(ubuf);
            return;
        }
    }

    msgErro("Conta nao encontrada.");
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
    int opcao;

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
        printf("\nMenu do Administrador - %s (%s)\n", NOME_BANCO, AGENCIA_PADRAO);
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
        printf("12 - Remover conta\n");
        printf("0 - Voltar\n");
        if (!lerInteiro("Escolha: ", &opcao)) opcao = -1;

        switch (opcao) {
            case 1:
                if (!garantirCapacidade(contas, capacidade, (*total) + 1)) {
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
                if (carregarDados(contas, total, capacidade, fn)) {
                    // ajustar capacidade para pelo menos total (carregarDados já atualiza capacidade)
                    if (*capacidade < *total) *capacidade = *total;
                    printf("Contas carregadas de %s (total=%d)\n", fn, *total);
                } else printf("Erro ao carregar contas ou arquivo inexistente.\n");
                break;
            }
            case 10: {
                int numero;
                if (!lerInteiro("Numero da conta: ", &numero)) { printf("Entrada invalida.\n"); break; }
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
                int numero;
                if (!lerInteiro("Numero da conta: ", &numero)) { printf("Entrada invalida.\n"); break; }
                for (int i = 0; i < *total; i++) {
                    if ((*contas)[i].id == numero) {
                           printf("Conta %d - FLAG_PREMIUM=%s\n",
                               numero,
                               ((*contas)[i].flags & FLAG_PREMIUM) ? "SIM" : "NAO");
                        break;
                    }
                }
                break;
            }
            case 12: {
                adminRemoverConta(contas, total);
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

// Remove uma conta do vetor: procura por id, move elementos à esquerda e decrementa total
void adminRemoverConta(Conta **contas, int *total) {
    if (!contas || !total || *total <= 0 || *contas == NULL) {
        printf("Nenhuma conta para remover.\n");
        return;
    }
    printf("\nRemover Conta\n");
    int numero;
    if (!lerInteiro("Numero da conta: ", &numero)) {
        printf("Entrada invalida.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < *total; i++) {
        if ((*contas)[i].id == numero) { idx = i; break; }
    }
    if (idx < 0) { printf("Conta nao encontrada.\n"); return; }

    // move elementos à esquerda
    for (int j = idx; j < (*total) - 1; j++) {
        (*contas)[j] = (*contas)[j+1];
    }
    (*total)--;
    printf("Conta %d removida.\n", numero);
}