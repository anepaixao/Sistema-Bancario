#include "banco.h"

// Definição da variável global
Usuario usuario = {"123.456.789-00", "12345678", "001 - AGENCIA DO CENTRO", 1500.00, "", 0};

// Implementação das funções

int login(void) {
    char cpf_digitado[15];
    char senha_digitada[9];
    int tentativas = 3;

    while(tentativas > 0) {
        printf("Digite seu CPF: ");
        scanf(" %[^\n]", cpf_digitado);
        printf("Digite sua senha de 8 digitos: ");
        scanf(" %8[^\n]", senha_digitada);

        if(strcmp(cpf_digitado, usuario.cpf) == 0 &&
           strcmp(senha_digitada, usuario.senha) == 0) {
            printf("Login realizado com sucesso!\n");
            return 0;
        } else {
            tentativas--;
            if(tentativas > 0) {
                printf("CPF ou senha incorretos. Tentativas restantes: %d\n\n", tentativas);
            } else {
                printf("Número máximo de tentativas atingido. Sistema bloqueado.\n");
                return -1;
            }
        }
    }
    return -1;
}

int tela_principal(void) {
    char *agencia = (char *) malloc(TAM * sizeof(char));
    if(agencia == NULL) {
        puts("ERRO");
        system("PAUSE");
        exit(1);
    }

    strcpy(usuario.agencia, "001 - AGENCIA DO CENTRO");
    char cabecalho[80];
    strcpy(cabecalho, "AGENCIA:");
    strcat(cabecalho, usuario.agencia);

    printf("\n%s\t  SALDO: %.2f\t  CPF: %s\n\n", cabecalho, usuario.saldo, usuario.cpf);
    printf("\n1 - EXTRATO\t 2 - DEPOSITO\t 3 - SAQUE\t 4 - TRANSFERENCIA\n\n");
    printf("\n5 - OUTRAS OPCOES\n");

    free(agencia);
    return 0;
}

double deposito(double n, int interacoes){
    if(interacoes <= 0){
        return usuario.saldo;
    }
    else{
        usuario.saldo += n;
        printf("+ R$ %.2f\n", n);
        return deposito(n, interacoes - 1);
    }
}

void pix_aleatorio(void){
    sprintf(usuario.pix, "%s@banco%s.com", usuario.agencia, usuario.cpf);
    printf("Nova chave PIX gerada: %s\n", usuario.pix);
}

int pix_cadastro(void){
    int b;
    char numero[12];
    char email[50];

    printf("Escolha o que voce quer cadastrar: 1 - CPF\t 2 - Numero\t 3 - Email\t");
    scanf("%d", &b);

    switch(b){
        case 1:
            printf("%s agora é sua nova chave\n", usuario.cpf);
            strcpy(usuario.pix, usuario.cpf);
            break;
        case 2:
            printf("Digite seu numero: \n");
            scanf(" %[^\n]", numero);
            printf("%s é sua nova chave agora\n", numero);
            strcpy(usuario.pix, numero);
            break;
        case 3:
            printf("Digite seu email: \n");
            scanf("%s", email);
            printf("%s é agora sua nova chave", email);
            strcpy(usuario.pix, email);
            break;
        default:
            printf("Opção inválida!\n");
    }
    return 0;
}

int pix_cadastrados(void){
    if(strlen(usuario.pix) == 0) {
        printf("Nenhuma chave PIX cadastrada.\n");
    }
    else {
        printf("Chave PIX cadastrada: %s\n", usuario.pix);
    }
    return 0;
}

void qr_code(void){
    printf("\n");
    printf("/////////////////////// QR CODE /////////////////////\n");
    printf("\n");
    printf("|||| |||||||  |||||| ||         ||||  ||||||||| ||||| ||\n");
    printf("||||                 ||         ||||                  ||\n");
    printf("||||                 ||         ||||                  ||\n");
    printf("||||                 ||         ||||                  ||\n");
    printf("||||                 ||         ||||                  ||\n");
    printf("|||| |||||||  |||||| ||         ||||                  ||\n");
    printf("                                ||||                  ||\n");
    printf("  |||| |||||||||| |||           ||||                  ||\n");
    printf("    ||     ||||||  |||          ||||  ||||||||| ||||| ||\n");
    printf(" ||||||      ||||||||||||||             |||||||||||  || \n");
    printf("    |||||||||||  ||||||||||||  ||||||           ||||||||\n");
    printf("    ||||                       ||||||                   \n");
    printf("    ||||                       ||||||  |||||||||||  |||||\n");
    printf("    ||||                       ||||||  |||||           ||\n");
    printf("    ||||                       ||||||  |||||           ||\n");
    printf("    ||||                       ||||||  |||||           ||\n");
    printf("    ||||                       ||||||  |||||           ||\n");
    printf("    ||||                       ||||||  |||||| |||||||| ||\n");
    printf("    ||||                       ||||||                    \n");
    printf("    |||||   ||||||||||||   ||||||||||           ||||||   \n");
    printf("\n");
    printf("Escaneie com a camera do celular ou o app do banco\n\n");
}

