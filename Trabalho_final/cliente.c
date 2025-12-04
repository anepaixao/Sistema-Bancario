#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "banco.h"
#include "unir.h"

// Deixei os protótipos das telas/visuais aqui em cima
// só para manter a organização do arquivo
// --- PROTÓTIPOS DAS FUNÇÕES VISUAIS ---
void boleto(void);
void qr_code(void);
void plano_protecaoCONTRATO(void);
void plano_protecaoCOMPROVANTE(void);
void plano_Limitehorarioparatransacoes(void);
void plano_Limitehorarioparatransacoescomprovante(void);
void plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(void);
void plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(void);

// --- FUNÇÕES AUXILIARES ---

// Corrigi erro de escopo e concentrei as flags aqui
// Optei por usar bitwise para guardar várias configurações
// em um único byte (economiza memória e fica rápido de checar)
#define NOTIFICACOES_ATIVAS (1u << 4)
#define TRANSACOES_NOTURNAS (1u << 5)
#define CONFIRMACAO_VALORES (1u << 6)
void ativar_configuracao(unsigned char *config, unsigned char flag) {
    *config |= flag;
    printf("Configuracao ativada.\n");
}

void desativar_configuracao(unsigned char *config, unsigned char flag) {
    *config &= ~flag;
    printf("Configuracao desativada.\n");
}

int verificar_configuracao(unsigned char config, unsigned char flag) {
    return (config & flag) != 0;
}

// Resolvi mostrar recursão com um exemplo simples
// de rendimento composto mês a mês
double deposito_recursivo_simulado(double saldo, double taxa, int meses) {
    if (meses <= 0) return saldo;
    return deposito_recursivo_simulado(saldo * (1 + taxa), taxa, meses - 1);
}

// Implementação do fluxo do cliente

// Login do cliente: peço CPF (com máscara) e senha mascarada
// e valido procurando no vetor de contas
int login(Conta *contas, int total, int *outIdx) {
    char cpf_digitado[15];
    char senha_digitada[20];
    int tentativas = 3;

    cabecalho("LOGIN DO CLIENTE"); // Do unir.c
    barraCarregamento("Preparando tela de login", 400);

    while(tentativas > 0) {
        limparEntrada();
        lerCPFmascarado(cpf_digitado, sizeof(cpf_digitado));
        // Normaliza CPF digitado para apenas digitos antes da busca
        char cpf_dig[16]; apenasDigitos(cpf_digitado, cpf_dig);
        int idx = buscarIndicePorCPF(contas, total, cpf_dig); // unir.c

        if (idx == -1) {
            barraCarregamento("Buscando CPF", 300);
            msgErro("CPF nao encontrado.");
            tentativas--;
            continue;
        }

        lerSenhamascarada(senha_digitada, sizeof(senha_digitada));

        if (strcmp(contas[idx].senha, senha_digitada) == 0) {
            barraCarregamento("Validando credenciais", 400);
            msgSucesso("Login realizado com sucesso!");
            *outIdx = idx;
            return 1; // Sucesso
        } else {
            tentativas--;
            char buf[64]; snprintf(buf, sizeof(buf), "Senha incorreta. Tentativas restantes: %d", tentativas);
            msgErro(buf);
        }
    }
    msgErro("Numero maximo de tentativas atingido.");
    return 0;
}

// Operações financeiras básicas do dia a dia
void deposito(Conta *c, double valor) {
    if (valor <= 0) {
        msgErro("Valor invalido.");
        return;
    }
    barraCarregamento("Processando deposito", 500);
    c->saldo += (float)valor;
    char buf[64]; snprintf(buf, sizeof(buf), "+ R$ %.2f", valor);
    msgSucesso(buf);
    registrarLog(c->id, "Deposito");
}

void saque(Conta *c, double valor) {
    if (c->flags & FLAG_BLOQUEADA) {
        msgErro("Conta bloqueada. Operacao nao permitida.");
        return;
    }
    if (valor > c->saldo) {
        msgErro("Saldo insuficiente.");
        return;
    }
    barraCarregamento("Processando saque", 500);
    c->saldo -= (float)valor;
    char buf2[64]; snprintf(buf2, sizeof(buf2), "Saque de R$ %.2f realizado.", valor);
    msgSucesso(buf2);
    registrarLog(c->id, "Saque");
}

