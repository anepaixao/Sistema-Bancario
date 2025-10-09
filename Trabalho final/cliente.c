

#define TAM 100

typedef struct {
    char cpf[15];
    char senha[8];
    char agencia[50];
    double saldo;
    char pix[TAM];
} Usuario;

Usuario usuario = {"", "", "", 1500.00, ""};

int login() {
    puts("Digite seu CPF: ");
    scanf(" %[^\n]", usuario.cpf);
    puts("Digite sua senha de 8 digitos: ");
    scanf(" %[^\n]", usuario.senha);
    return 0;
}

int tela_principal() {
    
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
    puts("\nEXTRATO\t DEPOSITO\t SAQUE\t TRANSFERENCIA\n\n");
    puts("\noutras opcões\n");
    
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
        
        double novo_saldo = deposito(valor, 1);
        printf("Operação concluída! Saldo final: R$ %.2f\n", novo_saldo);
        }
    }
    else if(strcmp(escolha, "SAQUE") == 0) {
        printf("Digite o valor a ser sacado: ");
        scanf("%lf", &valor);
        saque(valor, 1);
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
