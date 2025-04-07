#ifndef CIPHER_FUNCTIONS_H
#define CIPHER_FUNCTIONS_H

#include "common.h"

#define SHIFT 1
#define PLAYFAIR 2

typedef struct {
  int row;
  int column;
} Position;

typedef enum {
  CIPHER_MODE_SHIFT = 1,
  CIPHER_MODE_PLAYFAIR = 2,
} CIPHER_FUNCTIONS;

const char FILLER_LETTER = 'C';

void process_cipher();

void process_shift_cipher();
char *shift_cipher(const char *text, int key, bool is_encryption);

void process_playfair_cipher();
void make_playfair_matrix(const char *key_str, const int key_size,
                          char key_matrix[5][5]);
void prepare_plain_text(const char *plain_text, char *output_text);
Position get_letter_position(const char c, const char key_matrix[5][5]);
void encrypt_digram(const char a, const char b, const char key_matrix[5][5],
                    const bool is_encryption, char *out_a, char *out_b);
char *playfair_cipher(const char *text, const char key_matrix[5][5],
                      const bool is_encryption);

void process_cipher() {
  int cipher_mode;

  printf("Select an method! 1: Shift Cipher, 2: Playfair Cipher, "
         "others: Exit! -> ");
  scanf("%d", &cipher_mode);

  switch (cipher_mode) {
  case CIPHER_MODE_SHIFT:
    process_shift_cipher();
    break;
  case CIPHER_MODE_PLAYFAIR:
    process_playfair_cipher();
    break;
  default:
    break;
  }

  return;
}

void process_shift_cipher() {
  /**
   * 1 -> encryption mode.
   * 0 -> decryption mode.
   */
  bool is_encryption;
  int key;
  char *result = NULL;

  char input_text[MAX_STR_LEN]; // you can use "attack", "retreat", or "hold
                                // current position".

  printf("Enter a shift key: ");
  scanf("%d", &key);

  while (true) {
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

    result = shift_cipher(input_text, key, is_encryption);

    printf("Result: %s\n", result);

    if (result) {
      free(result);
    }
  }

  return;
}

void process_playfair_cipher() {
  bool is_encryption;
  char input_text[MAX_STR_LEN]; // you can use "attack", "retreat", or "hold
                                // current position".
  char key_arr[MAX_STR_LEN];
  char key_matrix[5][5];

  char *result = NULL;

  printf("Enter a playfair key string: ");
  scanf(" %[^\n]s", key_arr);

  make_playfair_matrix(key_arr, strlen(key_arr), key_matrix);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%c ", key_matrix[i][j]);
    }
    printf("\n");
  }

  while (true) {
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

    result = playfair_cipher(input_text, key_matrix, is_encryption);

    printf("Result: %s\n", result);

    if (result) {
      free(result);
    }
  }
}

char *shift_cipher(const char *text, int key, bool is_encryption) {
  int str_len = strlen(text);

  char *result = (char *)malloc(sizeof(char) * (str_len + 1));
  if (!result) {
    printf("Memory allocation failed!");
    exit(EXIT_FAILURE);
  }
  strcpy(result, text);

  int shift = is_encryption ? key : key * -1;

  for (int i = 0; i < str_len; i++) {
    if (!isalpha(result[i]))
      continue;

    if (result[i] >= 'a' && result[i] <= 'z') {
      result[i] = 'a' + (result[i] - 'a' + shift + 26) % 26;
    }
    if (result[i] >= 'A' && result[i] <= 'Z') {
      result[i] = 'A' + (result[i] - 'A' + shift + 26) % 26;
    }
  }

  return result;
}