void transferencia_bancaria(Conta *contas, int total, int idx) {
    Conta *c = &contas[idx];
    double valor;
    int tipo;

    limparTela(); cabecalho("TRANSFERENCIA BANCARIA");
    barraCarregamento("Preparando transferencia", 400);
    printf("Saldo disponivel: R$ %.2f\n", c->saldo);
    // Pergunta o valor primeiro (ajuste pedido)
    if (!lerDouble("Valor a transferir: R$ ", &valor)) { printf("Entrada invalida.\n"); return; }

    if (!lerInteiro("1 - PIX\n2 - Transferencia entre contas (TED)\nEscolha: ", &tipo)) return;

    if (tipo == 1) { // PIX
        // Lógica dos menus adaptada (aqui apenas opções de chave/visualizacao)
        int b;
        printf("1 - Gerar chave aleatoria\n2 - Cadastro nova chave\n3 - Ver chaves\n4 - QR Code\n");
        if (!lerInteiro(NULL, &b)) return;
            switch(b) {
            case 1: { barraCarregamento("Gerando chave PIX", 400); char cpfmask[16]; mascararCPF(c->cpf, cpfmask, sizeof(cpfmask)); msgSucesso("Chave gerada com sucesso"); printf("Nova chave: %s-pix\n", cpfmask); break; } // pix_aleatorio
            case 2: barraCarregamento("Cadastrando chave PIX", 400); msgSucesso("Chave cadastrada com sucesso"); break; // pix_cadastro
            case 3: barraCarregamento("Carregando chaves", 400); printf("Chaves: CPF, Email.\n"); break; // pix_cadastrados
            case 4: barraCarregamento("Gerando QR Code", 400); qr_code(); break; // Visual
        }
        // Nota: aqui nao implementamos fluxo de pagamento via PIX por chave; pode ser estendido
    } 
    else if (tipo == 2) { // TED
        if (c->flags & FLAG_BLOQUEADA) {
            msgErro("Conta bloqueada!"); return;
        }
        int idDest;
        if (!lerInteiro("Numero da conta destino: ", &idDest)) return;
        int idxDest = buscarIndicePorId(contas, total, idDest); // unir.c

        if (idxDest == -1) {
            msgErro("Conta destino nao encontrada.");
        } else {
            if (valor > 0 && c->saldo >= valor) {
                barraCarregamento("Realizando transferencia TED", 500);
                c->saldo -= valor;
                contas[idxDest].saldo += valor;
                msgSucesso("Transferencia realizada!");
                registrarLog(c->id, "Transferencia TED");
                // Gerar comprovante HTML da transferencia TED
                if (!gerarComprovanteHTML(c->id, idDest, valor, "Transferencia TED")) {
                    msgErro("Falha ao gerar comprovante HTML.");
                } else {
                    printInfo("Comprovante gerado em output/comprovante.html");
                }
            } else {
                msgErro("Saldo insuficiente ou valor invalido.");
            }
        }
    }
}

