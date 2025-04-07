#include "breaking_tools.h"
#include "cipher_functions.h"

typedef enum {
  CIPHER_MODE = 1,
  BREAK_CIPHER_MODE = 2,
} PROGRAM_MODE;

/**
 * NOTE: This code is written in the following environment. (my c compiler
 * version!!)
 * Apple clang version 16.0.0 (clang-1600.0.26.6)
 * Target: arm64-apple-darwin24.3.0
 */
int main() {
  int program_mode;

  printf("Select an mode! 1: cipher mode, 2: break cipher mode, "
         "others: Exit -> ");
  scanf("%d", &program_mode);

  switch (program_mode) {
  case CIPHER_MODE:
    process_cipher();
    break;
  case BREAK_CIPHER_MODE:
    process_break_cipher();
    break;
  default:
    break;
  }

  return 0;
}
