#ifndef CIPHER_FUNCTIONS_H
#define CIPHER_FUNCTIONS_H

#include "common.h"

#define HILL_CIPHER 1

void process_cipher();
void process_hill_cipher();

void process_cipher() {
  int cipher_mode;

  printf("Select an method! 1: Hill Cipher, others: Exit! -> ");
  scanf("%d", &cipher_mode);

  switch (cipher_mode) {
  case HILL_CIPHER:
    process_hill_cipher();
    break;
  default:
    break;
  }

  return;
}

#endif
