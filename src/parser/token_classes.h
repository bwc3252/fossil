#ifndef TOKEN_CLASSES_H
#define TOKEN_CLASSES_H

enum token_class {
    UNKNOWN = -1,
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    DELIMITER,
    LITERAL
};

// keywords

#define KEYWORD_FN      "fn"
#define KEYWORD_IF      "if"
#define KEYWORD_ELSE    "else"
#define KEYWORD_FOR     "for"
#define KEYWORD_WHILE   "while"
#define KEYWORD_IN      "in"
#define KEYWORD_RETURN  "return"
#define KEYWORD_INT     "int"
#define KEYWORD_FLOAT   "float"
#define KEYWORD_STR     "str"
#define KEYWORD_TYPE    "=>"

char *keywords[] = {KEYWORD_FN, KEYWORD_IF, KEYWORD_ELSE, KEYWORD_FOR, 
    KEYWORD_WHILE, KEYWORD_IN, KEYWORD_RETURN, KEYWORD_INT, KEYWORD_FLOAT,
    KEYWORD_STR, KEYWORD_TYPE, NULL};

// operators

#define OP_ADD          "+"
#define OP_SUBTRACT     "-"
#define OP_MULTIPLY     "*"
#define OP_DIVIDE       "/"
#define OP_EXP          "**"
#define OP_MOD          "%"
#define OP_EQUAL        "=="
#define OP_LESS_EQUAL   "<="
#define OP_GR_EQUAL     ">="
#define OP_NOT_EQUAL    "!="
#define OP_NOT          "!"
#define OP_LESS         "<"
#define OP_GREATER      ">"
#define OP_AND          "&"
#define OP_OR           "|"

char *operators[] = {OP_ADD, OP_SUBTRACT, OP_MULTIPLY, OP_DIVIDE, OP_EXP,
    OP_MOD, OP_EQUAL, OP_LESS_EQUAL, OP_GR_EQUAL, OP_NOT_EQUAL, OP_NOT,
    OP_LESS, OP_GREATER, OP_AND, OP_OR, NULL};

// delimiters

#define DELIM_LEFT_PAR  "("
#define DELIM_RIGHT_PAR ")"
#define DELIM_LEFT_BR   "["
#define DELIM_RIGHT_BR  "]"
#define DELIM_LEFT_CURL "{"
#define DELIM_RIGHT_CURL "}"

char *delimiters[] = {DELIM_LEFT_PAR, DELIM_RIGHT_PAR, DELIM_LEFT_BR,
    DELIM_RIGHT_BR, DELIM_LEFT_CURL, DELIM_RIGHT_CURL, NULL};

#endif
