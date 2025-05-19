#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 200

// ASCII to Binary
void print_binary(unsigned char ch) {
  for (int i = 7; i >= 0; i--) {
    printf("%d", (ch >> i) & 1);
  }
}

void generate_key(char *key, int length) {
  printf("Randomly generated key => ");
  for (int i = 0; i < length; i++) {
    key[i] = rand() % 256;
    printf("%c ", key[i]);
  }
  printf("\n");
}

void print_binary8(unsigned char ch) {
  for (int i = 7; i >= 0; i--) {
    printf("%d", (ch >> i) & 1);
  }
}

void encrypt(const char *plaintext, char *key, char *ciphertext) {
  printf("\n===== [ENCRYPTION - XOR] =====\n");

  int len = strlen(plaintext);
  for (int i = 0; i < len; i++) {
    ciphertext[i] = plaintext[i] ^ key[i];

    printf("%c (%3d, ", plaintext[i], (unsigned char)plaintext[i]);
    print_binary8(plaintext[i]);
    printf(") XOR ");

    printf("%c (%3d, ", key[i], (unsigned char)key[i]);
    print_binary8(key[i]);
    printf(") => ");

    printf("%c (%3d, ", ciphertext[i], (unsigned char)ciphertext[i]);
    print_binary8(ciphertext[i]);
    printf(")\n");
  }

  ciphertext[len] = '\0';
}

void decrypt(const char *ciphertext, const char *key, char *recovered) {
  int len = strlen(ciphertext);

  printf("\n===== [DECRYPTION - XOR] =====\n");

  for (int i = 0; i < len; i++) {
    recovered[i] = ciphertext[i] ^ key[i];

    printf("%c (%3d, ", ciphertext[i], (unsigned char)ciphertext[i]);
    print_binary8(ciphertext[i]);
    printf(") XOR ");

    printf("%c (%3d, ", key[i], (unsigned char)key[i]);
    print_binary8(key[i]);
    printf(") => ");

    printf("%c (%3d, ", recovered[i], (unsigned char)recovered[i]);
    print_binary8(recovered[i]);
    printf(")\n");
  }

  recovered[len] = '\0';
}

int main() {
  char plaintext[MAX_LEN];
  char key[MAX_LEN], ciphertext[MAX_LEN], recovered[MAX_LEN];
  int length;

  printf("Enter length of the plaintext: ");
  scanf("%d", &length);

  srand((unsigned int)time(NULL));
  generate_key(key, length);

  if (length <= 0 || length >= MAX_LEN) {
    printf("Invalid length.\n");
    return 1;
  }

  printf("Enter a plaintext of exactly %d characters: ", length);
  scanf("%s", plaintext);

  if (strlen(plaintext) != length) {
    printf("Error: Input length mismatch.\n");
    return 1;
  }

  encrypt(plaintext, key, ciphertext);
  printf("Encrypted Plaintext: %s\n", ciphertext);
  decrypt(ciphertext, key, recovered);
  printf("Recovered Plaintext: %s\n", recovered);

  return 0;
}
