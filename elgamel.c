#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

int compute_inverse(int a, int b) {
  int g = b/a;
  int r = b - a*g;
  printf("G: %d, R: %d\n", g, r);

  if (r != 0) {
    compute_inverse(r, a); 
  } else {
    return 0;
  }
  return 0;
}

int main(int argc, char *argv[]) {
   /*
    * PUBLIC PARAMETER CREATION
    * */ 
   compute_inverse(8,11);
   size_t prime = 467;
   size_t g     = 2;
   
   srand(time(NULL));

   int a = rand() % (prime - 1);
   int A = pow(g, a);
   /*
    * A is published as the public key
    * */

   int plaintext     = 2701;
   
   int ephemeral_key = rand() % (prime - 1);
   int c1            = pow(g, ephemeral_key);
   int c2            = plaintext * pow(A, ephemeral_key);
   /*
    * (c1, c2) are sent as ciphertext
    * */

   

}
