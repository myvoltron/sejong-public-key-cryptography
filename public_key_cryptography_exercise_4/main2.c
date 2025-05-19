#include <stdio.h>
#include <string.h>

#define MAX_ROW 10
#define MAX_COLUMN 20
#define MAX_STR_LEN 200

int main(void) {
  char str[MAX_STR_LEN];
  char matrix[MAX_ROW][MAX_COLUMN];

  int row, column, key_size;
  int key_arr[MAX_COLUMN];
  int frequency;

  printf("Enter number of rows: ");
  scanf("%d", &row);
  printf("Enter number of columns: ");
  scanf("%d", &column);

  int expected_len = row * column;
  printf("Enter the plaintext (exactly %d characters): ", expected_len);
  scanf("%s", str);

  if (strlen(str) != expected_len) {
    printf("Error: The length of the input text must be %d.\n", expected_len);
    return 1;
  }

  printf("Enter frequency (number of rounds): ");
  scanf("%d", &frequency);

  printf("Enter key size (must equal %d): ", column);
  scanf("%d", &key_size);
  if (key_size != column) {
    printf("Error: Key size must match column count (%d).\n", column);
    return 1;
  }

  printf("Enter key array (%d values): ", key_size);
  for (int i = 0; i < key_size; i++) {
    scanf("%d", &key_arr[i]);
  }

  // Encryption
  for (int i = 0; i < frequency; i++) {
    // Fill matrix by row
    int idx = 0;
    for (int r = 0; r < row; r++) {
      for (int c = 0; c < column; c++) {
        matrix[r][c] = str[idx++];
      }
    }

    for (int r = 0; r < row; r++) {
      for (int c = 0; c < column; c++) {
        printf("%c ", matrix[r][c]);
      }
      printf("\n");
    }

    // Rearrange columns based on key
    idx = 0;
    for (int k = 0; k < key_size; k++) {
      int key_col = key_arr[k];
      for (int r = 0; r < row; r++) {
        str[idx++] = matrix[r][key_col];
      }
    }

    str[idx] = '\0';
    printf("Encrypted [%d]: %s\n", i + 1, str);
  }

  // Decryption
  for (int i = 0; i < frequency; i++) {
    // Build inverse key
    int inverse_key[MAX_COLUMN];
    for (int j = 0; j < key_size; j++) {
      inverse_key[key_arr[j]] = j;
    }

    // Fill matrix column-by-column
    int idx = 0;
    for (int k = 0; k < key_size; k++) {
      int orig_col = inverse_key[k];
      for (int r = 0; r < row; r++) {
        matrix[r][orig_col] = str[idx++];
      }
    }

    for (int r = 0; r < row; r++) {
      for (int c = 0; c < column; c++) {
        printf("%c ", matrix[r][c]);
      }
      printf("\n");
    }

    // Reconstruct str row-wise
    idx = 0;
    for (int r = 0; r < row; r++) {
      for (int c = 0; c < column; c++) {
        str[idx++] = matrix[r][c];
      }
    }

    str[idx] = '\0';
    printf("Decrypted [%d]: %s\n", i + 1, str);
  }

  return 0;
}
