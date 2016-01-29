#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RAW_INPUT_LENGTH 80
#define TOKEN_LENGTH 80
#define OPERAND_LENGTH 80
#define SIGNIFICANT_DIGITS 16

void scanTerm(char rawInput[]);
int tokenize(char rawInput[], int token[], double operand[]);
void clearCharArray(char array[], int length);
void clearIntArray(int array[], int length);
void clearDoubleArray(double array[], int length);
int scanFile(char rawInput[]);

int main(void)
{
    char rawInput[RAW_INPUT_LENGTH + 1] = {'\0'};
    int token[TOKEN_LENGTH] = {0};
    double operand[OPERAND_LENGTH] = {0};
    
    char c;
    int i = 0,
        tmp = 0;
    
    do
    {
        scanTerm(rawInput);
        printf("\n");
        
        if(strcmp(rawInput, "file") == 0)
        {
            if(scanFile(rawInput) != 0)
            {
                printf("Error, file could not be opened!\n");
                continue;
            }
        }
        
        tmp = tokenize(rawInput, token, operand);
        if(tmp != 0)
        {
            switch(tmp)
            {
            case 1: printf("Illegal Error!");
                break;
            case 2: printf("Error, please enter only valid characters (+, -, /, (, ), ., 0 - 9)!");
                break;
            case 3: printf("Error, please enter a shorter term!");
                break;
            default: printf("Illegal Error!");
                break;
            }
            
            printf("\n\n");
            continue;
        }
        
        // debug output
        printf("Token-Array:\n");
        for(i = 0; i < TOKEN_LENGTH; ++i)
        {
            printf("%d", token[i]);
            printf("|");
        }
        printf("\nOperand-Array:\n");
        for(i = 0; i < OPERAND_LENGTH; ++i)
        {
            printf("%g", operand[i]);
            printf("|");
        }
        printf("\n\n");
        
        printf("Do you want to calculate more terms? (y|n): ");
        while((c = getchar()) == '\n');
        printf("\n");
    } while(c != 'n');
    
    return 0;
}

void scanTerm(char rawInput[])
{
    clearCharArray(rawInput, RAW_INPUT_LENGTH);
    
    printf("Please enter Term: ");
    scanf("%s", rawInput);
}

int tokenize (char rawInput[], int token[], double operand[])
{
    int i = 0;
    char buffer[SIGNIFICANT_DIGITS + 1] = {'\0'};
    int bufferIndex = 0, tokenIndex = 0, operandIndex = 0;
    int tmp;
    
    clearDoubleArray(operand, OPERAND_LENGTH);
    clearIntArray(token, TOKEN_LENGTH);
    
    while (rawInput[i] != '\0')
    {
        if(i >= RAW_INPUT_LENGTH)
        {
            return 3;
        }

        if(rawInput[i] >= '0' && rawInput[i] <= '9') // 0 - 9
        {
            clearCharArray(buffer, SIGNIFICANT_DIGITS);
            bufferIndex = 0;
            
            while((rawInput[i] >= '0' && rawInput[i] <= '9') || rawInput[i] == '.') // 0 - 9, .
            {
                if((i + 1) >= RAW_INPUT_LENGTH || bufferIndex >= SIGNIFICANT_DIGITS)
                {
                    return 3;
                }

                buffer[bufferIndex++] = rawInput[i++];
            }

            token[tokenIndex++] = operandIndex;
            operand[operandIndex++] = atof(buffer);
        }
        
        else if (rawInput[i] >= '(' && rawInput[i] <= '/' && rawInput[i] != '.' && rawInput[i] != ',') // +, -, *, /, (, )
        {
            switch (rawInput[i])
            {
                case '+': tmp = -1;
                    break;
                case '-': tmp = -2;
                    break;
                case '*': tmp = -3;
                    break;
                case '/': tmp = -4;
                    break;
                case '(': tmp = -5;
                    break;
                case ')': tmp = -6;
                    break;
                default: return 1;
            }
            token[tokenIndex++] = tmp;
            ++i;
        }
        
        else
        {
            return 2;
        }
    }
    return 0;
}

void clearCharArray(char array[], int length)
{
    int i;
    
    for(i = 0; i < length; ++i)
    {
        array[i] = '\0';
    }
}

void clearDoubleArray(double array[], int length)
{
    int i;
    
    for(i = 0; i < length; ++i)
    {
        array[i] = 0;
    }
}

void clearIntArray(int array[], int length)
{
    int i;
    
    for(i = 0; i < length; ++i)
    {
        array[i] = 0;
    }
}

int scanFile(char rawInput[])
{
    char path[100];
    
    printf("Please enter path: ");
    scanf("%s", path);
    
    FILE* fp = fopen(path, "r");
    
    if(fp == NULL)
    {
        return 1;
    }
    
    clearCharArray(rawInput, RAW_INPUT_LENGTH);
    
    while(!feof(fp))
    {
        fscanf(fp,"%s", rawInput);
    }
    
    printf("%s", rawInput);
    
    fclose(fp);
    return 0;
}
