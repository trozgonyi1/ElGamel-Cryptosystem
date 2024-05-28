#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

int compute_inverse(int a, int b, int *x, int *y) {
  if ( a == 0 ){
    *x = 0;
    *y = 1;
    return b;
  }

  int x1, y1;
  compute_inverse(b%a, a, &x1, &y1);

  *x = y1 - (b/a) * x1;
  *y = x1;

  int inverse = *x;
  if (*x < 0) {
    inverse = b + *x;
  }
  return inverse % b;
}

int ipow(int base, int exp, int modulus) {
  int result = 1;
  for (;;){
    if (exp & 1) {
      result *= base;
    }

    exp >>= 1;
    if (!exp) {
      break;
    }
    base *= base;
    base = base % modulus;
  }

  return result % modulus;
}

int main(int argc, char *argv[]) {
   /*
    * PUBLIC PARAMETER CREATION
    * */ 
   int prime    = 11;
   size_t g     = 7;
   
   srand(time(NULL));

   int alice_private = (((int) rand()) % (prime - 1)) + 1;
   // int alice_private = 5;
   int A             = (ipow(g, alice_private, prime));
   printf("A = %d\n", A);
   /*
    * A is published as the public key
    * */

   int plaintext     = 4;
   
   int ephemeral_key = (int) rand() % (prime - 1) + 1;
   // int ephemeral_key = 10;
   printf("Ephemeral key = %d\n", ephemeral_key);
   // int ephemeral_key = 7;
   int c1            = (ipow(g, ephemeral_key, prime));
   printf("MIDWAY = %d\n", ipow(A, ephemeral_key, prime));
   int c2            = (plaintext * ipow(A, ephemeral_key, prime) % prime);
   printf("(c1, c2) = (%d, %d)\n", c1, c2);
   /*
    * (c1, c2) are sent as ciphertext
    * */

   int s             = ((int) pow(c1, alice_private)) % prime;
   int l, m;
   int s_inverse     = compute_inverse(s, prime, &l, &m);
   int decrypted     = (c2 * s_inverse) % prime;

   printf("Plaintext is : %d\n", decrypted);
}
