// Lexer code to tokenize an input program

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokens.h"
#include "lexer.h"

#define INIT_BUF_SIZE 4 // big enough for all "standard" tokens (i.e. not 
                        // variable names, etc.)
                        // NOTE: this MUST be at least 2

int line_no;

token_t tokenize(char *str) {
    line_no = 1;
    token_t token; // the current token struct
    token_t head = NULL; // first token in the linked list
    while (*str != '\0') {
        if (*str == ' ' || *str == '\t') {
            // skip whitespace
            ++ str;
            continue;
        }
        if (*str == '\n') {
            // increment line number, but otherwise skip
            ++ str;
            ++ line_no;
            continue;
        }
        // at this point, we should always be at the beginning of a new token
        if (head == NULL) {
            // first token
            token = create_token();
            head = token;
        }
        else {
            // not the first token
            token->next = create_token();
            token = token->next;
        }
        // fill the token with the correct contents
        str = next_token(token, str);
    }
    return head;
}

char *next_token(token_t token, char *str) {
    switch (*str) {
        // first, check for the simple one-character tokens
        case '(':
            token_append(token, *str);
            return str + 1;
        case ')':
            token_append(token, *str);
            return str + 1;
        case '{':
            token_append(token, *str);
            return str + 1;
        case '}':
            token_append(token, *str);
            return str + 1;
        case '+':
            token_append(token, *str);
            return str + 1;
        case '-':
            token_append(token, *str);
            return str + 1;
        case '*':
            token_append(token, *str);
            return str + 1;
        case '/':
            token_append(token, *str);
            return str + 1;
        case ';':
            token_append(token, *str);
            return str + 1;
        // next, check for either = or ==
        case '=':
            token_append(token, *str);
            ++ str;
            if (*str == '\0' || *str != '=') {
                token_append(token, *str);
                return str;
            }
            token_append(token, *str);
            return str + 1;
        // do similar checks for <, <=, >, >=, and !=
        case '<':
            token_append(token, *str);
            ++ str;
            if (*str == '\0' || *str != '=') {
                token_append(token, *str);
                return str;
            }
            token_append(token, *str);
            return str + 1;
        case '>':
            token_append(token, *str);
            ++ str;
            if (*str == '\0' || *str != '=') {
                token_append(token, *str);
                return str;
            }
            token_append(token, *str);
            return str + 1;
        case '!':
            token_append(token, *str);
            ++ str;
            if (*str == '\0' || *str != '=') {
                token_append(token, *str);
                return str;
            }
            token_append(token, *str);
            return str + 1;
        // default case for all the rest of the tokens
        default:
            while (*str != '\0' && *str != '\n' && *str != ' ' && *str != '\t'
                    && *str != '(' && *str != ')' && *str != '{' && *str != '}'
                    && *str != '=' && *str != '<' && *str != '>' && *str != '!'
                    && *str != '+' && *str != '-' && *str != '*' && *str != '/'
                    && *str != ';') {
                token_append(token, *str);
                ++ str;
            }
            return str;
    }
}

token_t create_token(void) {
    token_t token = malloc(sizeof(struct token_s));
    token->line_no = line_no;
    token->tok = calloc(INIT_BUF_SIZE, sizeof(char));
    token->len = 0;
    token->size = INIT_BUF_SIZE;
    token->next = NULL;
    return token;
}

void destroy_token_list(token_t token) {
    token_t temp;
    while (token != NULL) {
        temp = token->next;
        free(token->tok);
        free(token);
        token = temp;
    }
}

void token_append(token_t token, char c) {
    realloc_token(token);
    token->tok[token->len] = c;
    ++ token->len;
    token->tok[token->len] = '\0';
}

void realloc_token(token_t token) {
    // check if the token string is at capacity, and if it is, double
    // the size of the buffer
    if (token->len >= token->size - 1) {
        token->tok = realloc(token->tok, 2 * token->size);
        token->size *= 2;
    }
}
