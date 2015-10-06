#include "add.h"

int main(int argc, char **argv) {
  CLIENT *cl;
  add_in in;
  add_out *out;

  if (argc!=4) { 
    printf("Usage: %s <machine> <int1> <int2>\n\n",argv[0]); return 1; 
  }

  cl = clnt_create(argv[1], ADD_PROGRAM, ADD_VERSION, "tcp");
  if (cl==NULL) {
    perror("Error creating RPC client!");
    return 1;
  }
  in.arg1 = atol(argv[2]);
  in.arg2 = atol(argv[3]);
  out = add_1(&in, cl);
  
  if (out==NULL) {
    printf("Error: %s\n",clnt_sperror(cl,argv[1]));
    return 1;
  }
  else {
    printf("We received the result: %ld\n",*out);
  }
  return 0;
}
