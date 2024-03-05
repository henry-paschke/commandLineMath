#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    OPEN_PAR,
    CLOSE_PAR,
    NUMBER
}
TokenType;

typedef struct Token 
{
    double value;
    TokenType type;

    struct Token* next;
    struct Token* prev;
} 
Token;

Token* createToken(TokenType type, double value);
void insertTokenEnd(Token** head, TokenType type, double value);
void insertTokenStart(Token** head, TokenType type, double value);
void insertTokenAfter(Token* prev, TokenType type, double value);
void insertTokenBefore(Token* next, TokenType type, double value);
void replaceThreeTokens(Token** head, Token** center, TokenType type, double value);
int getTokenCount(Token* head);

void verifyTokens(Token* head);
void addImplicitMultipication(Token* head);
void simplify(Token** head);
void removeExtraParenthesis(Token** head);
void parenthesisEval(Token** head, Token* startpos);

void printToken(Token* token);
void printTokens(Token** head);
void reversePrintTokens(Token* head);
void freeTokens(Token** head);
Token* createTokens(char* string);

void evalEquation(char* equation);

#endif