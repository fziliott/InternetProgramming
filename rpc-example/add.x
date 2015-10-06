struct add_in
{
  long arg1;
  long arg2;
};

typedef long add_out;

program ADD_PROGRAM
{
  version ADD_VERSION
  {
    add_out ADD(add_in) = 78;        /* Procedure nb */
  } = 1;                             /* Version nb */
} = 0x20001234;                      /* Program number */
