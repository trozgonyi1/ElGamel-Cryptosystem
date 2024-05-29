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
  int gcd = compute_inverse(b%a, a, &x1, &y1);

  *x = y1 - (b/a) * x1;
  *y = x1;

  return gcd;
}

int mod(int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}


int ipow(long long base, long exp, int modulus) {
  int result = 1;
  
  base = base % modulus;
  if ( base == 0 ) return 0;

  while ( exp > 0 ) {
    if ( exp & 1 ) {
      result = (result*base) % modulus;
    }

    exp = exp>>1;
    base = (base*base) % modulus;
  }
  return result;
}

int main(int argc, char *argv[]) {
   /*
    * PUBLIC PARAMETER CREATION
    * */ 
   int prime    = 467;
   size_t g     = 2;
   
   srand(time(NULL));

   int alice_private = (((int) rand()) % (prime - 1)) + 1;
   int A             = (ipow(g, alice_private, prime));
   
   printf("----------PUBLIC KEY----------\n");
   printf("--   A, p : %d, %d   ---\n", A, prime);
   printf("------------------------------\n\n\n");
   /*
    * A and p are published as the public key
    * */

   int plaintext     = 271;
   
   int ephemeral_key = (int) rand() % (prime - 1) + 1;
   int c1            = ipow(g, ephemeral_key, prime);
   int c2            = mod(plaintext * ipow(A, ephemeral_key, prime), prime);
   printf("----------CIPHERTEXT---------\n");
   printf("---(c1, c2) = (%d, %d)---\n", c1, c2);
   printf("-----------------------------\n\n\n");
   /*
    * (c1, c2) are sent as ciphertext
    * */

   int s             = ipow((long long) c1, (long) alice_private, prime);
   int s_inverse, m = 0;
   compute_inverse(s, prime, &s_inverse, &m);
   if ( s_inverse < 0 ){
    s_inverse = prime + s_inverse;
  }
   int decrypted     = (c2 * s_inverse) % prime;

   printf("==================\n");
   printf("Plaintext is : %d\n", decrypted);
   printf("==================\n");
}
