#ifndef BREAKING_TOOLS_H
#define BREAKING_TOOLS_H

#include "cipher_functions.h"
#include "common.h"

typedef struct {
  char digram[3];
  double score;
} DigramScore;

typedef enum {
  BRUTE_FORCE_SHIFT_CIPHER = 1,
  FREQUENCY_ANALYSIS_SHIFT_CIPHER = 2,
  BREAK_PLAYFAIR_CIPHER = 3,
} BREAKING_TOOLS;

const char english_letter_frequency_table[] = {'E', 'T', 'A'};
const int english_letter_frequency_table_length = 3;

// English digram frequency. (by
// https://en.wikipedia.org/wiki/Bigram#Bigram_frequency_in_the_English_language)
const DigramScore english_digrams_frequency_table[] = {
    {"TH", 3.56}, {"HE", 3.07}, {"IN", 2.43}, {"ER", 2.05}, {"AN", 1.99},
    {"RE", 1.85}, {"ON", 1.76}, {"AT", 1.49}, {"EN", 1.45}, {"ND", 1.35},
    {"TI", 1.34}, {"ES", 1.34}, {"OR", 1.28}, {"TE", 1.20}, {"OF", 1.17},
    {"ED", 1.17}, {"IS", 1.13}, {"IT", 1.12}, {"AL", 1.09}, {"AR", 1.07},
    {"ST", 1.05}, {"TO", 1.05}, {"NT", 1.04}, {"NG", 0.95}, {"SE", 0.93},
    {"HA", 0.93}, {"AS", 0.87}, {"OU", 0.87}, {"IO", 0.83}, {"LE", 0.83},
    {"VE", 0.83}, {"CO", 0.79}, {"ME", 0.79}, {"DE", 0.76}, {"HI", 0.76},
    {"RI", 0.73}, {"RO", 0.73}, {"IC", 0.70}, {"NE", 0.69}, {"EA", 0.69},
    {"RA", 0.69}, {"CE", 0.65}};
const int digram_table_size =
    sizeof(english_digrams_frequency_table) / sizeof(DigramScore);

// Example Keywords to attack playfair cipher.
const char *keywords[] = {"MONARCHY", "KEYWORD",  "SECURITY", "FREEDOM",
                          "VICTORY",  "ENCRYPT",  "HISTORY",  "AGENT",
                          "NATION",   "OPERATION"};

void process_break_cipher();

void brute_force_shift_cipher(const char *encrypted_text);

void frequency_analysis_shift_cipher(const char *encrypted_text);
void count_frequency(const char *encrypted_text, char alpha_arr[26]);
int find_max_frequency_value(const char alpha_count_arr[26]);

/**
* This function performs a dictionary-based attack on a Playfair-encrypted
* ciphertext.
* It works by:
* 1. Generating key matrices from a list of possible keywords.
* 2. Decrypting the ciphertext using each matrix.
* 3. Scoring the results using English digram frequencies.
* 4. Printing each candidate so the best plaintext can be selected manually
(or automatically with scoring).
*/
void break_playfair_cipher(const char *encrypted_text);
double get_digram_score(const char *pair);
double score_plaintext(const char *text);

void process_break_cipher() {
  int attack_mode;

  char *result = NULL;
  char encrypted_text[MAX_STR_LEN];

  printf("Select an attack method! 1: brute force shift cipher, 2: frequency "
         "analysis shift cipher, 3: attack playfair cipher, others: Exit! -> ");
  scanf("%d", &attack_mode);
  getchar();

  printf("Enter the text to decrypt: ");
  scanf("%[^\n]s", encrypted_text);

  switch (attack_mode) {
  case BRUTE_FORCE_SHIFT_CIPHER:
    brute_force_shift_cipher(encrypted_text);
    break;
  case FREQUENCY_ANALYSIS_SHIFT_CIPHER:
    frequency_analysis_shift_cipher(encrypted_text);
    break;
  case BREAK_PLAYFAIR_CIPHER:
    break_playfair_cipher(encrypted_text);
    break;
  default:
    break;
  }
}

void brute_force_shift_cipher(const char *encrypted_text) {
  char *result = NULL;
  const bool is_encryption = false;

  for (int i = 0; i < 26; i++) {
    result = shift_cipher(encrypted_text, i, is_encryption);

    printf("(%dth) %s\n", i + 1, result);

    if (result) {
      free(result);
    }
  }
}

void frequency_analysis_shift_cipher(const char *encrypted_text) {
  const bool is_encryption = false;
  char letter_freq[26] = {0};

  count_frequency(encrypted_text, letter_freq);

  const int max_frequency_value = find_max_frequency_value(letter_freq);

  for (int i = 0; i < 26; i++) {
    if (letter_freq[i] != max_frequency_value)
      continue;

    char max_frequency_alpha = 'A' + i;
    for (int i = 0; i < english_letter_frequency_table_length; i++) {
      int key =
          (max_frequency_alpha - english_letter_frequency_table[i] + 26) % 26;
      char *result = shift_cipher(encrypted_text, key, is_encryption);

      printf("(assuming max letter is %c, matching to %c) (key of %d) -> %s\n",
             max_frequency_alpha, english_letter_frequency_table[i], key,
             result);

      if (result) {
        free(result);
      }
    }
  }
}

void count_frequency(const char *encrypted_text, char alpha_count_arr[26]) {
  for (int i = 0; encrypted_text[i] != '\0'; i++) {
    if (!isalpha(encrypted_text[i]))
      continue;
    char c = toupper(encrypted_text[i]);

    alpha_count_arr[c - 'A'] += 1;
  }
  return;
}

int find_max_frequency_value(const char alpha_count_arr[26]) {
  int max_frequency_value = -1;

  for (int i = 0; i < 26; i++) {
    if (alpha_count_arr[i] <= max_frequency_value)
      continue;

    max_frequency_value = alpha_count_arr[i];
  }

  return max_frequency_value;
}

void break_playfair_cipher(const char *encrypted_text) {
  double best_score = -1.0;
  char best_plain_text[MAX_STR_LEN];
  char best_key_matrix[5][5];

  const bool is_encryption = false;

  for (int keyword_index = 0; keyword_index < sizeof(keywords) / sizeof(char *);
       keyword_index++) {
    char *decrypted = NULL;
    char tmp_matrix[5][5];

    make_playfair_matrix(keywords[keyword_index],
                         strlen(keywords[keyword_index]), tmp_matrix);

    decrypted = playfair_cipher(encrypted_text, tmp_matrix, is_encryption);

    double current_score = score_plaintext(decrypted);
    if (current_score > best_score) {
      best_score = current_score;
      strcpy(best_plain_text, decrypted);
      memcpy(best_key_matrix, tmp_matrix, sizeof(tmp_matrix));
    }

    if (decrypted) {
      free(decrypted);
    }
  }

  printf("Best Decryption Result: %s\n", best_plain_text);
  printf("Key Matrix:\n");
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%c ", best_key_matrix[i][j]);
    }
    printf("\n");
  }
}

double get_digram_score(const char *pair) {
  for (int i = 0; i < digram_table_size; i++) {
    if (strncmp(pair, english_digrams_frequency_table[i].digram, 2) == 0) {
      return english_digrams_frequency_table[i].score;
    }
  }
  return 0.0;
}

double score_plaintext(const char *text) {
  double total_score = 0.0;
  int len = strlen(text);
  for (int i = 0; i < len - 1; i++) {
    char digram[3] = {text[i], text[i + 1], '\0'};
    total_score += get_digram_score(digram);
  }
  return total_score;
}

#endif // !BREAKING_TOOLS_H
