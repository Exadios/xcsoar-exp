#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//------------------------------------------------------------------------------
int yyerror(const char *)
  {
  return 0;
  }

//------------------------------------------------------------------------------
int main(int argc, const char **)
  {
  setlocale (LC_ALL, "");
  return yyparse ();

  return 0;
  }
