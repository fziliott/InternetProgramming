#include "add.h"

add_out *add_1_svc(add_in *in, struct svc_req *rqstp)
{
  static add_out out;
  printf("Received request: %ld+%ld\n", in->arg1, in->arg2);
  printf("Version %i\n\n", ADD_VERSION);
  out = in->arg1 + in->arg2;
  return(&out);
}