void boleto(){
    printf("\n");
    printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |               |  00000002305402030900055J607500 - 01             |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    LOCAL DE PAGAMENTO                     |                VENCIMENTO                     |\n");
    printf("|                                           |                                               |\n");
    printf("|    PREFERENCIALMENTE BANCO DO CENTRO      |                10/10/2025                     |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 10.000                                |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|                                           |      DESCONTO\\ABATIMENTO                     |\n");
    printf("|                                           |      R$ 00,00                                 |\n");
    printf("|                                           | _____________________________________________ |\n");
    printf("|                                                                                           |\n");
    printf("|                                                                                           |n");
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
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846     054 0000000000000001    |\n");
    printf("____________________________________________________________________________________________\n");
}

int extrato(){
    printf("CPF - %s\n", usuario.cpf);
    printf("Saldo atual - R$ %.2f\n", usuario.saldo);
    printf("Nenhuma compra foi efetuada ate agora");
    return 0;
}

double saque(double valor, int max_tentativas) {
    if(max_tentativas <= 0) {
        printf("Acabaram as tentativas!\n");
        return usuario.saldo;
    }

    if(valor <= 0) {
        printf("Valor deve ser positivo!\n");
        return usuario.saldo;
    }

    if(valor > usuario.saldo) {
        printf("Saldo insuficiente! Digite outro valor (%d tentativas): ", max_tentativas - 1);
        scanf("%lf", &valor);
        return saque(valor, max_tentativas - 1);
    }

    usuario.saldo -= valor;
    printf("Sacado: R$ %.2f | Saldo: R$ %.2f\n", valor, usuario.saldo);
    return usuario.saldo;
}

void transferencia_bancaria() {
    double valor;
    char tipo[20];
    char chave_destino[100];
    double taxa = 0.0;

    printf("\n=== TRANSFERÊNCIA BANCÁRIA ===\n");
    printf("Saldo disponível: R$ %.2f\n", usuario.saldo);

    printf("Tipos de transferência:\n");
    printf("1 - PIX\n");
    printf("2 - Transferência entre contas\n");
    printf("Escolha o tipo: ");
    scanf(" %19[^\n]", tipo);

    printf("Digite o valor: R$ ");
    scanf("%lf", &valor);

    if(valor <= 0) {
        printf("Valor deve ser positivo!\n");
        return;
    }

    if(valor > usuario.saldo) {
        printf("Saldo insuficiente! Saldo atual: R$ %.2f\n", usuario.saldo);
        return;
    }

    if(strcmp(tipo, "1") == 0 || strcmp(tipo, "PIX") == 0) {
        printf("Digite a chave PIX: ");
        scanf(" %99[^\n]", chave_destino);

        usuario.saldo -= valor;
        printf("\nTransferência PIX realizada!\n");
        printf("Valor: R$ %.2f\n", valor);
        printf("Chave PIX: %s\n", chave_destino);
        printf("Saldo atual: R$ %.2f\n", usuario.saldo);

    } else if(strcmp(tipo, "2") == 0 || strcmp(tipo, "TRANSFERENCIA") == 0) {
        char agencia[20], conta[20];

        printf("Digite a agência: ");
        scanf(" %19[^\n]", agencia);
        printf("Digite a conta: ");
        scanf(" %19[^\n]", conta);

        usuario.saldo -= valor;
        printf("\nTransferência realizada!\n");
        printf("Valor: R$ %.2f\n", valor);
        printf("Agência: %s\n", agencia);
        printf("Conta: %s\n", conta);
        printf("Saldo atual: R$ %.2f\n", usuario.saldo);

    } else {
        printf("Tipo de transferência inválido!\n");
        return;
    }

    if(valor > 1000.00) {
        taxa = 5.00;
        if(usuario.saldo >= taxa) {
            usuario.saldo -= taxa;
            printf("Taxa de transferência: R$ %.2f\n", taxa);
            printf("Saldo final: R$ %.2f\n", usuario.saldo);
        }
    }
}


