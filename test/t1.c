#include <stdio.h>


int a = 3;

int myInt(int a, int b, int c, int d) {
  printf("%d", a);
  printf("%d", b);
  return 11;
}

int main() {
  int a = myInt(1, 2,3,4);

  return 44;
}
