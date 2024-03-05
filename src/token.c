#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


Token* createToken(TokenType type, double value)
{
    Token* newToken = (Token*)malloc(sizeof(Token));

    if (newToken == NULL)
    {
        fprintf(stderr, "createToken: Memory allocation failed.");
        exit(1);
    }

    newToken->type = type;
    newToken->value = value;
    newToken->prev = NULL;
    newToken->next = NULL;

    return newToken;
}

void insertTokenEnd(Token** head, TokenType type, double value)
{
    Token* newToken = createToken(type, value);

    if (*head == NULL)
    {
        *head = newToken;
        return;
    }
    Token* current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newToken;
    newToken->prev = current;
}

void insertTokenStart(Token** head, TokenType type, double value)
{
    Token* newToken = createToken(type, value);

    (*head)->prev = newToken;
    newToken->next = *head;
    *head = newToken;
}

void insertTokenAfter(Token* prev, TokenType type, double value)
{
    Token* newToken = createToken(type, value);
    
    newToken->next = prev->next;
    newToken->prev = prev;
    prev->next->prev = newToken;
    prev->next = newToken;
}

void insertTokenBefore(Token* next, TokenType type, double value)
{
    Token* newToken = createToken(type, value);

    newToken->prev = next->prev;
    newToken->next = next;
    next->prev->next = newToken;
    next->prev = newToken;
}

void replaceThreeTokens(Token** head, Token** center, TokenType type, double value)
{
    Token* newToken = createToken(type, value);

    if ((*center)->prev->prev != NULL)
    {
        (*center)->prev->prev->next = newToken;
        newToken->prev = (*center)->prev->prev;
    }
    else
    {
        *head = newToken;
    }

    if ((*center)->next->next != NULL)
    {
        (*center)->next->next->prev = newToken;
        newToken->next = (*center)->next->next;
    }

    free(*center);
    free((*center)->next);
    free((*center)->prev);

    *center = newToken;
}

int getTokenCount(Token* head)
{
    int count = 0;
    while(head != NULL)
    {
        count++;
        head = head->next;
    }
    return count;
}



void verifyTokens(Token* head)
{
    Token* current = head;
    while(current->next != NULL)
    {
        if (current->type == NUMBER && current->next->type == NUMBER)
        {
            fprintf(stdout, "Syntax error, no operator here -> %g %g\n", current->value, current->next->value);
            exit(1);
        }

        if ((current->type == ADD || current->type == SUB || current->type == MUL || current->type == DIV || current->type == POW) && 
            (current->next->type == ADD || current->next->type == SUB || current->next->type == MUL || current->next->type == DIV || current->next->type == POW))
        {
            fprintf(stdout, "Syntax error, adjacent operators here -> ");
            printToken(current);
            fprintf(stdout, " ");
            printToken(current->next);
            fprintf(stdout, "\n");
            exit(1);
        }
        current = current->next;
    }
}

void addImplicitMultipication(Token* head)
{
    Token* current = head;
    while(current->next != NULL)
    {
        if ((current->type == NUMBER || current->type == CLOSE_PAR) && current->next->type == OPEN_PAR)
        {
            insertTokenAfter(current, MUL, 0);
        }

        if (current->type == CLOSE_PAR && current->next->type == NUMBER)
        {
            insertTokenAfter(current, MUL, 0);
        }

        current = current->next;
    }

}

void simplify(Token** head)
{
    Token* temp = *head;
    Token* startpos = NULL;
    while(temp != NULL)
    {
        if (temp->type == OPEN_PAR)
        {
            startpos = temp;
        }
        if ((temp->type == CLOSE_PAR && startpos != NULL) || (temp->next == NULL && startpos == NULL))
        {
            if (startpos == NULL) {startpos = *head;}
            parenthesisEval(head, startpos);
            removeExtraParenthesis(head);
            break;
        }
        temp = temp->next;
    }
    if (getTokenCount(*head) != 1)
    {
        simplify(head);
    }
}

void removeExtraParenthesis(Token** head)
{
    Token* temp = *head;

    while(temp->next != NULL)
    {
        if (temp->next->type == CLOSE_PAR && temp->prev != NULL && temp->prev->type == OPEN_PAR)
        {
            printTokens(head);
            replaceThreeTokens(head, &temp, temp->type, temp->value);
            printTokens(head);
        }
        else
        {
            temp = temp->next;
        }
    }
}

