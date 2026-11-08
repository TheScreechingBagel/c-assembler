#include "parser.h"
#include "scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }
  init_scanner(argv[1]);
  scan_tokens();
  parse_lines();
  print_binary();
  return 0;
}
