#ifndef CAT_S21_CAT_H_
#define CAT_S21_CAT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cat_specificator {
  int min_b;
  int min_e;
  int min_n;
  int min_s;
  int min_t;
  int min_v;
  int min_E;
  int min_T;
} cat_spec;

int open_file(FILE* file, char* argv, char* file_name, cat_spec spec_file);

#endif  // CAT_S21_CAT_H_