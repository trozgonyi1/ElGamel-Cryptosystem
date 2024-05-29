#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const char *HOST = "localhost";
const char *PORT = "2701";

FILE *socket_dial(const char *host, const char *port) {
    /* Lookup server address information */
    struct addrinfo *results;
    struct addrinfo  hints = {
        .ai_family   = AF_UNSPEC,   /* Return IPv4 and IPv6 choices */
        .ai_socktype = SOCK_STREAM, /* Use TCP */
    };
    int status;
    if ((status = getaddrinfo(host, port, &hints, &results)) != 0) {
    	fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
	return NULL;
    }

    /* For each server entry, allocate socket and try to connect */
    int client_fd = -1;
    for (struct addrinfo *p = results; p != NULL && client_fd < 0; p = p->ai_next) {
	/* Allocate socket */
	if ((client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
	    fprintf(stderr, "Unable to make socket: %s\n", strerror(errno));
	    continue;
	}

	/* Connect to host */
	if (connect(client_fd, p->ai_addr, p->ai_addrlen) < 0) {
	    close(client_fd);
	    client_fd = -1;
	    continue;
	}
    }

    /* Release allocate address information */
    freeaddrinfo(results);

    if (client_fd < 0) {
	fprintf(stderr, "Unable to connect to %s:%s: %s\n", host, port, strerror(errno));
    	return NULL;
    }

    /* Open file stream from socket file descriptor */
    FILE *client_file = fdopen(client_fd, "w+");
    if (!client_file) {
        fprintf(stderr, "Unable to fdopen: %s\n", strerror(errno));
        close(client_fd);
        return NULL;
    }

    return client_file;
}

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

void parse_pub_key(char *str, int *c1, int *c2) {
  const char delim[] = ",";
  char *field1, *field2;

  field1 = strtok(str, delim);
  if (field1 == NULL){
    return;
  }
  field2 = strtok(NULL,delim);
  if (field2 == NULL) {
    return;
  }
  *c1 = atoi(field1);
  *c2 = atoi(field2);
}

int main(int argc, char *argv[]) {
    /* Connect to remote machine */
    FILE *client_file = socket_dial(HOST, PORT);
    if (!client_file) {
        return EXIT_FAILURE;
    }

    int prime = 0;
    int g     = 0;

    printf("Enter the prime modulus : ");
    scanf("%d", &prime);

    printf("Enter the base : ");
    scanf("%d", &g);
    getchar();
    
    srand(time(NULL));
    int alice_private = ((int) rand() % (prime - 1)) + 1;
    int A             = ipow(g, alice_private, prime);

    /* Read from stdin and send to server */
    char buffer[BUFSIZ];
    char pub_key_transfer[BUFSIZ];
    sprintf(pub_key_transfer, "%d,%d,%d\n", A, prime, g);
   
    fputs(pub_key_transfer, client_file);
    fgets(buffer, BUFSIZ, client_file);
    char *pos = strstr(buffer, "PUBLIC KEY: ");
    int c1, c2;
    if (pos) {
      char *pub_key = pos + strlen("PUBLIC KEY: ");
      parse_pub_key(pub_key, &c1, &c2);
      fputs(buffer, stdout);
      printf("\n");

      int s            = ipow((long long) c1, (long) alice_private, prime);
      int s_inverse, m = 0;
      compute_inverse(s, prime, &s_inverse, &m);
      if (s_inverse < 0) {
        s_inverse = prime + s_inverse;
      }
      int decrypted = (c2 * s_inverse) % prime;
      printf("-------------------\n");
      printf("Plaintext is : %d\n", decrypted);
      printf("-------------------\n");
    } else {
      printf("Bad response from server\n");
    }

    fclose(client_file);
    return EXIT_SUCCESS;
}
