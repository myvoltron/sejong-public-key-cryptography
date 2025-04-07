#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SHIFT 1
#define SUBSTITUTION 2
#define VIGENERE 3

#define ALPHABET_SIZE 26

char *shift_cipher(char *text, int key, bool is_encryption);
char *substitution_cipher(char *text, char key[], bool is_encryption);
char *vigenere_cipher(char *text, int *key_arr, int key_size,
                      bool is_encryption);
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

  int key;
  char substitution_keys[] = "abcdefghijklmnopqrstuvwxyz";
  int *key_arr = NULL;
  int key_size;

  srand(time(NULL));

  printf("Select an encryption method!\n1: Shift Cipher, 2: Substitution "
         "Cipher, 3: Vigenere Cipher, others: "
         "Exit! -> ");
  scanf("%d", &cipher_mode);
  if (cipher_mode != SHIFT && cipher_mode != SUBSTITUTION &&
      cipher_mode != VIGENERE) {
    exit(EXIT_FAILURE);
  }

  switch (cipher_mode) {
  case SHIFT:
    printf("Enter a shift key: ");
    scanf("%d", &key);
    break;
  case SUBSTITUTION:
    for (int i = ALPHABET_SIZE - 1; i > 0; i--) {
      int j = rand() % (i + 1);
      char temp = substitution_keys[i];
      substitution_keys[i] = substitution_keys[j];
      substitution_keys[j] = temp;
    }
    printf("Generated Substitution key: %s\n", substitution_keys);
    break;
  case VIGENERE:
    printf("Enter a key for the Vigenere cipher: ");
    scanf("%s", tmp);

    key_size = strlen(tmp);
    key_arr = malloc(sizeof(int) * key_size);
    if (!key_arr) {
      printf("Memory allocation failed!");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < key_size; i++) {
      if (tmp[i] >= 'a' && tmp[i] <= 'z') {
        key_arr[i] = tmp[i] - 'a';
      }
      if (tmp[i] >= 'A' && tmp[i] <= 'Z') {
        key_arr[i] = tmp[i] - 'A';
      }
    }
    break;
  }

  while (1) {
    int mode_selection;
    printf("1: encryption mode, 0: decryption mode, others: Exit! -> ");
    scanf("%d", &mode_selection);
    getchar();

    if (mode_selection != 1 && mode_selection != 0) {
      break;
    }
    is_encryption = mode_selection == 1 ? true : false;

    printf("Enter the text to encrypt or decrypt: ");
    scanf("%[^\n]s", input_text);

    switch (cipher_mode) {
    case SHIFT:
      result = shift_cipher(input_text, key, is_encryption);
      break;
    case SUBSTITUTION:
      result =
          substitution_cipher(input_text, substitution_keys, is_encryption);
      break;
    case VIGENERE:
      result = vigenere_cipher(input_text, key_arr, key_size, is_encryption);
      break;
    }

    printf("Result: %s\n", result);

    if (result) {
      free(result);
    }
  }

  if (key_arr) {
    free(key_arr);
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
char *substitution_cipher(char *text, char key[], bool is_encryption) {
  int str_len = strlen(text);

  char *result = malloc(sizeof(char) * (str_len + 1));
  if (!result) {
    printf("Memory allocation failed!");
    exit(EXIT_FAILURE);
  }
  strcpy(result, text);

  int index;
  if (is_encryption) {
    for (int i = 0; i < str_len; i++) {
      if (result[i] == ' ') {
        continue;
      }

      if (result[i] >= 'a' && result[i] <= 'z') {
        index = result[i] - 'a';
      }
      if (result[i] >= 'A' && result[i] <= 'Z') {
        index = result[i] - 'A';
      }

      result[i] = key[index];
    }
  } else {
    char reverse_key[26];
    for (int i = 0; i < 26; i++) {
      reverse_key[key[i] - 'a'] = 'a' + i;
    }

    for (int i = 0; i < str_len; i++) {
      if (result[i] == ' ') {
        continue;
      }

      result[i] = reverse_key[result[i] - 'a'];
    }
  }

  return result;
}
char *vigenere_cipher(char *text, int key_arr[], int key_size,
                      bool is_encryption) {
  int str_len = strlen(text);

  char *result = malloc(sizeof(char) * (str_len + 1));
  if (!result) {
    printf("Memory allocation failed!");
    exit(EXIT_FAILURE);
  }
  strcpy(result, text);

  int key_index = 0;
  for (int i = 0; i < str_len; i++) {
    if (result[i] == ' ') {
      key_index = 0;
      continue;
    }

    int shift = is_encryption ? key_arr[key_index] : key_arr[key_index] * -1;

    if (result[i] >= 'a' && result[i] <= 'z') {
      result[i] = 'a' + (result[i] - 'a' + shift + 26) % 26;
    }
    if (result[i] >= 'A' && result[i] <= 'Z') {
      result[i] = 'A' + (result[i] - 'A' + shift + 26) % 26;
    }

    key_index++;
    if (key_index == key_size) {
      key_index = 0;
    }
  }

  return result;
}
