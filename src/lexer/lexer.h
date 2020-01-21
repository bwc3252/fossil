// Header file for the lexer

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

// Struct to hold tokens in a linked list (storing them in this way is
// helpful for the parsing stage)
typedef struct token_s {
    int line_no;    // line number the token occurs on (for error messages)
    char *tok;      // the token itself
    size_t len;     // current length of the token string
    size_t size;    // size of allocated buffer
    struct token_s *next;  // pointer to next token struct (NULL if end of linked 
                    // list)
} *token_t;

// Function to consume character string and return a linked list of tokens
token_t tokenize(char *str);

// Constructor and destructor functions for token struct

token_t create_token(void);

void destroy_token_list(token_t token);

// Function to fill the token's string with the correct text and return the
// input string after consuming one full token
char *next_token(token_t token, char *str);

// Append a character to the end of a token
void token_append(token_t token, char c);

// A helper function to double token buffer size when capacity is reached
void realloc_token(token_t token);

#endif
