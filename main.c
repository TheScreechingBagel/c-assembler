#include "parser.h"
#include "scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  init_scanner("example.asm");
  scan_tokens();
  parse_lines();
  print_binary();
  return 0;
}
