#ifndef BREAKING_TOOLS_H
#define BREAKING_TOOLS_H

#include "cipher_functions.h"
#include "common.h"

#define BREAK_HILL_CIPHER 1

typedef struct {
  char digram[3];
  double score;
} DigramScore;

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
void attack_hill_cipher(char *encrypted_text);

void process_break_cipher() {
  int attack_mode;

  char *result = NULL;
  char encrypted_text[MAX_STR_LEN];

  printf("Select an attack method! 1: break hill cipher, others: Exit! -> ");
  scanf("%d", &attack_mode);
  getchar();

  printf("Enter the text to decrypt: ");
  scanf("%[^\n]s", encrypted_text);

  switch (attack_mode) {
  case BREAK_HILL_CIPHER:
    attack_hill_cipher(encrypted_text);
    break;
  default:
    break;
  }
}

void attack_hill_cipher(char *encrypted_text) { return; }

#endif // !BREAKING_TOOLS_H
