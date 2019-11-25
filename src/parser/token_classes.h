#ifndef TOKEN_CLASSES_H
#define TOKEN_CLASSES_H

// keywords

#define KEYWORD_FN      "fn"
#define KEYWORD_IF      "if"
#define KEYWORD_ELIF    "elif"
#define KEYWORD_ELSE    "else"
#define KEYWORD_FOR     "for"
#define KEYWORD_WHILE   "while"
#define KEYWORD_RETURN  "return"
#define KEYWORD_INT     "int"
#define KEYWORD_FLOAT   "float"
#define KEYWORD_BOOL    "bool"
#define KEYWORD_STR     "str"
#define KEYWORD_TYPE    "=>"
#define KEYWORD_PRINT   "print"
#define KEYWORD_EQ      "="
#define KEYWORD_TRUE    "true"
#define KEYWORD_FALSE   "false"

// operators

#define OP_ADD          "+"
#define OP_SUBTRACT     "-"
#define OP_MULTIPLY     "*"
#define OP_DIVIDE       "/"
#define OP_EXP          "**"
#define OP_EQUAL        "=="
#define OP_LESS_EQUAL   "<="
#define OP_GR_EQUAL     ">="
#define OP_NOT_EQUAL    "!="
#define OP_NOT          "not"
#define OP_LESS         "<"
#define OP_GREATER      ">"
#define OP_AND          "and"
#define OP_OR           "or"
#define OP_NEGATE       "-"

enum operators {
    ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, EXP, EQUAL, LESS_EQUAL, GR_EQUAL,
    NOT_EQUAL, NOT_OP, LESS_OP, GREATER_OP, AND_OP, OR_OP, NEGATE, NO_OP
};

// delimiters

#define DELIM_LEFT_PAR  "("
#define DELIM_RIGHT_PAR ")"
#define DELIM_LEFT_BR   "["
#define DELIM_RIGHT_BR  "]"
#define DELIM_LEFT_CURL "{"
#define DELIM_RIGHT_CURL "}"

// assignments

enum assignments {
    EQUALS, PLUS_EQUALS, MINUS_EQUALS, TIMES_EQUALS, DIV_EQUALS
};

// miscellaneous stuff

#define QUOTE           "\""

#endif
