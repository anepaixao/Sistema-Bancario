#ifdef _WIN32
#include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "banco.h"
#include "unir.h"

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

// Bitwise: Movido para fora para corrigir erro de escopo
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

// Recursão: Simulação de Rendimento (Adaptada)
double deposito_recursivo_simulado(double saldo, double taxa, int meses) {
    if (meses <= 0) return saldo;
    return deposito_recursivo_simulado(saldo * (1 + taxa), taxa, meses - 1);
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

// Mantive o nome 'login', mas agora recebe os dados reais
int login(Conta *contas, int total, int *outIdx) {
    char cpf_digitado[15];
    char senha_digitada[20];
    int tentativas = 3;

    cabecalho("LOGIN DO CLIENTE"); // Do unir.c

    while(tentativas > 0) {
        limparEntrada();
        lerCPFmascarado(cpf_digitado, sizeof(cpf_digitado));
        // Normaliza CPF digitado para apenas digitos antes da busca
        char cpf_dig[16]; apenasDigitos(cpf_digitado, cpf_dig);
        int idx = buscarIndicePorCPF(contas, total, cpf_dig); // unir.c

        if (idx == -1) {
            printf("CPF nao encontrado.\n");
            tentativas--;
            continue;
        }

        lerSenhamascarada(senha_digitada, sizeof(senha_digitada));

        if (strcmp(contas[idx].senha, senha_digitada) == 0) {
            printf("Login realizado com sucesso!\n");
            *outIdx = idx;
            return 1; // Sucesso
        } else {
            tentativas--;
            printf("Senha incorreta. Tentativas restantes: %d\n", tentativas);
        }
    }
    printf("Numero maximo de tentativas atingido.\n");
    return 0;
}

// Funções financeiras simples
void deposito(Conta *c, double valor) {
    if (valor <= 0) {
        printf("Valor invalido.\n");
        return;
    }
    c->saldo += (float)valor;
    printf("+ R$ %.2f\n", valor);
    registrarLog(c->id, "Deposito");
}

void saque(Conta *c, double valor) {
    if (c->flags & FLAG_BLOQUEADA) {
        printf("Conta bloqueada. Operacao nao permitida.\n");
        return;
    }
    if (valor > c->saldo) {
        printf("Saldo insuficiente.\n");
        return;
    }
    c->saldo -= (float)valor;
    printf("Saque de R$ %.2f realizado.\n", valor);
    registrarLog(c->id, "Saque");
}

void transferencia_bancaria(Conta *contas, int total, int idx) {
    Conta *c = &contas[idx];
    double valor;
    int tipo;

    printf("\n=== TRANSFERENCIA BANCARIA ===\n");
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
            case 1: printf("Nova chave: %s-pix\n", c->cpf); break; // pix_aleatorio
            case 2: printf("Chave cadastrada com sucesso.\n"); break; // pix_cadastro
            case 3: printf("Chaves: CPF, Email.\n"); break; // pix_cadastrados
            case 4: qr_code(); break; // Visual
        }
        // Nota: aqui nao implementamos fluxo de pagamento via PIX por chave; pode ser estendido
    } 
    else if (tipo == 2) { // TED
        if (c->flags & FLAG_BLOQUEADA) {
            printf("Conta bloqueada!\n"); return;
        }
        int idDest;
        if (!lerInteiro("Numero da conta destino: ", &idDest)) return;
        int idxDest = buscarIndicePorId(contas, total, idDest); // unir.c

        if (idxDest == -1) {
            printf("Conta destino nao encontrada.\n");
        } else {
            if (valor > 0 && c->saldo >= valor) {
                c->saldo -= valor;
                contas[idxDest].saldo += valor;
                printf("Transferencia realizada!\n");
                registrarLog(c->id, "Transferencia TED");
            } else {
                printf("Saldo insuficiente ou valor invalido.\n");
            }
        }
    }
}

// Mantendo o nome 'outras_opcoes' que ela usou para configurações
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
            if(plano==1) { plano_protecaoCONTRATO(); pausarTela(); plano_protecaoCOMPROVANTE(); }
            if(plano==2) { plano_Limitehorarioparatransacoes(); pausarTela(); plano_Limitehorarioparatransacoescomprovante(); }
            if(plano==3) { plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(); pausarTela(); plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(); }
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

