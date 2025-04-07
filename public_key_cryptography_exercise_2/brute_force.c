#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT 1
#define SUBSTITUTION 2
#define VIGENERE 3

#define ALPHABET_SIZE 26

char *shift_cipher(char *text, int key, bool is_encryption);
/**
 * NOTE: This code is written in the following environment. (my c compiler
 * version!!)
 * Apple clang version 16.0.0 (clang-1600.0.26.6)
 * Target: arm64-apple-darwin24.3.0
 * Thread model: posix
 * InstalledDir: /Library/Developer/CommandLineTools/usr/bin
 */
int main() {
  int cipher_mode;
  /**
   * 1 -> encryption mode.
   * 0 -> decryption mode.
   */
  bool is_encryption;
  char tmp[100];

  char input_text[100]; // you can use "attack", "retreat", or "hold current
                        // position".
  char *result = NULL;

  printf("Enter the text to encrypt or decrypt: ");
  scanf("%[^\n]s", input_text);

  for (int key = 0; key < 26; key++) {
    result = shift_cipher(input_text, key, false);

    printf("%d key => decrypted result: %s\n", key, result);

    if (result) {
      free(result);
    }
  }

  return 0;
}

char *shift_cipher(char *text, int key, bool is_encryption) {
  int str_len = strlen(text);

  char *result = malloc(sizeof(char) * (str_len + 1));
  if (!result) {
    printf("Memory allocation failed!");
    exit(EXIT_FAILURE);
  }
  strcpy(result, text);

  int shift = is_encryption ? key : key * -1;

  for (int i = 0; i < str_len; i++) {
    if (result[i] == ' ') {
      continue;
    }

    if (result[i] >= 'a' && result[i] <= 'z') {
      result[i] = 'a' + (result[i] - 'a' + shift + 26) % 26;
    }
    if (result[i] >= 'A' && result[i] <= 'Z') {
      result[i] = 'A' + (result[i] - 'A' + shift + 26) % 26;
    }
  }

  return result;
}
