#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
  struct hostent *h;

  if (argc!=2)
  {
    printf("Syntax: %s <hostname>", argv[0]);
    exit(-1);
  }

  h = gethostbyname(argv[1]);
  if (h==NULL)
  {
    fprintf(stderr, "Hostname \"%s\" could not be resolved\n", argv[1]);
    exit(-2);
  }

  printf("h_name = %s\n\n", h->h_name);

  printf("h_aliases =");
  char **p = h->h_aliases;
  while (*p != NULL)
  {
    printf("\n\t%s", *p);
    p++;
  }
  printf("\n\n");

  printf("h_addrtype = %i\n\n", h->h_addrtype);

  printf("h_length = %i\n\n", h->h_length);

  printf("h_addr_list =");
  struct in_addr **q = (struct in_addr **)h->h_addr_list;
  while (*q != NULL)
  {
    printf("\n\t%s", inet_ntoa(**q));
    q++;
  }
  printf("\n");

  return 0;
}