void make_playfair_matrix(const char *key_str, const int key_size,
                          char key_matrix[5][5]) {
  bool is_seen_alpha_arr[26] = {false};

  int row = 0;
  int column = 0;
  for (int i = 0; i < key_size; i++) {
    if (!isalpha(key_str[i]))
      continue;
    char c = toupper(key_str[i]);
    // I/J
    if (key_str[i] == 'j')
      c = 'I';
    if (!is_seen_alpha_arr[c - 'A']) {
      is_seen_alpha_arr[c - 'A'] = true;

      key_matrix[row][column] = c;
      column++;
      if (column == 5) {
        column = 0;
        row++;
      }
    }
  }

  for (char c = 'A'; c <= 'Z'; c++) {
    if (c == 'J')
      continue;
    if (!is_seen_alpha_arr[c - 'A']) {
      is_seen_alpha_arr[c - 'A'] = true;

      key_matrix[row][column] = c;
      column++;
      if (column == 5) {
        column = 0;
        row++;
      }
    }
  }

  return;
}

void prepare_plain_text(const char *plain_text, char *output_text) {
  int len = 0;
  for (int i = 0; plain_text[i] != '\0'; i++) {
    char c = toupper(plain_text[i]);
    if (!isalpha(c))
      continue;
    // I/J
    if (c == 'J')
      c = 'I';
    output_text[len] = c;
    len++;
  }
  output_text[len] = '\0';

  char result[MAX_STR_LEN];
  int result_index = 0;
  for (int i = 0; i < len; i += 2) {
    result[result_index] = output_text[i];
    result_index++;

    if (i + 1 >=
        len) { // if the plain text is of odd length, add the Filler letter!!
      result[result_index] = FILLER_LETTER;
    } else if (output_text[i] ==
               output_text[i + 1]) { // if both letters are the same, add the
                                     // Filler letter!!
      result[result_index] = FILLER_LETTER;
    } else {
      result[result_index] = output_text[i + 1];
    }
    result_index++;
  }
  result[result_index] = '\0';

  strcpy(output_text, result);
}

Position get_letter_position(const char c, const char key_matrix[5][5]) {
  Position position;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (key_matrix[i][j] == c) {
        position.row = i;
        position.column = j;
        break;
      }
    }
  }

  return position;
}

void encrypt_digram(const char a, const char b, const char key_matrix[5][5],
                    const bool is_encryption, char *out_a, char *out_b) {

  Position a_position = get_letter_position(a, key_matrix);
  Position b_position = get_letter_position(b, key_matrix);

  int out_a_row, out_a_column, out_b_row, out_b_column;
  const int shifter = is_encryption ? 1 : -1;
  if (a_position.column == b_position.column) { // same column -> go below
    out_a_row = (a_position.row + shifter + 5) % 5;
    out_a_column = a_position.column;
    out_b_row = (b_position.row + shifter + 5) % 5;
    out_b_column = b_position.column;
  } else if (a_position.row == b_position.row) { // same row -> go right
    out_a_row = a_position.row;
    out_a_column = (a_position.column + shifter + 5) % 5;
    out_b_row = b_position.row;
    out_b_column = (b_position.column + shifter + 5) % 5;
  } else { // not on the same row or column -> square !!
    out_a_column = b_position.column;
    out_a_row = a_position.row;

    out_b_column = a_position.column;
    out_b_row = b_position.row;
  }

  *out_a = key_matrix[out_a_row][out_a_column];
  *out_b = key_matrix[out_b_row][out_b_column];
}

char *playfair_cipher(const char *text, const char key_matrix[5][5],
                      const bool is_encryption) {
  char modified_plain_text[MAX_STR_LEN];
  char *result = (char *)malloc(sizeof(char) * MAX_STR_LEN);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed!\n");
    exit(EXIT_FAILURE);
  }

  prepare_plain_text(text, modified_plain_text);

  int k = 0;
  for (int i = 0; i < strlen(modified_plain_text); i += 2) {
    char out_a;
    char out_b;

    // break the message into digrams (groups of 2 letters)
    encrypt_digram(modified_plain_text[i], modified_plain_text[i + 1],
                   key_matrix, is_encryption, &out_a, &out_b);

    result[k++] = out_a;
    result[k++] = out_b;
  }
  result[k] = '\0';

  return result;
}

#endif
