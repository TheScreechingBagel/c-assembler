#include "parser.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

struct Parser {
  Token *current;
  int next_byte;
} parser;

Token peek() { return *parser.current; }

Token consume() {
  Token t = *parser.current;
  if (t.type == TOKEN_EOF) {
    printf("Error on line %d: Unexpected end of file\n", t.line);
    exit(1);
  }
  parser.current++; // go to next token
  return t;
}

Token expect(TokenType type, char *error_message) {
  Token t = consume();
  if (t.type != type) {
    printf("Error on line %d: %s\n", t.line, error_message);
    exit(1);
  }
  return t;
}

void init_parser() {
  parser.current = tokens;
  parser.next_byte = 0;
}

void compile_byte(u8 x) {
  final_binary[parser.next_byte] = x;
  parser.next_byte += 1;
}

void parse_move() {
  Token regis = expect(TOKEN_REGISTER, "You can only move into a register");
  expect(TOKEN_COMMA, "Expected comma");
  Token rhs = consume();

  // compile loading constant into register 1
  if (rhs.type == TOKEN_INTEGER) {
    if (regis.value != 0) {
      printf("Error on line %d: Can only move constant into register 1.\n",
             regis.line);
      exit(1);
    }

    if (rhs.value > 63) {
      printf("Error on line %d: Can't move numbers bigger than 63 into "
             "register 1.\n",
             rhs.line);
      exit(1);
    }

    compile_byte(rhs.value);
    return;
  }

  // compile loading register into another register
  if (rhs.type == TOKEN_REGISTER) {
    int register_a = rhs.value << 3;
    int register_b = regis.value;
    int opcode = 0b10000000;
    compile_byte(opcode | register_a | register_b);
    return;
  }

  // invalid case
  printf("Error on line %d: Expected either a register or a constant for move "
         "operation.\n",
         rhs.line);
  exit(1);
}

void parse_line() {
  Token t = consume();

  if (t.type == TOKEN_MOV) {
    parse_move();
    return;
  }

  printf("Error on line %d: Unexpected token.\n", t.line);
  exit(1);
}

void parse_lines() {
  init_parser();

  while (peek().type != TOKEN_EOF) {
    parse_line();
  }
}

void print_binary() {
  for (int i = 0; i < parser.next_byte; i++) {
    for (int j = 7; j >= 0; j--) {
      if (final_binary[i] & (1 << j)) {
        printf("1");
      } else {
        printf("0");
      }
    }
    printf("\n");
  }
}