// Mantive o nome 'tela_principal', mas removi o malloc errado de agencia
int tela_principal(Conta *contas, int total, int idx) {
    Conta *c = &contas[idx];
    int opc = 0;

    // Use a agência padrão do banco
    char agencia[] = AGENCIA_PADRAO;

    while (1) {
        limparTela();
        // Cabeçalho igual ao dela
        printf("\nAGENCIA: %s\t  SALDO: %.2f\t  CPF: %s\n\n", agencia, c->saldo, c->cpf);
        printf("1 - EXTRATO\n2 - DEPOSITO\n3 - SAQUE\n4 - TRANSFERENCIA\n5 - OUTRAS OPCOES\n0 - SAIR\n");
        printf("Escolha: ");
        
        char opcbuf[16];
        if (fgets(opcbuf, sizeof(opcbuf), stdin) == NULL) break;
        opc = atoi(opcbuf);

        if (opc == 0) break;
        else if (opc == 1) {
            printf("Extrato - Conta %d | Nome: %s\nSaldo: R$ %.2f\n", c->id, c->nome, c->saldo);
            pausarTela();
        } 
        else if (opc == 2) {
            printf("1-Dinheiro 2-Boleto: ");
            int t;
            if (!lerInteiro(NULL, &t)) { printf("Entrada invalida.\n"); continue; }

            double v;
            if (!lerDouble("Valor: ", &v)) { printf("Entrada invalida.\n"); continue; }
            if(t==2) boleto();
            deposito(c, v);
            pausarTela();
        } 
        else if (opc == 3) {
            double v;
            if (!lerDouble("Valor: ", &v)) { printf("Entrada invalida.\n"); continue; }
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
            printf("Opcao invalida.\n");
        }
    }
    return 0;
}

// Ponto de entrada (chamado pela main)
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
        limparTela();
        printf("\n--- %s ---\n", NOME_BANCO);
        printf("1. Login\n2. Criar Conta\n0. Voltar\n");
        printf("Escolha: ");
        {
            char ibuf[32]; if (fgets(ibuf, sizeof(ibuf), stdin) == NULL) return; trimNewline(ibuf);
            char *e = NULL; long tv = strtol(ibuf, &e, 10); if (e == ibuf || *e != '\0') { opc = -1; } else opc = (int)tv;
        }

        if (opc == 0) break;
        if (opc == 1) {
            int idx = -1;
            if (login(*contas, *total, &idx)) {
                tela_principal(*contas, *total, idx);
            }
        }
        if (opc == 2) {
            clienteCriarConta(contas, total, capacidade); // Criar Conta
            pausarTela();
        }
    }
}

// --- FUNÇÃO DE CRIAR CONTA ---
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

    // Ler e validar CPF (usa validarCPF em unir.c)
    while (1) {
        char cpfbuf[32];
        printf("CPF: ");
        if (fgets(cpfbuf, sizeof(cpfbuf), stdin) == NULL) { printf("Erro de leitura.\n"); return; }
        trimNewline(cpfbuf);
        if (!validarCPF(cpfbuf)) { printf("CPF invalido. Use 11 digitos (com ou sem pontuacao).\n"); continue; }
        // Normaliza para apenas digitos e verifica duplicidade
        char dig[16]; apenasDigitos(cpfbuf, dig);
        if (buscarIndicePorCPF(*contas, *total, dig) != -1) { printf("CPF ja cadastrado.\n"); continue; }
        // Armazena apenas os digitos no registro
        strncpy(c->cpf, dig, sizeof(c->cpf)-1);
        c->cpf[sizeof(c->cpf)-1] = '\0';
        break;
    }

    // Ler e validar senha (exatamente 6 digitos numericos) com confirmacao
    while (1) {
        char s1[32];
        char s2[32];
        printf("Senha (6 digitos): ");
        if (fgets(s1, sizeof(s1), stdin) == NULL) { printf("Erro de leitura.\n"); return; }
        trimNewline(s1);
        if (!senhaValidaNDigitos(s1, 6)) { printf("Senha invalida: precisa ter exatamente 6 digitos numericos.\n"); continue; }
        printf("Confirmar senha: ");
        if (fgets(s2, sizeof(s2), stdin) == NULL) { printf("Erro de leitura.\n"); return; }
        trimNewline(s2);
        if (strcmp(s1, s2) != 0) { printf("Senhas diferentes. Tente novamente.\n"); continue; }
        strncpy(c->senha, s1, sizeof(c->senha)-1);
        c->senha[sizeof(c->senha)-1] = '\0';
        break;
    }

    c->saldo = 0.0;
    c->flags = 0;
    (*total)++;
    
    printf("Conta Criada! ID: %d\n", c->id);
    registrarLog(c->id, "Conta Criada");
}

