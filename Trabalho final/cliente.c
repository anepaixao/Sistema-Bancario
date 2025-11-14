#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

/* Validação de CPF: remove caracteres não numéricos, checa tamanho 11,
   descarta sequências repetidas e verifica dígitos conforme algoritmo oficial */
static int validarCPF(const char *entrada) {
    char dig[12];
    int j = 0;
    for (int i = 0; entrada[i] != '\0' && j < 11; i++) {
        if (entrada[i] >= '0' && entrada[i] <= '9') {
            dig[j++] = entrada[i];
        }
    }
    dig[j] = '\0';
    if (j != 11) return 0;
    // Verifica se todos os dígitos são iguais
    int iguais = 1;
    for (int i = 1; i < 11; i++) {
        if (dig[i] != dig[0]) { iguais = 0; break; }
    }
    if (iguais) return 0;
    // Calcula primeiro dígito verificador
    int soma = 0;
    for (int i = 0; i < 9; i++) soma += (dig[i]-'0') * (10 - i);
    int dv1 = (soma * 10) % 11; if (dv1 == 10) dv1 = 0;
    if (dv1 != (dig[9]-'0')) return 0;
    // Segundo dígito
    soma = 0;
    for (int i = 0; i < 10; i++) soma += (dig[i]-'0') * (11 - i);
    int dv2 = (soma * 10) % 11; if (dv2 == 10) dv2 = 0;
    if (dv2 != (dig[10]-'0')) return 0;
    return 1;
}

static int senhaValida6Digitos(const char *senha) {
    if (strlen(senha) != 6) return 0;
    for (int i = 0; senha[i] != '\0'; i++) {
        if (senha[i] < '0' || senha[i] > '9') return 0;
    }
    return 1;
}


Usuario* criar_usuario(void) {
    Usuario *novo = (Usuario*) malloc(sizeof(Usuario));
    if (novo == NULL) {
        printf("Erro ao alocar memória para usuário!\n");
        exit(1);
    }
    
    strcpy(novo->cpf, "");
    strcpy(novo->senha, "");
    strcpy(novo->agencia, "001 - AGENCIA DO CENTRO");
    novo->saldo = 1500.00;
    strcpy(novo->pix, "");
    novo->prox = NULL;
    
    return novo;
}


int login(Usuario *usuario) {
    static int tentativas = 3;
    
    if (tentativas <= 0) {
        printf("Número máximo de tentativas excedido!\n");
        return -1;
    }
    
    printf("Digite seu CPF (somente números ou formatado): ");
    scanf(" %[^\n]", usuario->cpf);
    if (!validarCPF(usuario->cpf)) {
        printf("CPF inválido! Tentativas restantes: %d\n", --tentativas);
        return login(usuario);
    }

    printf("Digite sua senha de 6 dígitos (somente números): ");
    scanf(" %[^\n]", usuario->senha);
    if (!senhaValida6Digitos(usuario->senha)) {
        printf("Senha inválida (precisa ter exatamente 6 dígitos numéricos)! Tentativas restantes: %d\n", --tentativas);
        return login(usuario);
    }

    printf("Login realizado com sucesso!\n");
    return 0;
}

int tela_principal(Usuario *usuario) {
    char *cabecalho = (char*) malloc(200 * sizeof(char));
    if (cabecalho == NULL) {
        puts("ERRO ao alocar memória!");
        return -1;
    }
    
    snprintf(cabecalho, 200, "AGENCIA: %s\t  SALDO: %.2f\t  CPF: %s\n\n", 
             usuario->agencia, usuario->saldo, usuario->cpf);
    
    printf("\n%s", cabecalho);
    puts("EXTRATO\t DEPOSITO\t SAQUE\t TRANSFERENCIA\n\n");
    puts("outras opções\n");
    
    free(cabecalho);
    return 0;
}


double deposito_recursivo(Usuario *usuario, double valor, int interacoes) {
    if (interacoes <= 0) {
        return usuario->saldo;
    }
    else {
        usuario->saldo += valor;
        printf("+ R$ %.2f\n", valor); 
        return deposito_recursivo(usuario, valor, interacoes - 1);
    }
}