void plano_protecaoCONTRATO(){

 printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
printf("|                                                                                                                                                                                                               |");
printf("| CLÁUSULA PRIMEIRA - DO OBJETO                                                                                                                                                                                 |");
printf("| O presente termo tem por objeto a contratação, pelo CLIENTE, do serviço de Proteção PIX oferecido pelo BANCO, que compreende um conjunto de medidas de segurança para transações via sistema PIX.             |");
printf("|                                                                                                                                                                                                               |");
printf("|CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO                                                                                                                                                              |");
printf("|O serviço de Proteção PIX inclui:                                                                                                                                                                              |");
printf("|a) Limite horário para transações: operações bloqueadas das 22h às 6h                                                                                                                                          |");
printf("|b) Confirmação adicional para valores superiores a R$ 1.000,00                                                                                                                                                 |");
printf("|c) Monitoramento de transações atípicas                                                                                                                                                                        |");
printf("|d) Seguro contra fraudes comprovadas até o limite de R$ 10.000,00                                                                                                                                              |");
printf("|e) Notificação imediata via SMS para todas as transações                                                                                                                                                       |");
printf("|                                                                                                                                                                                                               |");
printf("|CLÁUSULA TERCEIRA - DO PRAZO                                                                                                                                                                                   |");
printf("|O serviço terá vigência por prazo indeterminado, podendo ser cancelado a qualquer tempo pelo CLIENTE, sem ônus.                                                                                                |");
printf("|                                                                                                                                                                                                               |");
printf("|CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE                                                                                                                                                                    |");
printf("|O CLIENTE se compromete a:                                                                                                                                                                                     |");
printf("|a) Manter sigilo de suas credenciais de acesso                                                                                                                                                                 |");
printf("|b) Notificar o BANCO imediatamente em caso de perda ou roubo do dispositivo                                                                                                                                    |");
printf("|c) Verificar regularmente o extrato de movimentações                                                                                                                                                           |");
printf("|d) Utilizar senhas complexas e atualizá-las periodicamente                                                                                                                                                     |");
printf("|                                                                                                                                                                                                               |");
printf("|CLÁUSULA QUINTA - DA RESPONSABILIDADE                                                                                                                                                                          |");
printf("|O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas, excluídas as situações de negligência do CLIENTE.                                                        |");
printf("|                                                                                                                                                                                                               |");
printf("|CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO                                                                                                                                                                       |");
printf("|Para acionamento do serviço em caso de suspeita de fraude: 0800-000-0000, 24 horas.                                                                                                                            |");
printf("|                                                                                                                                                                                                               |");
printf("|CLÁUSULA SÉTIMA - DO ACEITE                                                                                                                                                                                    |");
printf("|Ao confirmar ""SIM"" na adesão, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.                                                                                                |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                               |");
printf("|Local e Data: ______, ___ de ________ de 2025                                                                                                                                                                  |");
printf("|                                                                                                                                                                                                               |");
printf("|Assinatura do Cliente: _________________________                                                                                                                                                               |");
printf("|                                                                                                                                                                                                               |");
printf("|Carimbo e Assinatura do Banco: _________________                                                                                                                                                               |");
printf("|                                                                                                                                                                                                               |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                      Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.                                                  |");
printf("|                                                                                                                                                                                                               |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("\n");

}


void plano_protecaoCOMPROVANTE(){

 printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |  000000-01             |  00000002305402030900055J607500 - 01    |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    FORMA DE PAGAMENTO                     |                DATA DO DOCUMENTO              |\n");
    printf("|                                           |                                               |\n");
    printf("|    DEBITO AUTOMATICO!                     |                28/11/2025                     |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 1.200,00                              |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|   O CODIGO DE BARRAS ABAIXO               |      DESCONTO\\ABATIMENTO                     |\n");
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
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846     054 0000000000000001    |\n");
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
printf("|                      Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.                                                        |");
printf("|                                                                                                                                                                                                                     |");
printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");                                                                                                                                                                                                        printf("|");
puts("\n");

}


void plano_Limitehorarioparatransacoescomprovante(){

 printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |  000000-01             |  00000002305402030900055J607500 - 01    |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    FORMA DE PAGAMENTO                     |                DATA DO DOCUMENTO              |\n");
    printf("|                                           |                                               |\n");
    printf("|    DEBITO AUTOMATICO!                     |                28/11/2025                     |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 2.400,00                              |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|   O CODIGO DE BARRAS ABAIXO               |      DESCONTO\\ABATIMENTO                     |\n");
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
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846     054 0000000000000001    |\n");
    printf("____________________________________________________________________________________________\n");
}

void plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO(){

printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                                 |");
printf("|CLÁUSULA PRIMEIRA - DO OBJETO                                                                                                                                                                                    |");
printf("|O presente termo tem por objeto a contratação, pelo CLIENTE, do serviço de Confirmação por Senha para Valores Altos oferecido pelo BANCO.                                                                        |");
printf("|                                                                                                                                                                                                                 |");
printf("|CLÁUSULA SEGUNDA - DAS CARACTERÍSTICAS DO SERVIÇO                                                                                                                                                                |");
printf("|O serviço de Confirmação por Senha para Valores Altos inclui:                                                                                                                                                    |");
printf("|a) Confirmação obrigatória para transações PIX superiores a R$ 1.000,00                                                                                                                                          |");
printf("|b) Autenticação adicional para transferências TED/DOC acima de R$ 2.000,00                                                                                                                                       |");
printf("|c) Verificação por senha única para compras com cartão acima de R$ 500,00                                                                                                                                        |");
printf("|d) Dupla validação para pagamentos de contas superiores a R$ 800,00                                                                                                                                              |");
printf("|e) Notificação imediata via aplicativo para todas as transações de alto valor                                                                                                                                    |");
printf("|                                                                                                                                                                                                                 |");
printf("|CLÁUSULA TERCEIRA - DO PRAZO                                                                                                                                                                                     |");
printf("|O serviço terá vigência por prazo indeterminado, podendo ser cancelado a qualquer tempo pelo CLIENTE, sem ônus.                                                                                                  |");
printf("|                                                                                                                                                                                                                 |");
printf("|CLÁUSULA QUARTA - DAS OBRIGAÇÕES DO CLIENTE                                                                                                                                                                      |");
printf("|O CLIENTE se compromete a:                                                                                                                                                                                       |");
printf("|a) Manter sigilo absoluto de suas senhas de confirmação                                                                                                                                                          |");
printf("|b) Não compartilhar códigos de verificação com terceiros                                                                                                                                                         |");
printf("|c) Notificar o BANCO imediatamente em caso de solicitação não autorizada                                                                                                                                         |");
printf("|d) Utilizar senhas distintas das utilizadas para acesso habitual                                                                                                                                                 |");
printf("|                                                                                                                                                                                                                 |");
printf("|CLÁUSULA QUINTA - DA RESPONSABILIDADE                                                                                                                                                                            |");
printf("|O BANCO se responsabiliza por indenizar prejuízos decorrentes de falhas comprovadas em seus sistemas de confirmação, excluídas as situações de negligência do CLIENTE.                                           |");
printf("|                                                                                                                                                                                                                 |");
printf("|CLÁUSULA SEXTA - DO CANAL DE ATENDIMENTO                                                                                                                                                                         |");
printf("|Para desbloqueio emergencial ou reporte de problemas: 0800-000-0000, 24 horas.                                                                                                                                   |");
printf("|                                                                                                                                                                                                                 |");
printf("|   CLÁUSULA SÉTIMA - DO ACEITE                                                                                                                                                                                   |");
printf("|   Ao confirmar ""SIM"" na adesão, o CLIENTE declara ter lido e concordado com todos os termos aqui estabelecidos.                                                                                               |");
printf("|                                                                                                                                                                                                                 |");
printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                                                                                                                                                                                                                 |");
printf("|  Local e Data: ______, ___ de ________ de 2025                                                                                                                                                                  |");
printf("|                                                                                                                                                                                                                 |");
printf("|  Assinatura do Cliente: _________________________                                                                                                                                                               |");
printf("|                                                                                                                                                                                                                 |");
printf("| Carimbo e Assinatura do Banco: _________________                                                                                                                                                                |");
printf("|                                                                                                                                                                                                                 |");
printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");
printf("|                     Este termo integra o contrato de prestação de serviços bancários, regendo-se pela Lei nº 13.709/2018 (LGPD) e demais normas aplicáveis.                                                     |");
printf("|                                                                                                                                                                                                                 |");
printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|");

}

void plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE(){

 printf("____________________________________________________________________________________________\n");
    printf("|    AGENCIA  DO CENTRO  |  000000-01             |  00000002305402030900055J607500 - 01    |\n");
    printf("_________________________|_______________|__________________________________________________|\n");
    printf("|    FORMA DE PAGAMENTO                     |                DATA DO DOCUMENTO              |\n");
    printf("|                                           |                                               |\n");
    printf("|    DEBITO AUTOMATICO!                     |                28/11/2025                     |\n");
    printf("____________________________________________|_______________________________________________|\n");
    printf("|    CEDENTE                                |      NOSSO NUMERO                             |\n");
    printf("|                                           |      +557399999 - 9999                        |\n");
    printf("|    USUARIO SILVA SANTOS                   |_______________________________________________|\n");
    printf("|                                           |      VALOR DO DOCUMENTO                       |\n");
    printf("|___________________________________________|      R$ 2.400,00                              |\n");
    printf("|    INSTRUÇÕES                             |_______________________________________________|\n");
    printf("|   O CODIGO DE BARRAS ABAIXO               |      DESCONTO\\ABATIMENTO                     |\n");
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
    printf("|        5A   ÇJD54471R EVBUHUHHH 85 74 RYHE2GU   RNKJNENN 7846     054 0000000000000001    |\n");
    printf("____________________________________________________________________________________________\n");
}

void outras_opcoes() {
    char escolha[10];
    int opcao_principal = 0;

    printf("\n=== OUTRAS OPÇÕES ===\n");
    printf("1 - Planos de Segurança\n");
    printf("2 - Configurações do Sistema\n");
    printf("3 - Ferramentas Avançadas\n");
    printf("4 - Validações e Verificações\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao_principal);

    switch(opcao_principal) {
        case 1: {

            char escolha_seguranca[10];
            int plano = 0;

            printf("Deseja ver os planos de segurança pix e cartão que nosso banco oferece? (sim/nao): ");
            scanf(" %9[^\n]", escolha_seguranca);

            if(strcmp(escolha_seguranca, "sim") == 0){
                printf("\n=== PLANOS DE SEGURANÇA PIX ===\n");
                printf("1. Proteção contra transferências não autorizadas\n");
                printf("2. Limite horário para transações\n");
                printf("3. Confirmação por senha para valores altos\n");
                scanf("%d", &plano);

                switch(plano) {
                    case 1:
                        plano_protecaoCONTRATO();
                        plano_protecaoCOMPROVANTE();
                        break;
                    case 2:
                        plano_Limitehorarioparatransacoes();
                        plano_Limitehorarioparatransacoescomprovante();
                        break;
                    case 3:
                        plano_ConfirmacaoporsenhaparavaloresaltosCONTRATO();
                        plano_ConfirmacaoporsenhaparavaloresaltosCOMPROVANTE();
                        break;
                    default:
                        printf("OPÇÃO INVÁLIDA!\n");
                }
            } else {
                printf("Obrigado por usar nossos serviços!\n");
            }
            break;
        }

        case 2: {

            void ativar_configuracao(unsigned char *config, unsigned char flag) {
                *config |= flag;
                printf("Configuração ativada: 0x%02X\n", flag);
            }

            void desativar_configuracao(unsigned char *config, unsigned char flag) {
                *config &= ~flag;
                printf("Configuração desativada: 0x%02X\n", flag);
            }

            int verificar_configuracao(unsigned char config, unsigned char flag) {
                return (config & flag) != 0;
            }

            void exibir_configuracoes_recursivo(unsigned char config, int bit) {
                if (bit < 0) return;

                exibir_configuracoes_recursivo(config, bit - 1);

                const char *nomes[] = {
                    "Notificações Ativas",
                    "Transações Noturnas",
                    "Confirmação Valores Altos",
                    "Segurança Reforçada"
                };

                if (bit < 4) {
                    printf("Bit %d (%s): %s\n", bit, nomes[bit],
                           verificar_configuracao(config, 1 << bit) ? "ATIVO" : "INATIVO");
                }
            }

            printf("\n=== CONFIGURAÇÕES DO SISTEMA ===\n");
            printf("Configurações atuais: 0x%02X\n", usuario.configuracoes);
            exibir_configuracoes_recursivo(usuario.configuracoes, 3);

            printf("\nOpções:\n");
            printf("1 - Ativar notificações\n");
            printf("2 - Desativar notificações\n");
            printf("3 - Ativar transações noturnas\n");
            printf("4 - Desativar transações noturnas\n");
            printf("5 - Ativar confirmação para valores altos\n");
            printf("6 - Desativar confirmação para valores altos\n");
            printf("Escolha: ");

            int opcao_config;
            scanf("%d", &opcao_config);

            switch(opcao_config) {
                case 1: ativar_configuracao(&usuario.configuracoes, NOTIFICACOES_ATIVAS); break;
                case 2: desativar_configuracao(&usuario.configuracoes, NOTIFICACOES_ATIVAS); break;
                case 3: ativar_configuracao(&usuario.configuracoes, TRANSACOES_NOTURNAS); break;
                case 4: desativar_configuracao(&usuario.configuracoes, TRANSACOES_NOTURNAS); break;
                case 5: ativar_configuracao(&usuario.configuracoes, CONFIRMACAO_VALORES); break;
                case 6: desativar_configuracao(&usuario.configuracoes, CONFIRMACAO_VALORES); break;
                default: printf("Opção inválida!\n");
            }
            break;
        }

        case 3: {

            void notificar_sucesso(const char* mensagem) {
                printf(" NOTIFICAÇÃO: %s\n", mensagem);
            }

            void notificar_erro(const char* mensagem) {
                printf("== ERRO: %s\n", mensagem);
            }

            void notificar_alerta(const char* mensagem) {
                printf("==  ALERTA: %s\n", mensagem);
            }

            void executar_com_callback(FuncaoCallback callback, const char* mensagem) {
                callback(mensagem);
            }

            void processar_transacoes_array(double *transacoes, int tamanho, OperacaoFinanceira operacao) {
                printf("Processando %d transações:\n", tamanho);
                for (int i = 0; i < tamanho; i++) {
                    double resultado = operacao(transacoes[i], 1);
                    printf("Transação %d: R$ %.2f -> Saldo: R$ %.2f\n", i + 1, transacoes[i], resultado);
                }
            }

            void exibir_historico_recursivo(double *historico, int index, int tamanho) {
                if (index >= tamanho) return;

                exibir_historico_recursivo(historico, index + 1, tamanho);

                if (historico[index] != 0) {
                    printf("Histórico[%d]: R$ %+.2f\n", index, historico[index]);
                }
            }


            printf("\n=== FERRAMENTAS AVANÇADAS ===\n");
            printf("1 - Testar Sistema de Notificações\n");
            printf("2 - Processar Múltiplas Transações\n");
            printf("3 - Exibir Histórico Recursivo\n");
            printf("Escolha: ");

            int opcao_ferramentas;
            scanf("%d", &opcao_ferramentas);

            switch(opcao_ferramentas) {
                case 1:
                    printf("\n--- SISTEMA DE NOTIFICAÇÕES ---\n");
                    executar_com_callback(notificar_sucesso, "Teste de notificação de sucesso");
                    executar_com_callback(notificar_erro, "Teste de notificação de erro");
                    executar_com_callback(notificar_alerta, "Teste de notificação de alerta");
                    break;

                case 2:
                    printf("\n--- PROCESSAMENTO DE TRANSAÇÕES ---\n");
                    double transacoes[] = {100.0, 200.0, -50.0, 300.0};
                    processar_transacoes_array(transacoes, 4, deposito);
                    break;

                case 3:
                    printf("\n--- HISTÓRICO RECURSIVO ---\n");
                    double historico[6] = {100.0, -50.0, 200.0, -30.0, 150.0, 0.0};
                    exibir_historico_recursivo(historico, 0, 6);
                    break;

                default:
                    printf("Opção inválida!\n");
            }
            break;
        }

        case 4: {

            int validar_cpf(const char* cpf) {
                int digitos = 0;
                for (int i = 0; cpf[i]; i++) {
                    if (isdigit(cpf[i])) digitos++;
                }
                return digitos == 11;
            }

            int validar_senha(const char* senha) {
                int length = strlen(senha);
                return length >= 8 && length <= 20;
            }

            int validar_chave_pix(const char* pix) {
                return strlen(pix) > 0;
            }

            int executar_validacao(FuncaoValidacao validacao, const char* dado) {
                return validacao(dado);
            }

            char* formatar_cpf_ponteiro(const char* cpf, char* buffer) {
                char *ptr = buffer;
                const char *src = cpf;

                while (*src) {
                    if (isdigit(*src)) {
                        *ptr++ = *src;
                    }
                    src++;
                }
                *ptr = '\0';
                return buffer;
            }

            printf("\n=== VALIDAÇÕES E VERIFICAÇÕES ===\n");
            printf("1 - Validar CPF\n");
            printf("2 - Validar Senha\n");
            printf("3 - Validar Chave PIX\n");
            printf("4 - Formatar CPF\n");
            printf("Escolha: ");

            int opcao_validacao;
            scanf("%d", &opcao_validacao);

            switch(opcao_validacao) {
                case 1:
                    printf("Validação do CPF '%s': %s\n",
                           usuario.cpf,
                           executar_validacao(validar_cpf, usuario.cpf) ? "VÁLIDO" : "INVÁLIDO");
                    break;

                case 2:
                    printf("Validação da senha: %s\n",
                           executar_validacao(validar_senha, usuario.senha) ? "VÁLIDA" : "INVÁLIDA");
                    break;

                case 3:
                    printf("Validação da chave PIX '%s': %s\n",
                           usuario.pix,
                           executar_validacao(validar_chave_pix, usuario.pix) ? "VÁLIDA" : "INVÁLIDA");
                    break;

                case 4:
                    printf("Formatação de CPF:\n");
                    char cpf_formatado[15];
                    formatar_cpf_ponteiro(usuario.cpf, cpf_formatado);
                    printf("CPF original: %s\n", usuario.cpf);
                    printf("CPF formatado: %s\n", cpf_formatado);
                    break;

                default:
                    printf("Opção inválida!\n");
            }
            break;
        }

        default:
            printf("Opção inválida!\n");
    }
}

int opcoes(char *escolha) {
    double valor;
    int opcao_transferencia = 0;
    char pix[30] = "";

    int opcao;
    if(sscanf(escolha, "%d", &opcao) == 1) {

        switch(opcao) {
            case 1:
                extrato();
                break;
            case 2:
                printf("Digite a forma de deposito: 1 - PIX\t  2 - BOLETO\t  3 - TED\t");
                scanf("%d", &opcao_transferencia);

                if(opcao_transferencia == 1){
                    printf("Escolha uma opção:\n");
                    printf("1 - Gerar chave aleatoria\n");
                    printf("2 - Fazer cadastro de nova chave\n");
                    printf("3 - Ver chaves disponiveis\n");
                    printf("4 - Gerar QR code\n");
                    printf("Opção: ");

                    int opcao_pix;
                    scanf("%d", &opcao_pix);

                    switch(opcao_pix) {
                        case 1:
                            pix_aleatorio();
                            break;
                        case 2:
                            pix_cadastro();
                            break;
                        case 3:
                            pix_cadastrados();
                            break;
                        case 4:
                            qr_code();
                            break;
                        default:
                            printf("Opção inválida!\n");
                    }
                }
                else if(opcao_transferencia == 2){
                    boleto();
                }
                else if(opcao_transferencia == 3){
                    printf("Digite o valor a ser depositado: ");
                    scanf("%lf", &valor);
                    double novo_saldo = deposito(valor, 1);
                    printf("Operação concluída! Saldo final: R$ %.2f\n", novo_saldo);
                }
                else {
                    printf("Opção de depósito inválida!\n");
                }
                break;
            case 3:
                printf("Digite o valor a ser sacado: ");
                scanf("%lf", &valor);
                saque(valor, 3);
                break;
            case 4:
                transferencia_bancaria();
                break;
            case 5:
                outras_opcoes();
                break;
            default:
                printf("OPÇÃO INVÁLIDA!\n");
        }
    } else {
        if(strcmp(escolha, "EXTRATO") == 0) {
            extrato();
        }
    }
    return 0;
}
