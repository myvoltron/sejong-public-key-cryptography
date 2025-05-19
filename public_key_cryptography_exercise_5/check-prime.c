#include <stdio.h>
#include <stdlib.h>

void print_divisor_of_num(int num);
int check_prime(int num);
int check_co_prime_a_and_b(int a, int b);
int gcd(int a, int b);

int main() {
  int n;
  int *number_arr;

  printf("How many numbers do you want to enter: ");
  scanf("%d", &n);

  number_arr = (int *)malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    scanf("%d", &number_arr[i]);
    print_divisor_of_num(number_arr[i]);
  }

  // prime check
  for (int i = 0; i < n; i++) {
    int is_prime = check_prime(number_arr[i]);
    printf("%d is ", number_arr[i]);
    if (is_prime) {
      printf("prime!");
    } else {
      printf("not prime!");
    }
    printf("\n");
  }

  // co-prime check
  for (int i = 0; i < n; i++) {
    printf("co-prime with %d: ", number_arr[i]);
    for (int j = 0; j < n; j++) {
      if (i == j)
        continue;

      int is_coprime = check_co_prime_a_and_b(number_arr[i], number_arr[j]);
      if (is_coprime) {
        printf("%d ", number_arr[j]);
      }
    }
    printf("\n");
  }
}

void print_divisor_of_num(int num) {
  printf("divisors of %d: ", num);
  for (int i = 1; i <= num; i++) {
    if (num % i == 0) {
      printf("%d ", i);
    }
  }
  printf("\n");
}
int check_prime(int num) {
  int result = 1;
  for (int i = 2; i < num; i++) {
    if (num % i == 0) {
      result = 0;
    }
  }
  return result;
}
int check_co_prime_a_and_b(int a, int b) { return gcd(a, b) == 1; }
int gcd(int a, int b) {
  while (b != 0) {
    int tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}
