#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Identificações possíveis, atribuindo ao respectivo token (Tipo do Token)
typedef enum {
    TOKEN_ID, // Variáveis
    TOKEN_NUMBER, // Números
    TOKEN_PLUS, // Símbolo de Soma
    TOKEN_MINUS, // Símbolo de Subtração
    TOKEN_DIVIDE, // Símbolo de Divisão
    TOKEN_MULTIPLY, // Símbolo de Multiplicação
    TOKEN_LPAREN, // Abertura de Parentese
    TOKEN_RPAREN, // Fechamento de Parentese
    TOKEN_EOF, // Fim do Arquivo
    TOKEN_ERROR // Qualquer outra coisa que não se encaixe nas definições acima
} TokenTipo;

// Estrutura de um Token
typedef struct {
    TokenTipo tipo;
    char* valor;
} Token;

void * liberaToken(Token token) {
    if(token.valor != NULL) {
        free(token.valor);
    }
}


Token classificaToken(FILE* arquivo) {
    Token token;

    // Guarda na variável o código ASCII do caracter lido, ou EOF caso o arquivo tenha acabado
    int c = fgetc(arquivo);

    // Enquanto c tiver o código ASCII referente a espaços vazios, vamos pulando e lendo até aparecer um caracter diferente de vazio
    while (c == 32) {
        c = fgetc(arquivo);
    }

    if (c == EOF) {
        token.tipo = TOKEN_EOF;
        token.valor = NULL;
        return token;
    }
    
    // Verifica se é uma Letra
    if(isalpha(c)) {
        token.tipo = TOKEN_ID;
        // Aloco na memória 2 espaços (1 para o valor de C e outro para o terminador da string)
        token.valor = (char *) malloc(2);
        token.valor[0] = c;
        token.valor[1] = '\0';
        
        // Atualizo C, andando para o próximo caractere
        c = fgetc(arquivo);
        // Inicia em 2, pois token.valor já tem duas posições inicialmente (0 e 1)
        int i = 2;

        // Caso comece por uma letra, será uma variável (TOKEN_ID) e por regra pode vir seguida de mais letras ou números (exemplo: xx, x45, a2b)
        // Então faço a verificação sobre os próximos digitos (irá parar quando encontrar algo diferente de um número ou letra)
        while(c != EOF && isalnum(c)) {
            int aux;
            char *temp = (char *) realloc(token.valor, i + 1); // Realoca a memória dando espaço para o próximo caractere
            if (temp == NULL) {
                // Tratar erro de alocação
                printf("Erro ao alocar memória\n");
                liberaToken(token); // Libera a memória alocada anteriormente
                exit(1);
            }
            token.valor = temp;
            token.valor[--i] = c;
            token.valor[++i] = '\0';

            c = fgetc(arquivo); 
        }
        return token;
    }

    switch (c) {
        case 43:
            token.tipo = TOKEN_PLUS;
            break;
        case 45:
            token.tipo = TOKEN_MINUS;
            break;
        case 42:
            token.tipo = TOKEN_MULTIPLY;
            break;
        case 47:
            token.tipo = TOKEN_DIVIDE;
            break;
        case 40:
            token.tipo = TOKEN_LPAREN;
            break;
        case 41:
            token.tipo = TOKEN_RPAREN;
            break;
        default:
            token.tipo = TOKEN_ERROR;
            break;
    }
    token.valor = NULL;
    return token;
}

int main() {
    // Transcreve para a variável o que se encontra no arquivo de texto
    FILE* arquivo = fopen("Lexer.txt", "r");
    
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    Token token;
    
    do {
        token = classificaToken(arquivo);
        
        switch (token.tipo) {
            case TOKEN_ID:
                printf("Identificador: %s\n", token.valor);
                break;
            case TOKEN_NUMBER:
                printf("Numero: %s\n", token.valor);
                break;
            case TOKEN_PLUS:
                printf("Operador de adicao \n");
                break;
            case TOKEN_MINUS:
                printf("Operador de subtracao\n");
                break;
            case TOKEN_MULTIPLY:
                printf("Operador de multiplicacao\n");
                break;
            case TOKEN_DIVIDE:
                printf("Operador de divisao\n");
                break;
            case TOKEN_LPAREN:
                printf("Parentese esquerdo\n");
                break;
            case TOKEN_RPAREN:
                printf("Parentese direito \n");
                break;
            case TOKEN_EOF:
                printf("Fim do arquivo\n");
                break;
            case TOKEN_ERROR:
                printf("Erro de token\n");
                break;
        }
        liberaToken(token);
    } while (token.tipo != TOKEN_EOF && token.tipo != TOKEN_ERROR);

    fclose(arquivo);
    return 0;
}