#include "parser.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

u8 final_binary[255];

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

void parse_binary_operation(u8 opcode) {
  Token dest = expect(TOKEN_REGISTER, "Expected a destination register.");
  expect(TOKEN_COMMA, "Expected comma.");
  Token lhs = expect(TOKEN_REGISTER, "Expected register 1 as first operand.");
  expect(TOKEN_COMMA, "Expected comma.");
  Token rhs = expect(TOKEN_REGISTER, "Expected register 2 as second operand.");

  if (lhs.value != 0) {
    printf("Error on line %d: First operand must be register 1.\n", lhs.line);
    exit(1);
  }
  if (rhs.value != 1) {
    printf("Error on line %d: Second operand must be register 2.\n", rhs.line);
    exit(1);
  }

  compile_byte(opcode | (dest.value << 3));
}

// add DEST, reg1, reg2
// ; Performs reg1 + reg2 and stores result into register DEST
void parse_add() { parse_binary_operation(0b01000001); }

// sub DEST, reg1, reg2
// ; Performs reg1 - reg2 and stores result into register DEST
void parse_sub() { parse_binary_operation(0b01000010); }

// and DEST, reg1, reg2
// ; Performs reg1 & reg2 (bitwise and) and stores result into register DEST
void parse_and() { parse_binary_operation(0b01000100); }

// nand DEST, reg1, reg2
// ; Performs ~(reg1 & reg2) (bitwise nand) and stores result into register DEST
void parse_nand() { parse_binary_operation(0b01000011); }

// xor DEST, reg1, reg2
// ; Performs reg1 ^ reg2 (bitwise xor) and stores result into register DEST
void parse_xor() { parse_binary_operation(0b01000110); }

// or DEST, reg1, reg2
// ; Performs reg1 | reg2 (bitwise or) and stores result into register DEST
void parse_or() { parse_binary_operation(0b01000000); }

// eq DEST, reg1, reg2
// ; Performs reg1 == reg2 and stores result into register DEST
void parse_eq() { parse_binary_operation(0b01000111); }

// not DEST, reg1
// ; Performs ~reg1 (bitwise not) and stores result into register DEST
void parse_not() {
  Token dest = expect(TOKEN_REGISTER, "Expected a destination register.");
  expect(TOKEN_COMMA, "Expected comma.");
  Token lhs = expect(TOKEN_REGISTER, "Expected register 1 as operand.");

  if (lhs.value != 0) {
    printf("Error on line %d: Operand must be register 1.\n", lhs.line);
    exit(1);
  }

  int opcode = 0b01000101;
  compile_byte(opcode | (dest.value << 3));
}

void parse_line() {
  Token t = consume();

  if (t.type == TOKEN_MOV) {
    parse_move();
    return;
  }
  if (t.type == TOKEN_ADD) {
    parse_add();
    return;
  }
  if (t.type == TOKEN_SUB) {
    parse_sub();
    return;
  }
  if (t.type == TOKEN_AND) {
    parse_and();
    return;
  }
  if (t.type == TOKEN_NAND) {
    parse_nand();
    return;
  }
  if (t.type == TOKEN_OR) {
    parse_or();
    return;
  }
  if (t.type == TOKEN_XOR) {
    parse_xor();
    return;
  }
  if (t.type == TOKEN_EQ) {
    parse_eq();
    return;
  }
  if (t.type == TOKEN_NOT) {
    parse_not();
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
