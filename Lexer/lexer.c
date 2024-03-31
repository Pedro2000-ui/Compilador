#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //https://petbcc.ufscar.br/ctypefuncoes/
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
    TOKEN_ASSIGNMENT, // Símbolo de atribuição
    TOKEN_SEMICOLON, // Ponto e vírgula
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

    // Enquanto c tiver o código ASCII referente a espaços vazios, tabulação, quebra de linha, etc vamos pulando e lendo até aparecer um caracter válido
    while (isspace(c)) {
        c = fgetc(arquivo);
    }
    
    if (c == EOF) {
        token.tipo = TOKEN_EOF;
        token.valor = NULL;
        return token;
    }
    
    // Verifica se é uma Letra (retorna 1 caso seja uma letra do alfabeto ou 0 caso não esteja no alfabeto.)
    if(isalpha(c)) {
        // Caso comece por uma letra, será uma variável (TOKEN_ID) e por regra pode vir seguida de mais letras ou números (exemplo: xx, x45, a2b)
        token.tipo = TOKEN_ID;
        // Aloco na memória 2 espaços (1 para o valor de C e outro para o terminador da string)
        token.valor = (char *) malloc(2);
        token.valor[0] = c;
        token.valor[1] = '\0';
        
        // Atualizo C, andando para o próximo caractere
        c = fgetc(arquivo);
        // Inicia em 2, pois token.valor já tem duas posições inicialmente (0 e 1)
        int i = 2;

        // Faço a verificação sobre os próximos digitos (irá parar quando encontrar algo diferente de um número ou letra)
        while(c != EOF && isalnum(c)) {
            char *temp = (char *) realloc(token.valor, i + 1); // Realoca a memória dando espaço para o próximo caractere
            if (temp == NULL) {
                // Tratar erro de alocação
                printf("Erro ao alocar memória\n");
                liberaToken(token); // Libera a memória alocada anteriormente
                token.tipo = TOKEN_ERROR;
                return token;
            }
            token.valor = temp;
            token.valor[--i] = c;
            token.valor[++i] = '\0';

            // Atualizo C, andando para o próximo caractere
            c = fgetc(arquivo); 

            // Atualizo a variável i
            i++;
        }
        // Se o caractere atual é um espaço, tabulação, quebra de linha ou ";", eu apenas retorno o token
        if(isspace(c) || c == 59) {
            // Move o ponteiro do arquivo de volta um caractere (faço isso, para que na próxima verificação ele inicie 1 caractere após o final da variável encontrada)
            fseek(arquivo, -1, SEEK_CUR);
            return token;
        }
        // Se for algo diferente disso, retorno um token inválido, já que não obedece a regra de composição de uma variável
        token.tipo = TOKEN_ERROR;
        return token;
    }

    // Verifico se é um número
    if(isdigit(c)) {
        token.tipo = TOKEN_NUMBER;
        token.valor = (char *) malloc(2);
        token.valor[0] = c;
        token.valor[1] = '\0';

        int i = 2;
        // Atualizo C, andando para o próximo caractere
        c = fgetc(arquivo);

        // um número só pode ser seguido de números
        while(c != EOF && isdigit(c)) {
            char* temp = realloc(token.valor, i+1);
            token.valor = temp;
            token.valor[--i] = c;
            token.valor[++i] = '\0';

            // Atualizo C, andando para o próximo caractere
            c = fgetc(arquivo); 

            // Atualizo a variável i
            i++;
        }

        // Se o caractere atual é um espaço, tabulação, quebra de linha ou ";", eu apenas retorno o token
        if(isspace(c) || c == 59) {
            // Move o ponteiro do arquivo de volta um caractere (faço isso, para que na próxima verificação ele inicie 1 caractere após o final do número encontrado)
            fseek(arquivo, -1, SEEK_CUR);
            return token;
        }
        // Se for algo diferente disso, retorno um token inválido, já que não obedece a regra de composição de um número
        token.tipo = TOKEN_ERROR;
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
        case 61:
            token.tipo = TOKEN_ASSIGNMENT;
            break;
        case 59:
            token.tipo = TOKEN_SEMICOLON;
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
    FILE* arquivoParser = fopen("../Parser/Parser.txt", "w");

    if (arquivo == NULL || arquivoParser == NULL) {
        perror("Erro ao abrir arquivo de entrada/saida");
        return 1;
    }

    Token token;
    
    do {
        token = classificaToken(arquivo);
        
        switch (token.tipo) {
            case TOKEN_ID:
                // Identificador de Variável
                fprintf(arquivoParser, "%s%s%s", "TOKEN_ID (", token.valor, ")\n");
                break;
            case TOKEN_NUMBER:
                // Identificador de Números
                fprintf(arquivoParser, "%s%s%s", "TOKEN_NUMBER (", token.valor, ")\n");
                break;
            case TOKEN_PLUS:
                // Operador de Soma
                fprintf(arquivoParser, "%s", "TOKEN_PLUS\n");
                break;
            case TOKEN_MINUS:
                // Operador de Subtração 
                fprintf(arquivoParser, "%s", "TOKEN_MINUS\n");
                break;
            case TOKEN_MULTIPLY:
                // Operador de Multiplicação
                fprintf(arquivoParser, "%s", "TOKEN_MULTIPLY\n");
                break;
            case TOKEN_DIVIDE:
                // Operador de divisão
                fprintf(arquivoParser, "%s", "TOKEN_DIVIDE\n");
                break;
            case TOKEN_LPAREN:
                // Parentese esquerdo
                fprintf(arquivoParser, "%s", "TOKEN_LPAREN\n");
                break;
            case TOKEN_RPAREN:
                // Parentese direito
                fprintf(arquivoParser, "%s", "TOKEN_RPAREN\n");
                break;
            case TOKEN_ASSIGNMENT:
                // Atribuicao
                fprintf(arquivoParser, "%s", "TOKEN_ASSIGNMENT\n");
                break;
            case TOKEN_SEMICOLON:
                // Fim do arquivo
                fprintf(arquivoParser, "%s", "TOKEN_SEMICOLON\n");
                break;
            case TOKEN_EOF:
                // Fim do arquivo
                fprintf(arquivoParser, "%s", "TOKEN_EOF");
                break;
            case TOKEN_ERROR:
                // Caractere não aceito pela linguagem
                // Fecho o arquivo anterior
                fclose(arquivoParser);
                // Como encontrou um caractere de erro, eu reabro o arquivo em modo de escrita, com isso o arquivo será "limpo" e ai eu só escrevo o TOKEN_ERROR para facilitar para meu parser
                arquivoParser = fopen("../Parser/Parser.txt", "w");
                fprintf(arquivoParser, "%s", "TOKEN_ERROR");
                break;
        }
        liberaToken(token);
    } while (token.tipo != TOKEN_EOF && token.tipo != TOKEN_ERROR);

    fclose(arquivo);
    fclose(arquivoParser);
    return 0;
}
