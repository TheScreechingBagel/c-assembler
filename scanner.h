#pragma once

typedef enum {
  TOKEN_REGISTER,
  TOKEN_INTEGER,
  TOKEN_MOV,
  TOKEN_ADD,
  TOKEN_SUB,
  TOKEN_AND,
  TOKEN_NAND,
  TOKEN_OR,
  TOKEN_XOR,
  TOKEN_EQ,
  TOKEN_NOT,
  TOKEN_SEMICOLON,
  TOKEN_COMMA,
  TOKEN_EOF,
} TokenType;

typedef struct {
  TokenType type;
  int value;
  char *source;
  int length;
  int line;
} Token;

extern Token tokens[10000];

void init_scanner(char *file_name);

void scan_tokens();