// Menu de extras que eu incluí: contratos, flags (bitwise) e simulação
void outras_opcoes(Conta *c) {
    int opcao_principal = 0;

    printf("\n=== OUTRAS OPCOES ===\n");
    printf("1 - Planos de Seguranca (Contratos)\n");
    printf("2 - Configuracoes do Sistema (Bitwise)\n");
    printf("3 - Simulacao Rendimento (Recursao)\n");
    printf("0 - Voltar\n");
    if (!lerInteiro("Escolha: ", &opcao_principal)) return;

    switch(opcao_principal) {
        case 1: { // CONTRATOS
            int plano;
            printf("1. Protecao Fraudes\n2. Limite Horario\n3. Confirmacao Senha\n");
            if (!lerInteiro(NULL, &plano)) return;
            if(plano==1) { plano_protecaoCONTRATO(); plano_protecaoCOMPROVANTE(); }
            if(plano==2) { plano_Limitehorarioparatransacoes(); plano_Limitehorarioparatransacoescomprovante(); }
            if(plano==3) { plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(); plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(); }
            break;
        }
        case 2: { // BITWISE (Corrigido para usar c->flags)
            printf("\n=== CONFIGURACOES (BITWISE) ===\n");
            printf("Flags Atuais: 0x%02X\n", c->flags);
            printf("1 - Notificacoes: %s\n", verificar_configuracao(c->flags, NOTIFICACOES_ATIVAS) ? "ON" : "OFF");
            printf("2 - Transacoes Noturnas: %s\n", verificar_configuracao(c->flags, TRANSACOES_NOTURNAS) ? "ON" : "OFF");
            
            printf("Alterar: 1-Notificacoes 2-Noturnas: ");
            int op;
            if (!lerInteiro(NULL, &op)) return;
            if (op == 1) {
                if (verificar_configuracao(c->flags, NOTIFICACOES_ATIVAS)) desativar_configuracao(&c->flags, NOTIFICACOES_ATIVAS);
                else ativar_configuracao(&c->flags, NOTIFICACOES_ATIVAS);
            }
            if (op == 2) {
                if (verificar_configuracao(c->flags, TRANSACOES_NOTURNAS)) desativar_configuracao(&c->flags, TRANSACOES_NOTURNAS);
                else ativar_configuracao(&c->flags, TRANSACOES_NOTURNAS);
            }
            break;
        }
        case 3: { // RECURSÃO (Adaptada)
            int m;
            printf("Meses para simular (1%% a.m): ");
            if (!lerInteiro(NULL, &m)) return;
            double res = deposito_recursivo_simulado(c->saldo, 0.01, m);
            printf("Resultado: R$ %.2f\n", res);
            break;
        }
    }
    pausarTela();
}

// Tela principal do cliente (menu). Uso a agência padrão do banco
int tela_principal(Conta *contas, int total, int idx) {
    Conta *c = &contas[idx];
    int opc = 0;

    // Use a agência padrão do banco
    char agencia[] = AGENCIA_PADRAO;

    while (1) {
        limparTela(); cabecalho("TELA PRINCIPAL");
        // Cabeçalho com CPF mascarado para exibicao
        char cpfmask[16]; mascararCPF(c->cpf, cpfmask, sizeof(cpfmask));
        printf("AGENCIA: %s\t  SALDO: %.2f\t  CPF: %s\n\n", agencia, c->saldo, cpfmask);
        printf("1 - EXTRATO\n2 - DEPOSITO\n3 - SAQUE\n4 - TRANSFERENCIA\n5 - OUTRAS OPCOES\n0 - SAIR\n");
        printf("Escolha: ");
        
        char opcbuf[16];
        if (fgets(opcbuf, sizeof(opcbuf), stdin) == NULL) break;
        opc = atoi(opcbuf);

        if (opc == 0) break;
        else if (opc == 1) {
            limparTela(); cabecalho("EXTRATO");
            printf("Extrato - Conta %d | Nome: %s\nSaldo: R$ %.2f\n", c->id, c->nome, c->saldo);
            pausarTela();
        } 
        else if (opc == 2) {
            printf("1-Dinheiro 2-Boleto: ");
            int t;
            if (!lerInteiro(NULL, &t)) { printf("Entrada invalida.\n"); continue; }

            double v;
            if (!lerDouble("Valor: ", &v)) { printf("Entrada invalida.\n"); continue; }
            if(t==2) {
                barraCarregamento("Gerando boleto", 500);
                gerarBoletoHTML(v, "Deposito por boleto");
            }
            barraCarregamento("Efetuando deposito", 500);
            deposito(c, v);
            pausarTela();
        } 
        else if (opc == 3) {
            double v;
            if (!lerDouble("Valor: ", &v)) { printf("Entrada invalida.\n"); continue; }
            barraCarregamento("Efetuando saque", 500);
            saque(c, v);
            pausarTela();
        } 
        else if (opc == 4) {
            transferencia_bancaria(contas, total, idx);
            pausarTela();
        } 
        else if (opc == 5) {
            outras_opcoes(c);
        } 
        else {
            msgErro("Opcao invalida.");
        }
    }
    return 0;
}