void pix_aleatorio(Usuario *usuario) {
    sprintf(usuario->pix, "%s@banco%s.com", usuario->agencia, usuario->cpf);
    printf("Nova chave PIX gerada: %s\n", usuario->pix);
}


ListaChavesPix* cadastrar_pix_recursivo(ListaChavesPix *lista, Usuario *usuario, int tentativas) {
    if (tentativas <= 0) {
        printf("Número máximo de tentativas excedido!\n");
        return lista;
    }
    
    int opcao;
    printf("Escolha o que quer cadastrar: 1 - CPF\t 2 - Número\t 3 - Email\t 4 - Voltar: ");
    scanf("%d", &opcao);
    
    if (opcao == 4) {
        return lista;
    }
    
    ListaChavesPix *nova_chave = (ListaChavesPix*) malloc(sizeof(ListaChavesPix));
    if (nova_chave == NULL) {
        printf("Erro ao alocar memória!\n");
        return lista;
    }
    
    switch(opcao) {
        case 1:
            strcpy(nova_chave->chave, usuario->cpf);
            printf("%s agora é sua nova chave\n", usuario->cpf);
            break;
            
        case 2: {
            char numero[12];
            printf("Digite seu número: ");
            scanf(" %[^\n]", numero);
            strcpy(nova_chave->chave, numero);
            printf("%s é sua nova chave agora\n", numero);
            break;
        }
            
        case 3: {
            char email[50];
            printf("Digite seu email: ");
            scanf(" %[^\n]", email);
            strcpy(nova_chave->chave, email);
            printf("%s é agora sua nova chave\n", email);
            break;
        }
            
        default:
            printf("Opção inválida! Tentativas restantes: %d\n", tentativas - 1);
            free(nova_chave);
            return cadastrar_pix_recursivo(lista, usuario, tentativas - 1);
    }
    
    nova_chave->prox = lista;
    return nova_chave;
}


void exibir_chaves_pix_recursivo(ListaChavesPix *lista, int contador) {
    if (lista == NULL) {
        if (contador == 1) {
            printf("Nenhuma chave PIX cadastrada.\n");
        }
        return;
    }
    
    printf("Chave PIX %d: %s\n", contador, lista->chave);
    exibir_chaves_pix_recursivo(lista->prox, contador + 1);
}


void liberar_chaves_pix(ListaChavesPix *lista) {
    if (lista == NULL) return;
    
    liberar_chaves_pix(lista->prox);
    free(lista);
}


Transacao* adicionar_transacao(Transacao *lista, const char *tipo, double valor, const char *descricao) {
    Transacao *nova = (Transacao*) malloc(sizeof(Transacao));
    if (nova == NULL) {
        printf("Erro ao alocar memória para transação!\n");
        return lista;
    }
    
    strcpy(nova->tipo, tipo);
    nova->valor = valor;
    strcpy(nova->descricao, descricao);
    nova->prox = lista;
    
    return nova;
}


void extrato_recursivo(Transacao *transacao, Usuario *usuario, int numero) {
    if (transacao == NULL) {
        if (numero == 1) {
            printf("CPF - %s\n", usuario->cpf);
            printf("Saldo atual - R$ %.2f\n", usuario->saldo);
            printf("Nenhuma transação encontrada.\n");
        }
        return;
    }
    
    if (numero == 1) {
        printf("CPF - %s\n", usuario->cpf);
        printf("Saldo atual - R$ %.2f\n", usuario->saldo);
        printf("\n--- EXTRATO BANCÁRIO ---\n");
    }
    
    printf("%d. %s: R$ %.2f - %s\n", numero, transacao->tipo, transacao->valor, transacao->descricao);
    extrato_recursivo(transacao->prox, usuario, numero + 1);
}


void liberar_transacoes(Transacao *lista) {
    if (lista == NULL) return;
    
    liberar_transacoes(lista->prox);
    free(lista);
}


