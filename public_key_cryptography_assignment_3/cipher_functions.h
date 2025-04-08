#ifndef CIPHER_FUNCTIONS_H
#define CIPHER_FUNCTIONS_H

#include "common.h"
#include <math.h>
#include <time.h>

#define HILL_CIPHER 1

void process_cipher();
void process_hill_cipher();

int **make_matrix(int matrix_size);
void free_matrix(int **matrix, int matrix_size);
void print_matrix(int **matrix, int matrix_size);
int mod_alpha(int value);

char *apply_hill_cipher(char *text, int **key_matrix, int key_matrix_size);

int get_cofactor(int **matrix, int matrix_size, int row, int column);
void get_minor(int **src, int **dest, int n, int row, int col);
int get_determinant(int **matrix, int matrix_size);

int **get_adjoint_matrix(int **matrix, int matrix_size);
void get_cofactor_matrix(int **original_matrix, int **dest_matrix,
                         int matrix_size);
void transpose_matrix(int **original_matrix, int **dest_matrix,
                      int matrix_size);

int mod_inverse(int det, int mod);
void get_inverse_of_matrix(int inverse_of_det, int **adj_matrix,
                           int **inverse_of_matrix, int matrix_size);

void print_matrix_product_proof(int **a_matrix, int **b_matrix,
                                int matrix_size);

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

void process_hill_cipher() {
  char text[100];
  char *encrypted_text = NULL;
  char *decrypted_text = NULL;

  int **key_matrix = NULL;
  int **adjoint_of_key_matrix = NULL;
  int key_matrix_size;

  srand(time(NULL));

  printf("Input a key matrix size: ");
  scanf("%d", &key_matrix_size);

  key_matrix = make_matrix(key_matrix_size);

  // generate randomly key matrix based on key matrix size until a inverse of
  // the key matrix exists.
  bool has_inverse = false;
  while (has_inverse != true) {
    for (int i = 0; i < key_matrix_size; i++) {
      for (int j = 0; j < key_matrix_size; j++) {
        key_matrix[i][j] = mod_alpha(rand());
      }
    }
    const int det_of_matrix =
        mod_alpha(get_determinant(key_matrix, key_matrix_size));
    const int inverse_of_det = mod_inverse(det_of_matrix, 26);
    if (inverse_of_det == -1) {
      continue;
    }
    has_inverse = true;
  }

  // print randomly generated key matrix
  printf("randomly generated key matrix:\n");
  print_matrix(key_matrix, key_matrix_size);

  // find a inverse of the key matrix with proof.
  const int det_of_matrix =
      mod_alpha(get_determinant(key_matrix, key_matrix_size));
  printf("det_of_matrix: %d\n", det_of_matrix);

  const int inverse_of_det = mod_inverse(det_of_matrix, 26);
  if (inverse_of_det == -1) {
    printf(
        "Error: The inverse of the determinant does not exist modulo 26!!\n");
    exit(1);
  }
  printf("inverse of determinant in mod 26: %d\n", inverse_of_det);

  // Assume input text is multiple of the key matrix size.
  printf("Input a text to encrypt and decyprt: ");
  scanf("%s", text);
  if (strlen(text) % key_matrix_size != 0) {
    fprintf(stderr, "Input text length must be a multiple of key size.\\n");
    exit(EXIT_FAILURE);
  }

  // encryption
  encrypted_text = apply_hill_cipher(text, key_matrix, key_matrix_size);
  printf("encrypted text: %s\n", encrypted_text);

  adjoint_of_key_matrix = get_adjoint_matrix(key_matrix, key_matrix_size);
  printf("adjoint of key matrix:\n");
  print_matrix(adjoint_of_key_matrix, key_matrix_size);

  int **inverse_of_key_matrix = make_matrix(key_matrix_size);
  get_inverse_of_matrix(inverse_of_det, adjoint_of_key_matrix,
                        inverse_of_key_matrix, key_matrix_size);
  printf("inverse of key matrix:\n");
  print_matrix(inverse_of_key_matrix, key_matrix_size);

  print_matrix_product_proof(key_matrix, inverse_of_key_matrix,
                             key_matrix_size);

  // decryption
  decrypted_text =
      apply_hill_cipher(encrypted_text, inverse_of_key_matrix, key_matrix_size);
  printf("decrypted text: %s\n", decrypted_text);

  free_matrix(key_matrix, key_matrix_size);
  free_matrix(adjoint_of_key_matrix, key_matrix_size);
  free_matrix(inverse_of_key_matrix, key_matrix_size);

  if (encrypted_text) {
    free(encrypted_text);
  }

  if (decrypted_text) {
    free(decrypted_text);
  }
}