// Menu do cliente — é por aqui que a main entra no módulo
void clienteMenu(Conta **contas, int *total, int *capacidade) {
    int opc = 0;
    // Garante que o vetor esteja alocado se a capacidade ja foi definida
    if (*contas == NULL && *capacidade > 0) {
        *contas = (Conta *)malloc((size_t)(*capacidade) * sizeof(Conta));
        if (!*contas) {
            printf("Erro ao alocar memoria para contas.\n");
            return;
        }
    }
    while(1) {
        limparTela(); cabecalho("CLIENTE");
        printf("1. Login\n2. Criar Conta\n0. Voltar\n");
        printf("Escolha: ");
        {
            char ibuf[32]; if (fgets(ibuf, sizeof(ibuf), stdin) == NULL) return; trimNewline(ibuf);
            char *e = NULL; long tv = strtol(ibuf, &e, 10); if (e == ibuf || *e != '\0') { opc = -1; } else opc = (int)tv;
        }

        if (opc == 0) break;
        if (opc == 1) {
            int idx = -1;
            barraCarregamento("Autenticando", 600);
            if (login(*contas, *total, &idx)) {
                tela_principal(*contas, *total, idx);
            }
        }
        if (opc == 2) {
            barraCarregamento("Criando conta", 700);
            clienteCriarConta(contas, total, capacidade); // Criar Conta
            pausarTela();
        }
    }
}

// Criar conta: fluxo guiado com validação de CPF e senha
void clienteCriarConta(Conta **contas, int *total, int *capacidade) {
    // Se ponteiro nulo mas capacidade > 0, aloca primeiro
    if (*contas == NULL) {
        if (*capacidade <= 0) *capacidade = 2;
        *contas = (Conta *)malloc((size_t)(*capacidade) * sizeof(Conta));
        if (!*contas) { printf("Erro ao alocar memoria.\n"); return; }
    }
    if (*total >= *capacidade) {
        int nova = (*capacidade == 0) ? 2 : (*capacidade * 2);
        Conta *tmp = (Conta *)realloc(*contas, (size_t)nova * sizeof(Conta));
        if (!tmp) { printf("Erro ao realocar memoria.\n"); return; }
        *contas = tmp;
        *capacidade = nova;
    }

    Conta *c = &(*contas)[*total];
    c->id = gerarProximoId(*contas, *total); // unir.c

    printf("Nome: ");
    fgets(c->nome, 50, stdin); trimNewline(c->nome);

    // Ler e validar CPF (mascarado com opcao de visualizar)
    while (1) {
        char cpfbuf[32];
        lerCPFmascarado(cpfbuf, sizeof(cpfbuf));
        if (!validarCPF(cpfbuf)) { msgErro("CPF invalido. Use 11 digitos (com ou sem pontuacao)."); continue; }
        // Normaliza para apenas digitos e verifica duplicidade
        char dig[16]; apenasDigitos(cpfbuf, dig);
        if (buscarIndicePorCPF(*contas, *total, dig) != -1) { msgErro("CPF ja cadastrado."); continue; }
        // Armazena apenas os digitos no registro
        strncpy(c->cpf, dig, sizeof(c->cpf)-1);
        c->cpf[sizeof(c->cpf)-1] = '\0';
        break;
    }

    // Ler e validar senha (exatamente 6 digitos numericos) com confirmacao, mascarada
    while (1) {
        char s1[32];
        char s2[32];
        lerSenhamascarada(s1, sizeof(s1));
        if (!senhaValidaNDigitos(s1, 6)) { msgErro("Senha invalida: precisa ter exatamente 6 digitos numericos."); continue; }
        printf("Confirmar senha: ");
        lerSenhamascarada(s2, sizeof(s2));
        if (strcmp(s1, s2) != 0) { msgErro("Senhas diferentes. Tente novamente."); continue; }
        strncpy(c->senha, s1, sizeof(c->senha)-1);
        c->senha[sizeof(c->senha)-1] = '\0';
        break;
    }

    c->saldo = 0.0;
    c->flags = 0;
    (*total)++;
    
    char buf3[64]; snprintf(buf3, sizeof(buf3), "Conta Criada! ID: %d", c->id);
    msgSucesso(buf3);
    registrarLog(c->id, "Conta Criada");
}

