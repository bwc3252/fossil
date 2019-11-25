// Token string defines

#ifndef TOKENS_H
#define TOKENS_H

#define LEFT_PAR    "("
#define RIGHT_PAR   ")"
#define LEFT_BR     "["
#define RIGHT_BR    "]"
#define LEFT_CURL   "{"
#define RIGHT_CURL  "}"

#define SET_EQ      "="

#define EQ          "=="
#define LESS        "<"
#define GREATER     ">"
#define LESS_EQ     "<="
#define GREATER_EQ  ">="
#define NOT_EQ      "!="

#define ADD         "+"
#define SUBTRACT    "-"
#define MULTIPLY    "*"
#define DIVIDE      "/"
#define POWER       "**"
#define REMAINDER   "%"

#define AND         "and"
#define OR          "or"
#define NOT         "not"

#define SEMICOLON   ";"
#define COMMA       ","

#define QUOTE       "\""

#define STRUCT_GET  "->"

#define RETURNS     "=>"

#define NEWLINE     "\n"
#define SPACE       " "
#define TAB         "\t"

#define EMPTY       ""

char *token_list[] = {NEWLINE, TAB, SPACE, EQ, LESS_EQ, GREATER_EQ, NOT_EQ, POWER,
                      STRUCT_GET, RETURNS, LEFT_PAR, RIGHT_PAR, LEFT_BR,
                      RIGHT_BR, LEFT_CURL, RIGHT_CURL, SET_EQ, LESS, GREATER,
                      ADD, SUBTRACT, MULTIPLY, DIVIDE, REMAINDER, AND, OR, NOT,
                      SEMICOLON, COMMA, QUOTE, NULL};

#endif
