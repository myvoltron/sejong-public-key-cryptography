#ifndef BREAKING_TOOLS_H
#define BREAKING_TOOLS_H

#include "cipher_functions.h"
#include "common.h"

#define BREAK_HILL_CIPHER 1

#define MOD 26

typedef struct {
  char digram[3];
  double score;
} DigramScore;

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

void process_break_cipher();
void attack_hill_cipher(char *encrypted_text);
double score_plaintext(const char text[]);

void process_break_cipher() {
  int attack_mode;

  char *result = NULL;
  char encrypted_text[MAX_STR_LEN];

  printf("Select an attack method! 1: break hill cipher, others: Exit! -> ");
  scanf("%d", &attack_mode);
  getchar();

  printf("Enter the text to decrypt: ");
  scanf("%s", encrypted_text);

  switch (attack_mode) {
  case BREAK_HILL_CIPHER:
    attack_hill_cipher(encrypted_text);
    break;
  default:
    break;
  }
}

void attack_hill_cipher(char *encrypted_text) {
  int key[2][2];
  int inverse[2][2];
  char decrypted[100];

  double best_score = -1.0;
  char best_plaintext[100];
  int best_key[2][2] = {0};

  int **current_matrix = make_matrix(2);

  for (int a = 0; a < MOD; a++) {
    for (int b = 0; b < MOD; b++) {
      for (int c = 0; c < MOD; c++) {
        for (int d = 0; d < MOD; d++) {
          current_matrix[0][0] = a;
          current_matrix[0][1] = b;
          current_matrix[1][0] = c;
          current_matrix[1][1] = d;

          const int det_of_matrix =
              mod_alpha(get_determinant(current_matrix, 2));
          const int inverse_of_det = mod_inverse(det_of_matrix, 26);
          if (inverse_of_det == -1) {
            continue;
          }

          inverse[0][0] = mod_alpha(d * inverse_of_det);
          inverse[0][1] = mod_alpha(-b * inverse_of_det);
          inverse[1][0] = mod_alpha(-c * inverse_of_det);
          inverse[1][1] = mod_alpha(a * inverse_of_det);

          int i, j;
          for (i = 0; i < strlen(encrypted_text); i += 2) {
            int x = toupper(encrypted_text[i]) - 'A';
            int y = toupper(encrypted_text[i + 1]) - 'A';

            int p1 = mod_alpha(inverse[0][0] * x + inverse[0][1] * y);
            int p2 = mod_alpha(inverse[1][0] * x + inverse[1][1] * y);

            decrypted[i] = 'A' + p1;
            decrypted[i + 1] = 'A' + p2;
          }
          decrypted[i] = '\0';

          double current_score = score_plaintext(decrypted);
          if (current_score > best_score) {
            best_score = current_score;
            strcpy(best_plaintext, decrypted);
            best_key[0][0] = a;
            best_key[0][1] = b;
            best_key[1][0] = c;
            best_key[1][1] = d;
          }
        }
      }
    }
  }

  free_matrix(current_matrix, 2);

  printf("Best Key:");
  printf("%d %d\n", best_key[0][0], best_key[0][1]);
  printf("%d %d\n", best_key[1][0], best_key[1][1]);
  printf("Best Plaintext by Best Key: %s (Score: %.2f)\n", best_plaintext,
         best_score);
}

double score_plaintext(const char text[]) {
  double total_score = 0.0;
  for (int i = 0; text[i + 1] != '\0'; i++) {
    char digram[3];
    digram[0] = toupper(text[i]);
    digram[1] = toupper(text[i + 1]);
    digram[2] = '\0';

    for (int j = 0; j < digram_table_size; j++) {
      if (strcmp(digram, english_digrams_frequency_table[j].digram) == 0) {
        total_score += english_digrams_frequency_table[j].score;
        break;
      }
    }
  }
  return total_score;
}

#endif // !BREAKING_TOOLS_H
