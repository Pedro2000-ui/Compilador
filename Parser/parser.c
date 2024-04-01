#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //https://petbcc.ufscar.br/ctypefuncoes/
#include <string.h>

typedef enum
{
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_DIVIDE,
    TOKEN_MULTIPLY,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ASSIGNMENT,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenTipo;

typedef struct
{
    TokenTipo tipo;
    char *valor;
} Token;

void identificaToken(FILE *arquivo)
{
    Token token;
    int c = fgetc(arquivo);
    // Variável que será usada para construir a string após ler caractere por caractere
    char *string;

    string = (char *)malloc(2);
    string[0] = c;
    string[1] = '\0';

    // Inicia em 2, pois string já tem duas posições inicialmente (0 e 1)
    int i = 2;

    // Enquanto não chegar ao fim do arquivo (EOF) e não encontrar o símbolo que identifica o fim de um token (>) continuamos iterando e contruindo a string
    while ((c = fgetc(arquivo)) != EOF && c != '>')
    {
        /*
        / Caso encontre um "(" será um TOKEN_ID ou um TOKEN_NUMBER (garanto isso através do LEXER)
        / itero pelos caracteres que se encontram entre "(" e para guardar como valor do token (sendo ele o valor de uma variável ou de um número) ")"
        */ 
        if (c == '(')
        {
            c = fgetc(arquivo);
            char* stringValor;
            stringValor = (char *)malloc(2);
            stringValor[0] = c;
            stringValor[1] = '\0';
            int j = 2;
            
            while ((c = fgetc(arquivo)) != EOF && isalnum(c))
            {
                char *temp2 = (char *)realloc(stringValor, j + 1);
                if (temp2 == NULL)
                {
                    perror("Erro ao alocar memória\n");
                    exit(1);
                }
                stringValor = temp2;
                stringValor[j - 1] = c; 
                stringValor[j] = '\0';

                j++;
            }
            token.valor = stringValor;
            c = fgetc(arquivo);
            break;
        }

        char *temp = (char *)realloc(string, i + 1);
        if (temp == NULL)
        {
            perror("Erro ao alocar memória\n");
            exit(1);
        }
        string = temp;
        string[i - 1] = c;
        string[i] = '\0';
        i++;
    }

    if (strcmp(string, "TOKEN_ERROR") == 0)
    {
        printf("Erro de Sintaxe (simbolo nao aceito)\n");
        exit(1);
    }
    // token.tipo = string;
    printf("%s %s\n", string, token.valor); 
    free(string);
    free(token.valor);
}

int main()
{
    FILE *arquivo = fopen("Parser.txt", "r");

    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo de entrada/saída");
        return 1;
    }

    identificaToken(arquivo);

    fclose(arquivo);
    return 0;
}