// Funções visuais (efeito no terminal). Mantive simples só para ilustrar

void qr_code(void){
    printf("\n");
    printf("/////////////////////// QR CODE /////////////////////\n");
    printf("|||| |||||||  |||||| ||         ||||  ||||||||| ||\n");
    printf("||||                ||         ||||             ||\n");
    printf("|||| |||||||  |||||| ||         ||||             ||\n");
    printf("    ||    ||||||  |||          ||||  ||||||||| ||\n");
    printf(" ||||||      ||||||||||||||            |||||||||||  ||\n");
    printf("Escaneie com a camera do celular\n\n");
}

void boleto(void){
    printf("\n");
    printf("____________________________________________________\n");
    printf("|    AGENCIA  0001-C  |  00000002305402030900055|\n");
    printf("_________________________|_______________|__________|\n");
    printf("|    LOCAL DE PAGAMENTO  |  VENCIMENTO   | 10/12    |\n");
    printf("|    C-BANK S.A.         |  VALOR        | R$ ???   |\n");
    printf("____________________________________________________\n");
}

void plano_protecaoCONTRATO(){
    double valor;
    if (!lerDouble("Valor do contrato (se aplicavel): R$ ", &valor)) { msgErro("Entrada invalida."); return; }
    const char *texto =
        "CLÁUSULA PRIMEIRA - DO OBJETO\n"
        "O presente termo disciplina a ativação do serviço de Proteção contra Fraudes (PIX).\n\n"
        "CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO\n"
        "a) Monitoramento de transações suspeitas em tempo real\n"
        "b) Bloqueio preventivo com notificação ao cliente\n"
        "c) Canal prioritário para análise de eventos\n\n"
        "CLÁUSULA TERCEIRA - PRAZO\n"
        "Vigência por prazo indeterminado, com cancelamento a pedido do cliente.\n\n"
        "CLÁUSULA QUARTA - OBRIGAÇÕES DO CLIENTE\n"
        "a) Manter sigilo das credenciais\n"
        "b) Notificar o banco imediatamente em caso de suspeita\n\n"
        "CLÁUSULA QUINTA - RESPONSABILIDADE\n"
        "O BANCO indenizará prejuízos por falhas comprovadas em seus sistemas, excetuadas hipóteses de negligência do cliente.\n\n"
        "CLÁUSULA SEXTA - CANAL DE ATENDIMENTO\n"
        "0800-000-0000 (24 horas).\n\n"
        "CLÁUSULA SÉTIMA - ACEITE\n"
        "Ao confirmar, o CLIENTE declara ter lido e concordado com os termos.";
    gerarContratoHTMLComTexto("Contrato: Protecao Fraudes (PIX)", valor, 1, texto);
}


void plano_protecaoCOMPROVANTE(){
    double valor;
    if (!lerDouble("Valor do comprovante: R$ ", &valor)) { msgErro("Entrada invalida."); return; }
    gerarComprovanteHTML(0, 0, valor, "Comprovante Protecao Fraudes (PIX)");
}

