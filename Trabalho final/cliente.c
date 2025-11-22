
#include "cliente.h"
#include "banco.h"
#include "unir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 100

typedef struct {
    char cpf[15];
    char senha[8];
    char agencia[50];
    double saldo;
    char pix[TAM];
} Usuario;

Usuario usuario = {"", "", "", 1500.00, ""};

int login(void) {
    char buf[64];

    // Ler e validar CPF
    while (1) {
        printf("Digite seu CPF: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        trimNewline(buf);
        if (validarCPF(buf)) {
            strncpy(usuario.cpf, buf, sizeof(usuario.cpf)-1);
            usuario.cpf[sizeof(usuario.cpf)-1] = '\0';
            break;
        }
        printf("CPF invalido. Tente novamente.\n");
    }

    // Ler e validar senha (6 digitos numericos)
    while (1) {
        printf("Digite sua senha (6 digitos numericos): ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        trimNewline(buf);
        if (senhaValidaNDigitos(buf, 6)) {
            strncpy(usuario.senha, buf, sizeof(usuario.senha)-1);
            usuario.senha[sizeof(usuario.senha)-1] = '\0';
            break;
        }
        printf("Senha invalida. Deve conter exatamente 6 digitos numericos.\n");
    }

    return 1;
}

int tela_principal() {
    
    strcpy(usuario.agencia, "001 - AGENCIA DO CENTRO");
    char cabecalho[80];
    strcpy(cabecalho, "AGENCIA:");
    strcat(cabecalho, usuario.agencia);
    
    printf("\n%s\t  SALDO: %.2f\t  CPF: %s\n\n", cabecalho, usuario.saldo, usuario.cpf);
    puts("\nEXTRATO\t DEPOSITO\t SAQUE\t TRANSFERENCIA\n\n");
    puts("\noutras opcões\n");
    
    
    return 0;
}


double deposito(double valor) {
    
    usuario.saldo += valor;
    
    printf("+ R$ %.2f\n", valor);
    
    return usuario.saldo;
}

void pix_aleatorio(){
    
    sprintf(usuario.pix, "%s@banco%s.com", usuario.agencia, usuario.cpf);
    
    printf("Nova chave PIX gerada: %s\n", usuario.pix);
    
}

int pix_cadastro(){

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
            scanf("%[^\n]", numero);
            printf("%s é sua nova chave agora\n", numero);
            strcpy(usuario.pix, numero);
            
        break;

        case 3:
        
            printf("Digite seu email: \n");
            scanf("%s\n", email);
            printf("%s é agora sua nova chave", email);
            strcpy(usuario.pix, email);
            
        break;

    default:
    
printf("Opção inválida!\n");

}

    return 0;
}

int pix_cadastrados(){

    if(strlen(usuario.pix) == 0) {
        
        printf("Nenhuma chave PIX cadastrada.\n");
    }
    
    else {
        
        printf("Chave PIX cadastrada: %s\n", usuario.pix);
    }

    return 0;
}

void qr_code(){
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

int extrato(){
    
    printf("CPF - %s\n", usuario.cpf);
    printf("Saldo atual - R$ %f\n", usuario.saldo);
    printf("Nenhuma compra foi efetuada ate agora");

return 0;
}

double saque(double valor) {
    int tentativas = 3;
    while (tentativas > 0) {
        
        if (valor <= usuario.saldo && valor > 0) {
            usuario.saldo -= valor;
            return usuario.saldo;
        }
        tentativas--;
    }
    return usuario.saldo;
}
int opcoes(char *escolha) {
    double valor; 
    char transferencia[20] = "";
    char pix[30] = "";
    
    if(strcmp(escolha, "EXTRATO") == 0) {
        extrato();
    }
    else if(strcmp(escolha, "DEPOSITO") == 0) {
        printf("Digite a forma de deposito: PIX\t  BOLETO\t  TED\t");
        scanf("%s", transferencia);
        
        if(strcmp(transferencia, "PIX") == 0){
        printf("Escolha uma opção: gerar chave aleatoria\t fazer o cadastro de uma nova chave\t ver chaves disponiveis\t gerar qr code de chave aleatoria\n");
        
        scanf(" %[^\n]s", pix);
        
        if(strcmp(pix, "gerar chave aleatoria") == 0){
        pix_aleatorio();
        }
        
        else if(strcmp(pix, "fazer o cadastro de uma nova chave") == 0){
        pix_cadastro();
        }
        
        else if(strcmp(pix, "ver chaves disponiveis") == 0){
        pix_cadastrados();
        }
        
        else if(strcmp(pix, "gerar qr code de chave aleatoria") == 0){
        qr_code();
        }
        }
        
        else if(strcmp(transferencia, "BOLETO") == 0){
        boleto();
        }
        
        else if(strcmp(transferencia, "TED") == 0){
        printf("Digite o valor a ser depositado: ");
        scanf("%lf", &valor);
        
        double novo_saldo = deposito(valor);
        printf("Operação concluída! Saldo final: R$ %.2f\n", novo_saldo);
        }
    }
    else if(strcmp(escolha, "SAQUE") == 0) {
        printf("Digite o valor a ser sacado: ");
        scanf("%lf", &valor);
    }
    else if(strcmp(escolha, "TRANSFERENCIA") == 0) {
        printf("Transferencia selecionada\n");
    }
    else if(strcmp(escolha, "OUTRAS OPÇÕES") == 0) {
        printf("Outras opcoes selecionadas\n");
    }
    else {
        printf("OPÇÃO INVÁLIDA!\n");
    }
    return 0;
}

// Funções que operam sobre o vetor de contas passado pelo main
void clienteCriarConta(Conta **contas, int *total, int *capacidade) {
    if (!contas || !total || !capacidade) return;
    // garantir capacidade mínima
    if (*capacidade < 1) *capacidade = 2;
    if (*contas == NULL) {
        *contas = (Conta *)malloc((size_t)(*capacidade) * sizeof(Conta));
        if (!*contas) { *capacidade = 0; return; }
    }
    if (*total + 1 > *capacidade) {
        int nova = *capacidade * 2;
        Conta *tmp = (Conta *)realloc(*contas, (size_t)nova * sizeof(Conta));
        if (!tmp) return;
        *contas = tmp;
        *capacidade = nova;
    }

    // preencher nova conta
    Conta *c = &(*contas)[*total];
    // gerar id sequencial (busca maior id)
    int maxid = 1000;
    for (int i = 0; i < *total; i++) if ((*contas)[i].id > maxid) maxid = (*contas)[i].id;
    c->id = maxid + 1;

    // Ler dados do usuário
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
    printf("Nome completo: ");
    if (fgets(c->nome, sizeof(c->nome), stdin) == NULL) return;
    c->nome[strcspn(c->nome, "\n")] = '\0';
    // Ler e validar CPF
    char buffer[64];
    while (1) {
        printf("CPF (somente numeros ou com pontuacao): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
        trimNewline(buffer);
        if (validarCPF(buffer)) {
            strncpy(c->cpf, buffer, sizeof(c->cpf)-1);
            c->cpf[sizeof(c->cpf)-1] = '\0';
            break;
        }
        printf("CPF invalido. Tente novamente.\n");
    }

    // Ler e validar senha: exatamente 6 digitos numericos com confirmacao
    while (1) {
        char senha1[16];
        char senha2[16];
        printf("Senha (6 digitos numericos): ");
        if (fgets(senha1, sizeof(senha1), stdin) == NULL) { printf("Erro de leitura da senha.\n"); continue; }
        trimNewline(senha1);
        printf("Confirmar senha: ");
        if (fgets(senha2, sizeof(senha2), stdin) == NULL) { printf("Erro na confirmacao da senha.\n"); continue; }
        trimNewline(senha2);
        if (!senhaValidaNDigitos(senha1, 6) || !senhaValidaNDigitos(senha2, 6)) {
            printf("Senha invalida: deve conter exatamente 6 digitos numericos.\n");
            continue;
        }
        if (strcmp(senha1, senha2) != 0) {
            printf("Senhas diferentes.\n");
            continue;
        }
        strncpy(c->senha, senha1, sizeof(c->senha)-1);
        c->senha[sizeof(c->senha)-1] = '\0';
        break;
    }
    c->saldo = 0.0f;
    c->flags = 0;
    (*total)++;
    printf("Conta criada com id %d\n", c->id);
}

void clienteMenu(Conta **contas, int *total, int *capacidade) {
    // para já manter compatibilidade com funções antigas, apenas um menu simples
    int opc = 0;
    printf("\nMenu Cliente\n");
    printf("1 - Criar conta\n");
    printf("2 - Login\n");
    printf("0 - Voltar\n");
    printf("Escolha: ");
    if (scanf("%d", &opc) != 1) { int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} return; }
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
    if (opc == 1) {
        clienteCriarConta(contas, total, capacidade);
    } else if (opc == 2) {
        if (login()) {
            tela_principal();
        } else {
            printf("Falha no login do cliente.\n");
        }
    }
}