#include <stdio.h>

#define ROW 4
#define COLUMN 9
#define KEY_SIZE 9

int main(void) {
  char str[] = "CRYPTOGRAPHYANDMATHEMATICSARETHEBEST";
  char matrix[ROW][COLUMN];
  int key_arr[KEY_SIZE];
  int frequency;

  printf("Enter frequency (number of rounds): ");
  scanf("%d", &frequency);

  for (int i = 0; i < 9; i++) {
    scanf("%d", &key_arr[i]);
  }

  for (int i = 0; i < frequency; i++) {
    int row = 0;
    int column = 0;
    for (int j = 0; str[j] != '\0'; j++) {
      matrix[row][column] = str[j];
      column++;
      if (column == COLUMN) {
        row++;
        column = 0;
      }
    }

    for (int r = 0; r < ROW; r++) {
      for (int c = 0; c < COLUMN; c++) {
        printf("%c ", matrix[r][c]);
      }
      printf("\n");
    }

    int str_ptr = 0;
    for (int key_idx = 0; key_idx < KEY_SIZE; key_idx++) {
      int key_column = key_arr[key_idx];
      for (int row_idx = 0; row_idx < ROW; row_idx++) {
        str[str_ptr++] = matrix[row_idx][key_column];
      }
    }

    printf("%s\n", str);
  }

  // decryption
  for (int i = 0; i < frequency; i++) {
    int str_index = 0;
    for (int key_idx = 0; key_idx < KEY_SIZE; key_idx++) {
      int original_column = key_arr[key_idx];
      for (int row_idx = 0; row_idx < ROW; row_idx++) {
        matrix[row_idx][original_column] = str[str_index++];
      }
    }

    for (int r = 0; r < ROW; r++) {
      for (int c = 0; c < COLUMN; c++) {
        printf("%c ", matrix[r][c]);
      }
      printf("\n");
    }

    int str_ptr = 0;
    for (int r = 0; r < ROW; r++) {
      for (int c = 0; c < COLUMN; c++) {
        str[str_ptr++] = matrix[r][c];
      }
    }

    str[str_ptr] = '\0'; // null-termination
    printf("Decrypted: %s\n", str);
  }
}