// --- FUNÇÕES VISUAIS ---

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
    printf("|    AGENCIA  DO CENTRO  |  00000002305402030900055|\n");
    printf("_________________________|_______________|__________|\n");
    printf("|    LOCAL DE PAGAMENTO  |  VENCIMENTO   | 10/12    |\n");
    printf("|    C-BANK S.A.         |  VALOR        | R$ ???   |\n");
    printf("____________________________________________________\n");
}

void plano_protecaoCONTRATO(){
 printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
printf("|                                                                                                                                                                                                             |");
printf("| CLÁUSULA PRIMEIRA - DO OBJETO                                                                                                                                                                               |");
printf("| O presente termo tem por objeto a contratação, pelo CLIENTE, do serviço de Proteção PIX oferecido pelo BANCO, que compreende um conjunto de medidas de segurança para transações via sistema PIX.             |");
printf("|                                                                                                                                                                                                             |");
printf("|CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO                                                                                                                                                            |");
printf("|O serviço de Proteção PIX inclui:                                                                                                                                                                            |");
printf("|a) Limite horário para transações: operações bloqueadas das 22h às 6h                                                                                                                                        |");
printf("|b) Confirmação adicional para valores superiores a R$ 1.000,00                                                                                                                                               |");
printf("|c) Monitoramento de transações atípicas                                                                                                                                                                      |");
printf("|d) Seguro contra fraudes comprovadas até o limite de R$ 10.000,00                                                                                                                                            |");
printf("|e) Notificação imediata via SMS para todas as transações                                                                                                                                                     |");
printf("|                                                                                                                                                                                                             |");
printf("|CLÁUSULA TERCEIRA - DO PRAZO                                                                                                                                                                                 |");
printf("|O serviço terá vigência por prazo indeterminado, podendo ser cancelado a qualquer tempo pelo CLIENTE, sem ônus.                                                                                              |");
printf("|                                                                                                                                                                                                             |");
printf("|CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE                                                                                                                                                                  |");
printf("|O CLIENTE se compromete a:                                                                                                                                                                                   |");
printf("|a) Manter sigilo de suas credenciais de acesso                                                                                                                                                               |");
printf("|b) Notificar o BANCO imediatamente em caso de perda ou roubo do dispositivo                                                                                                                                  |");
printf("|c) Verificar regularmente o extrato de movimentações                                                                                                                                                         |");
printf("|d) Utilizar senhas complexas e atualizá-las periodicamente                                                                                                                                                   |");
printf("|                                                                                                                                                                                                             |");
printf("|CLÁUSULA QUINTA - DA RESPONSABILIDADE                                                                                                                                                                        |");
printf("|O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas, excluídas as situações de negligência do CLIENTE.                                                      |");
printf("|                                                                                                                                                                                                             |");
printf("|CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO                                                                                                                                                                     |");
printf("|Para acionamento do serviço em caso de suspeita de fraude: 0800-000-0000, 24 horas.                                                                                                                          |");
printf("|                                                                                                                                                                                                             |");
printf("|CLÁUSULA SÉTIMA - DO ACEITE                                                                                                                                                                                  |");
printf("|Ao confirmar ""SIM"" na adesão, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.                                                                                              |");
printf("|                                                                                                                                                                                                             |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                             |");
printf("|Local e Data: ______, ___ de ________ de 2025                                                                                                                                                                |");
printf("|                                                                                                                                                                                                             |");
printf("|Assinatura do Cliente: _________________________                                                                                                                                                             |");
printf("|                                                                                                                                                                                                             |");
printf("|Carimbo e Assinatura do Banco: _________________                                                                                                                                                             |");
printf("|                                                                                                                                                                                                             |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                       Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.                                                 |");
printf("|                                                                                                                                                                                                             |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("\n");

}


void plano_protecaoCOMPROVANTE(){

 printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |  000000-01             |  00000002305402030900055J607500 - 01    |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    FORMA DE PAGAMENTO                             |                DATA DO DOCUMENTO      |\n");
    printf("|                                                   |                                       |\n");
    printf("|    DEBITO AUTOMATICO!                             |                28/11/2025             |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 1.200,00                              |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|   O CODIGO DE BARRAS ABAIXO               |      DESCONTO\\ABATIMENTO                      |\n");
    printf("|   PODE SER UTILIZADO COMO COMPROVANTE     |      R$ 00,00                                 |\n");
    printf("|   DE PAGAMENTO,ACEITO EM QUALQUER         | _____________________________________________ |\n");
    printf("|   INSTITUIÇÃO DO BANCO OU PARCERIA                                                        |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    SACADO                                                                                 |\n");
    printf("|                                                                                           |\n");
    printf("|        ||   ||||||   |||||||||| || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        ||   ||||||   |||||||||| || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        ||   ||||||   |||||||||  || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846      054 0000000000000001     |\n");
    printf("____________________________________________________________________________________________\n");
}

void plano_Limitehorarioparatransacoes(){

printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA PRIMEIRA - DO OBJETO                                                                                                                                                                                        |");
printf("|O presente termo disciplina a ativação do sistema de limite horário para transações financeiras                                                                                                                      |");
printf("|realizadas pelo CLIENTE através dos canais PIX e Cartão de Crédito/Débito                                                                                                                                            |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO                                                                                                                                                                    |");
printf("|Ficam estabelecidos os seguintes limites horários:                                                                                                                                                                   |");
printf("|a) PERÍODO NOTURNO RESTRITO: Transações bloqueadas entre 23h e 5h                                                                                                                                                    |");
printf("|b) PERÍODO DIURNO LIVRE: Transações permitidas entre 5h e 23h                                                                                                                                                        |");
printf("|c) EXCEÇÕES: Serviços essenciais (hospitais, farmácias 24h) permanecem liberados mediante análise prévia                                                                                                             |");
printf("|                                                                                                                                                                                                                     |");
printf("|                                                                                                                                                                                                                     |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA TERCEIRA - DOS CANAIS AFETADOS                                                                                                                                                                              |");
printf("|O limite horário aplica-se a:                                                                                                                                                                                        |");
printf("|O limite horário aplica-se a:                                                                                                                                                                                        |");
printf("|a) Todas as transações PIX (envio e recebimento)                                                                                                                                                                     |");
printf("|b) Compras com cartão de débito e crédito                                                                                                                                                                            |");
printf("|c) Saques em terminais eletrônicos                                                                                                                                                                                   |");
printf("|d) Transferências entre contas                                                                                                                                                                                       |");
printf("|                                                                                                                                                                                                                     |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE                                                                                                                                                                          |");
printf("|O CLIENTE se compromete a:                                                                                                                                                                                           |");
printf("|a) Manter sigilo de suas credenciais de acesso                                                                                                                                                                       |");
printf("|b) Notificar o BANCO imediatamente em caso de perda ou roubo do dispositivo                                                                                                                                          |");
printf("|c) Verificar regularmente o extrato de movimentações                                                                                                                                                                 |");
printf("|d) Utilizar senhas complexas e atualizá-las periodicamente                                                                                                                                                           |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA QUINTA - DA RESPONSABILIDADE                                                                                                                                                                                |");
printf("|O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas, excluídas as situações de negligência do CLIENTE.                                                              |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO                                                                                                                                                                             |");
printf("|Para acionamento do serviço em caso de suspeita de fraude: 0800-000-0000, 24 horas.                                                                                                                                  |");
printf("|                                                                                                                                                                                                                     |");
printf("|CLÁUSULA SÉTIMA - DO ACEITE                                                                                                                                                                                          |");
printf("|Ao confirmar ""SIM"" na adesão, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.                                                                                                      |");
printf("|                                                                                                                                                                                                                     |");
printf("|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                                     |");
printf("|Local e Data: ______, ___ de ________ de 2025                                                                                                                                                                        |");
printf("|                                                                                                                                                                                                                     |");
printf("|Assinatura do Cliente: _________________________                                                                                                                                                                     |");
printf("|                                                                                                                                                                                                                     |");
printf("|Carimbo e Assinatura do Banco: _________________                                                                                                                                                                     |");
printf("|                                                                                                                                                                                                                     |");
printf("|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                       Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.                                                       |");
printf("|                                                                                                                                                                                                                     |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");                                                                                                                                                                                                printf("|");
puts("\n");

}


void plano_Limitehorarioparatransacoescomprovante(){

 printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |  000000-01             |  00000002305402030900055J607500 - 01    |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    FORMA DE PAGAMENTO                             |                DATA DO DOCUMENTO      |\n");
    printf("|                                                   |                                       |\n");
    printf("|    DEBITO AUTOMATICO!                             |                28/11/2025             |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 2.400,00                              |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|   O CODIGO DE BARRAS ABAIXO               |      DESCONTO\\ABATIMENTO                      |\n");
    printf("|   PODE SER UTILIZADO COMO COMPROVANTE     |      R$ 00,00                                 |\n");
    printf("|   DE PAGAMENTO,ACEITO EM QUALQUER         | _____________________________________________ |\n");
    printf("|   INSTITUIÇÃO DO BANCO OU PARCERIA                                                        |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    SACADO                                                                                 |\n");
    printf("|                                                                                           |\n");
    printf("|        ||   ||||||   |||||||||| || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        ||   ||||||   |||||||||| || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        ||   ||||||   |||||||||  || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846      054 0000000000000001     |\n");
    printf("____________________________________________________________________________________________\n");
}

void plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(){

printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                                |");
printf("|CLÁUSULA PRIMEIRA - DO OBJETO                                                                                                                                                                                   |");
printf("|O presente termo tem por objeto a contratação, pelo CLIENTE, do serviço de Confirmação por Senha para Valores Altos oferecido pelo BANCO.                                                                       |");
printf("|                                                                                                                                                                                                                |");
printf("|CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO                                                                                                                                                               |");
printf("|O serviço de Confirmação por Senha para Valores Altos inclui:                                                                                                                                                   |");
printf("|a) Confirmação obrigatória para transações PIX superiores a R$ 1.000,00                                                                                                                                         |");
printf("|b) Autenticação adicional para transferências TED/DOC acima de R$ 2.000,00                                                                                                                                      |");
printf("|c) Verificação por senha única para compras com cartão acima de R$ 500,00                                                                                                                                       |");
printf("|d) Dupla validação para pagamentos de contas superiores a R$ 800,00                                                                                                                                             |");
printf("|e) Notificação imediata via aplicativo para todas as transações de alto valor                                                                                                                                   |");
printf("|                                                                                                                                                                                                                |");
printf("|CLÁUSULA TERCEIRA - DO PRAZO                                                                                                                                                                                    |");
printf("|O serviço terá vigência por prazo indeterminado, podendo ser cancelado a qualquer tempo pelo CLIENTE, sem ônus.                                                                                                 |");
printf("|                                                                                                                                                                                                                |");
printf("|CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE                                                                                                                                                                     |");
printf("|O CLIENTE se compromete a:                                                                                                                                                                                      |");
printf("|a) Manter sigilo absoluto de suas senhas de confirmação                                                                                                                                                         |");
printf("|b) Não compartilhar códigos de verificação com terceiros                                                                                                                                                        |");
printf("|c) Notificar o BANCO imediatamente em caso de solicitação não autorizada                                                                                                                                        |");
printf("|d) Utilizar senhas distintas das utilizadas para acesso habitual                                                                                                                                                |");
printf("|                                                                                                                                                                                                                |");
printf("|CLÁUSULA QUINTA - DA RESPONSABILIDADE                                                                                                                                                                           |");
printf("|O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas de confirmação, excluídas as situações de negligência do CLIENTE.                                          |");
printf("|                                                                                                                                                                                                                |");
printf("|CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO                                                                                                                                                                        |");
printf("|Para desbloqueio emergencial ou reporte de problemas: 0800-000-0000, 24 horas.                                                                                                                                  |");
printf("|                                                                                                                                                                                                                |");
printf("|   CLÁUSULA SÉTIMA - DO ACEITE                                                                                                                                                                                  |");
printf("|   Ao confirmar ""SIM"" na adesão, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.                                                                                                |");
printf("|                                                                                                                                                                                                                |");
printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                                |");
printf("|  Local e Data: ______, ___ de ________ de 2025                                                                                                                                                                 |");
printf("|                                                                                                                                                                                                                |");
printf("|  Assinatura do Cliente: _________________________                                                                                                                                                              |");
printf("|                                                                                                                                                                                                                |");
printf("| Carimbo e Assinatura do Banco: _________________                                                                                                                                                               |");
printf("|                                                                                                                                                                                                                |");
printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                      Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.                                                   |");
printf("|                                                                                                                                                                                                                |");
printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");

}

void plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(){

 printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |  000000-01             |  00000002305402030900055J607500 - 01    |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    FORMA DE PAGAMENTO                             |                DATA DO DOCUMENTO      |\n");
    printf("|                                                   |                                       |\n");
    printf("|    DEBITO AUTOMATICO!                             |                28/11/2025             |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 2.400,00                              |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|   O CODIGO DE BARRAS ABAIXO               |      DESCONTO\\ABATIMENTO                      |\n");
    printf("|   PODE SER UTILIZADO COMO COMPROVANTE     |      R$ 00,00                                 |\n");
    printf("|   DE PAGAMENTO,ACEITO EM QUALQUER         | _____________________________________________ |\n");
    printf("|   INSTITUIÇÃO DO BANCO OU PARCERIA                                                        |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    SACADO                                                                                 |\n");
    printf("|                                                                                           |\n");
    printf("|        ||   ||||||   |||||||||| || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        ||   ||||||   |||||||||| || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        ||   ||||||   |||||||||  || || |||||||   |||||||| ||||     ||| |||||||||||||||     |\n");
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846      054 0000000000000001     |\n");
    printf("____________________________________________________________________________________________\n");
}