void plano_Limitehorarioparatransacoes(){
    double valor;
    if (!lerDouble("Valor do contrato (se aplicavel): R$ ", &valor)) { msgErro("Entrada invalida."); return; }
    const char *texto =
        "CLÁUSULA PRIMEIRA - DO OBJETO\n"
        "O presente termo disciplina a ativação do sistema de limite horário para transações financeiras realizadas pelo CLIENTE através dos canais PIX e Cartão de Crédito/Débito.\n\n"
        "CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO\n"
        "Ficam estabelecidos os seguintes limites horários:\n"
        "a) PERÍODO NOTURNO RESTRITO: Transações bloqueadas entre 23h e 5h\n"
        "b) PERÍODO DIURNO LIVRE: Transações permitidas entre 5h e 23h\n"
        "c) EXCEÇÕES: Serviços essenciais (hospitais, farmácias 24h) permanecem liberados mediante análise prévia\n\n"
        "CLÁUSULA TERCEIRA - DOS CANAIS AFETADOS\n"
        "Aplica-se a:\n"
        "a) Todas as transações PIX (envio e recebimento)\n"
        "b) Compras com cartão de débito e crédito\n"
        "c) Saques em terminais eletrônicos\n"
        "d) Transferências entre contas\n\n"
        "CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE\n"
        "a) Manter sigilo de suas credenciais de acesso\n"
        "b) Notificar o BANCO imediatamente em caso de perda ou roubo do dispositivo\n"
        "c) Verificar regularmente o extrato de movimentações\n"
        "d) Utilizar senhas complexas e atualizá-las periodicamente\n\n"
        "CLÁUSULA QUINTA - DA RESPONSABILIDADE\n"
        "O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas, excluídas as situações de negligência do CLIENTE.\n\n"
        "CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO\n"
        "Para acionamento do serviço em caso de suspeita de fraude: 0800-000-0000, 24 horas.\n\n"
        "CLÁUSULA SÉTIMA - DO ACEITE\n"
        "Ao confirmar, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.\n\n"
        "Local e Data: ______, ___ de ________ de 2025\n"
        "Assinatura do Cliente: _________________________\n"
        "Carimbo e Assinatura do Banco: _________________\n\n"
        "Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.";
    gerarContratoHTMLComTexto("Contrato: Limite de Horario para Transacoes", valor, 1, texto);
}


void plano_Limitehorarioparatransacoescomprovante(){
    double valor;
    if (!lerDouble("Valor do comprovante: R$ ", &valor)) { msgErro("Entrada invalida."); return; }
    gerarComprovanteHTML(0, 0, valor, "Comprovante Limite de Horario");
}

void plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(){
    double valor;
    if (!lerDouble("Valor do contrato (se aplicavel): R$ ", &valor)) { msgErro("Entrada invalida."); return; }
    const char *texto =
        "CLÁUSULA PRIMEIRA - DO OBJETO\n"
        "O presente termo tem por objeto a contratação, pelo CLIENTE, do serviço de Confirmação por Senha para Valores Altos oferecido pelo BANCO.\n\n"
        "CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO\n"
        "O serviço de Confirmação por Senha para Valores Altos inclui:\n"
        "a) Confirmação obrigatória para transações PIX superiores a R$ 1.000,00\n"
        "b) Autenticação adicional para transferências TED/DOC acima de R$ 2.000,00\n"
        "c) Verificação por senha única para compras com cartão acima de R$ 500,00\n"
        "d) Dupla validação para pagamentos de contas superiores a R$ 800,00\n"
        "e) Notificação imediata via aplicativo para todas as transações de alto valor\n\n"
        "CLÁUSULA TERCEIRA - DO PRAZO\n"
        "O serviço terá vigência por prazo indeterminado, podendo ser cancelado a qualquer tempo pelo CLIENTE, sem ônus.\n\n"
        "CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE\n"
        "a) Manter sigilo absoluto de suas senhas de confirmação\n"
        "b) Não compartilhar códigos de verificação com terceiros\n"
        "c) Notificar o BANCO imediatamente em caso de solicitação não autorizada\n"
        "d) Utilizar senhas distintas das utilizadas para acesso habitual\n\n"
        "CLÁUSULA QUINTA - DA RESPONSABILIDADE\n"
        "O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas de confirmação, excluídas as situações de negligência do CLIENTE.\n\n"
        "CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO\n"
        "Para desbloqueio emergencial ou reporte de problemas: 0800-000-0000, 24 horas.\n\n"
        "CLÁUSULA SÉTIMA - DO ACEITE\n"
        "Ao confirmar, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.\n\n"
        "Local e Data: ______, ___ de ________ de 2025\n"
        "Assinatura do Cliente: _________________________\n"
        "Carimbo e Assinatura do Banco: _________________\n\n"
        "Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.";
    gerarContratoHTMLComTexto("Contrato: Confirmacao por Senha para Valores Altos", valor, 1, texto);
}

void plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(){
    double valor;
    if (!lerDouble("Valor do comprovante: R$ ", &valor)) { msgErro("Entrada invalida."); return; }
    gerarComprovanteHTML(0, 0, valor, "Comprovante Confirmacao por Senha");
}