void parenthesisEval(Token** head, Token* startpos)
{
    Token* temp = startpos;

    unsigned short precedence_level = 0;

    while (temp->next != NULL && temp->next->type != CLOSE_PAR)
    {
        if (temp->type == POW)
        {
            precedence_level |= (1 << 3);
        }
        else if ((temp->type == MUL || temp->type == DIV) && precedence_level < 2)
        {
            precedence_level |= (1 << 2);
        } 
        else if ((temp->type == ADD || temp->type == SUB)  && precedence_level < 1)
        {
            precedence_level |= (1 << 1);
        }
        temp = temp->next;
    }


    if (precedence_level & (1 << 3))
    {
        while (temp->prev != NULL && temp->prev->type != OPEN_PAR)
        {
            if (temp->type == POW)
            {
                replaceThreeTokens(head, &temp, NUMBER, pow(temp->prev->value, temp->next->value));
            }
            else
            {
                temp = temp->prev;
            }
        }

        printTokens(head);
    }


    if (precedence_level & (1 << 2))
    {
        temp = startpos;

        while (temp->next != NULL && temp->next->type != CLOSE_PAR)
        {
            if (temp->type == MUL)
            {
                replaceThreeTokens(head, &temp, NUMBER, temp->prev->value * temp->next->value);
            }
            else if (temp->type == DIV)
            {
                replaceThreeTokens(head, &temp, NUMBER, temp->prev->value / temp->next->value);
            }
            else
            {
                temp = temp->next;
            }
        }

        printTokens(head);
    }


    if (precedence_level & (1 << 1))
    {
        temp = startpos;

        while (temp->next != NULL && temp->next->type != CLOSE_PAR)
        {
            if (temp->type == ADD)
            {
                replaceThreeTokens(head, &temp, NUMBER, temp->prev->value + temp->next->value);
            }
            else if (temp->type == SUB)
            {
                replaceThreeTokens(head, &temp, NUMBER, temp->prev->value - temp->next->value);
            }
            else
            {
                temp = temp->next;
            }
        }

        printTokens(head);
    }
}




void printToken(Token* token)
{
    switch (token->type)
    {
        case ADD:
            fprintf(stdout, "+");
            break;
        case SUB:
            fprintf(stdout, "-");
            break;
        case MUL:
            fprintf(stdout, "*");
            break;
        case DIV:
            fprintf(stdout, "/");
            break;
        case POW:
            fprintf(stdout, "^");
            break;
        case OPEN_PAR:
            fprintf(stdout, "(");
            break;
        case CLOSE_PAR:
            fprintf(stdout, ")");
            break;
        case NUMBER:
            fprintf(stdout, "%g", token->value);
            break;
        default:
            break;
    }
}

void printTokens(Token** head)
{
    Token* current = *head;
    while (current != NULL)
    {   
        printToken(current);
        fprintf(stdout, " ");
        current = current->next;
    }
    fprintf(stdout, "\n");
}

void reversePrintTokens(Token* head) {
    Token* current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    while (current != NULL) {
        printToken(current);
        fprintf(stdout, " ");
        current = current->prev;
    }
    printf("\n");
}

void freeTokens(Token** head)
{
    Token* current = *head;
    while (current != NULL)
    {
        Token* temp = current;
        current = current->next;
        free(temp);
    }
}

Token* createTokens(char* string)
{
    int len = strlen(string);
    Token* head = NULL;

    int current_token_count = 0;
    for (int i = 0; i < len; i++)
    {
        switch (string[i])
        {
        case '+':
            insertTokenEnd(&head, ADD, 0);
            break;

        case '-':
            insertTokenEnd(&head, SUB, 0);
            break;

        case '/':
            insertTokenEnd(&head, DIV, 0);
            break;

        case '*':
            insertTokenEnd(&head, MUL, 0);
            break;

        case '^':
            insertTokenEnd(&head, POW, 0);
            break;

        case '(':
            insertTokenEnd(&head, OPEN_PAR, 0);
            break;

        case ')':
            insertTokenEnd(&head, CLOSE_PAR, 0);
            break;

        default:
            if ('0' <= string[i] && '9' >= string[i])
            {
                int cursor = i;
                int decimal_index = -1;
                char* current_token = (char*)malloc(30 * sizeof(char));
                if (current_token == NULL)
                {
                    fprintf(stderr, "createTokens: Memory allocation failed.\n");
                    exit(1);
                }
                current_token[0] = '\0';

                while ((('0' <= string[cursor] && '9' >= string[cursor]) || string[cursor] == '.') && cursor < len)
                {
                    if (string[cursor] == '.')
                    {
                        decimal_index = cursor - i;
                    }
                    char temp[2];
                    temp[0] = string[cursor];
                    temp[1] = '\0';
                    strcat(current_token, temp);
                    if (cursor - i > 30)
                    {
                        fprintf(stderr, "input number is beyond double precision\n");
                    }
                    cursor++;
                }

                i = cursor - 1;

                double token_value = 0;
                int token_len = strlen(current_token);
                for (int j = 0; j < token_len; j++)
                {
                    if (j == decimal_index)
                    {
                        continue;
                    }

                    double place_value_exponent = 0;
                    if (decimal_index == -1)
                    {
                        place_value_exponent = token_len - j - 1;
                    }
                    else
                    {
                        place_value_exponent = decimal_index - j - ((j > decimal_index) ? 0 : 1);
                        
                    }
                    double place_value = pow(10, place_value_exponent);

                    token_value += ((current_token[j] - '0') * place_value);
                }

                insertTokenEnd(&head, NUMBER, token_value);

                free(current_token);
            }
            break;
        }
        current_token_count++;
    }

    return head;
    }


void evalEquation(char* equation)
{
    Token* tokens = createTokens(equation);
    verifyTokens(tokens);
    addImplicitMultipication(tokens);
    removeExtraParenthesis(&tokens);
    printTokens(&tokens);
    simplify(&tokens);
    freeTokens(&tokens);
}