int **make_matrix(int matrix_size) {
  int **result = (int **)malloc(sizeof(int *) * matrix_size);
  if (result == NULL) {
    fprintf(stderr, "Insufficient memory");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < matrix_size; i++) {
    result[i] = (int *)malloc(sizeof(int) * matrix_size);
    if (result[i] == NULL) {
      fprintf(stderr, "Insufficient memory");
      exit(EXIT_FAILURE);
    }
  }

  return result;
}

void free_matrix(int **matrix, int matrix_size) {
  for (int i = 0; i < matrix_size; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

int mod_alpha(int value) { return ((value % 26) + 26) % 26; }

char *apply_hill_cipher(char *text, int **key_matrix, int key_matrix_size) {
  char *result = (char *)malloc(sizeof(char) * (strlen(text) + 1));
  if (result == NULL) {
    fprintf(stderr, "Insufficient memory");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < strlen(text); i += key_matrix_size) {
    for (int j = 0; j < key_matrix_size; j++) {
      int sum = 0;
      for (int k = 0; k < key_matrix_size; k++) {
        char c = toupper(text[i + k]);
        sum += (c - 'A') * key_matrix[j][k];
      }
      result[i + j] = 'A' + mod_alpha(sum);
    }
  }

  return result;
}

int get_cofactor(int **matrix, int matrix_size, int row, int column) {
  int minor_matrix_size = matrix_size - 1;
  int **minor_matrix = make_matrix(minor_matrix_size);
  get_minor(matrix, minor_matrix, matrix_size, row, column);

  const int sign = ((row + column) % 2 == 0) ? 1 : -1;
  const int cofactor = sign * get_determinant(minor_matrix, minor_matrix_size);

  free_matrix(minor_matrix, minor_matrix_size);
  return cofactor;
}

int get_determinant(int **matrix, int matrix_size) {
  if (matrix_size == 1) {
    return matrix[0][0];
  }

  if (matrix_size == 2) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  }

  int det = 0;
  for (int i = 0; i < matrix_size; i++) {
    det += matrix[0][i] * get_cofactor(matrix, matrix_size, 0, i);
  }

  return det;
}

void get_minor(int **src, int **dest, int n, int row, int col) {
  int r = 0, c = 0;
  for (int i = 0; i < n; i++) {
    if (i == row)
      continue;
    c = 0;
    for (int j = 0; j < n; j++) {
      if (j == col)
        continue;
      dest[r][c++] = src[i][j];
    }
    r++;
  }
}

int **get_adjoint_matrix(int **matrix, int matrix_size) {
  int **cofactor_matrix = make_matrix(matrix_size);
  get_cofactor_matrix(matrix, cofactor_matrix, matrix_size);

  int **adjoint_matrix = make_matrix(matrix_size);
  transpose_matrix(cofactor_matrix, adjoint_matrix, matrix_size);

  free_matrix(cofactor_matrix, matrix_size);
  // mod 26
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j < matrix_size; j++) {
      adjoint_matrix[i][j] = mod_alpha(adjoint_matrix[i][j]);
    }
  }

  return adjoint_matrix;
}

void get_cofactor_matrix(int **original_matrix, int **dest_matrix,
                         int matrix_size) {
  for (int row = 0; row < matrix_size; row++) {
    for (int column = 0; column < matrix_size; column++) {
      dest_matrix[row][column] =
          get_cofactor(original_matrix, matrix_size, row, column);
    }
  }
}
void transpose_matrix(int **original_matrix, int **dest_matrix,
                      int matrix_size) {
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j < matrix_size; j++) {
      dest_matrix[j][i] = original_matrix[i][j];
    }
  }
}

int mod_inverse(int det, int mod) {
  for (int i = 1; i < mod; i++) {
    if ((det * i) % mod == 1)
      return i;
  }
  return -1; // inverse doesn't exist
}

void get_inverse_of_matrix(int inverse_of_det, int **adj_matrix,
                           int **inverse_of_matrix, int matrix_size) {
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j < matrix_size; j++) {
      inverse_of_matrix[i][j] = mod_alpha((inverse_of_det * adj_matrix[i][j]));
    }
  }
}

void print_matrix_product_proof(int **a_matrix, int **b_matrix,
                                int matrix_size) {
  int **result = make_matrix(matrix_size);

  printf("Proof inverse of matrix!\n");

  printf("Multiply of each matrix:\n");
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j < matrix_size; j++) {
      result[i][j] = 0;
      for (int k = 0; k < matrix_size; k++) {
        result[i][j] += a_matrix[i][k] * b_matrix[k][j];
      }
    }
  }
  print_matrix(result, matrix_size);

  printf("Multiply of each matrix modulo 26:\n");
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j < matrix_size; j++) {
      result[i][j] = mod_alpha(result[i][j]);
    }
  }
  print_matrix(result, matrix_size);

  free_matrix(result, matrix_size);
}

void print_matrix(int **matrix, int matrix_size) {
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j < matrix_size; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

#endif
