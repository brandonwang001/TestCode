#include <iostream>

int TwoNumSum(int a, int b) {
  int c = a + b;
  return c;
}


int ThreeNumSum(int a, int b, int c) {
  return TwoNumSum(TwoNumSum(a, b), c); 
}

int main() {
  int a = 1;
  int b = 2;
  int c = 3;
  int d = ThreeNumSum(a, b, c);
  return 0;
}