double saque_recursivo(Usuario *usuario, double valor, int tentativas) {
    if (tentativas <= 0) {
        printf("Acabaram as tentativas!\n");
        return usuario->saldo;
    }
    
    if (valor <= 0) {
        printf("Valor deve ser positivo! Tentativas restantes: %d\n", tentativas - 1);
        printf("Digite outro valor: ");
        scanf("%lf", &valor);
        return saque_recursivo(usuario, valor, tentativas - 1);
    }
    
    if (valor > usuario->saldo) {
        printf("Saldo insuficiente! Saldo atual: R$ %.2f\n", usuario->saldo);
        printf("Digite outro valor (%d tentativas restantes): ", tentativas - 1);
        scanf("%lf", &valor);
        return saque_recursivo(usuario, valor, tentativas - 1);
    }
    
    usuario->saldo -= valor;
    printf("Sacado: R$ %.2f | Saldo: R$ %.2f\n", valor, usuario->saldo);
    return usuario->saldo;
}


void qr_code(void) {
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


void boleto(void) {
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

int opcoes(Usuario *usuario, Transacao **transacoes, ListaChavesPix **chaves, char *escolha) {
    double valor; 
    char transferencia[20] = "";
    char pix[50] = "";
    
    if(strcmp(escolha, "EXTRATO") == 0) {
        extrato_recursivo(*transacoes, usuario, 1);
    }
    else if(strcmp(escolha, "DEPOSITO") == 0) {
        printf("Digite a forma de depósito: PIX\t  BOLETO\t  TED\t");
        scanf("%s", transferencia);
        
        if(strcmp(transferencia, "PIX") == 0) {
            printf("Escolha uma opção: gerar chave aleatoria\t fazer cadastro\t ver chaves\t gerar qr code\n");
            scanf(" %[^\n]", pix);
            
            if(strcmp(pix, "gerar chave aleatoria") == 0) {
                pix_aleatorio(usuario);
            }
            else if(strcmp(pix, "fazer cadastro") == 0) {
                *chaves = cadastrar_pix_recursivo(*chaves, usuario, 3);
            }
            else if(strcmp(pix, "ver chaves") == 0) {
                exibir_chaves_pix_recursivo(*chaves, 1);
            }
            else if(strcmp(pix, "gerar qr code") == 0) {
                qr_code();
            }
        }
        else if(strcmp(transferencia, "BOLETO") == 0) {
            boleto();
        }
        else if(strcmp(transferencia, "TED") == 0) {
            printf("Digite o valor a ser depositado: ");
            scanf("%lf", &valor);
            
            double novo_saldo = deposito_recursivo(usuario, valor, 1);
            *transacoes = adicionar_transacao(*transacoes, "DEPOSITO", valor, "Transferência TED");
            printf("Operação concluída! Saldo final: R$ %.2f\n", novo_saldo);
        }
    }
    else if(strcmp(escolha, "SAQUE") == 0) {
        printf("Digite o valor a ser sacado: ");
        scanf("%lf", &valor);
        double novo_saldo = saque_recursivo(usuario, valor, 3);
        *transacoes = adicionar_transacao(*transacoes, "SAQUE", -valor, "Saque em caixa eletrônico");
        printf("Saque realizado! Saldo final: R$ %.2f\n", novo_saldo);
    }
    else if(strcmp(escolha, "TRANSFERENCIA") == 0) {
        printf("Transferencia selecionada\n");
    }
    else if(strcmp(escolha, "OUTRAS") == 0) {
        printf("Outras opcoes selecionadas\n");
    }
    else {
        printf("OPÇÃO INVÁLIDA!\n");
    }
    return 0;
}


int clienteExecutar(void) {
    char escolha[20];  
    
    
    Usuario *usuario = criar_usuario();
    
   
    Transacao *transacoes = NULL;
    ListaChavesPix *chaves_pix = NULL;
    
    if (login(usuario) != 0) {
        printf("Falha no login. Encerrando...\n");
        free(usuario);
        return 1;
    }
    
    tela_principal(usuario);
    
    printf("DIGITE A OPÇÃO QUE DESEJA: "); 
    scanf(" %[^\n]", escolha);
    
    opcoes(usuario, &transacoes, &chaves_pix, escolha);
    

    liberar_transacoes(transacoes);
    liberar_chaves_pix(chaves_pix);
    free(usuario);
    
    printf("Obrigada por usar o banco!\n");
    return 0;
}
