#include "s21_grep.h"

void parse_arguments(int argc, char *argv[], grep_spec *spec_file,
                     char **file_names, int *num_files) {
  int flag = 1;
  int regular_f_and_e = 2;

  for (int i = 1; i < argc; i++) {
    for (size_t j = 1;
         j < strlen(argv[i]) && argv[i][0] == '-' && strlen(argv[i]) < 5; j++) {
      char menu = argv[i][j];
      if (argv[i][1] != '-') {
        switch (menu) {
          case 'e':
            spec_file->min_e = 1;
            break;
          case 'i':
            spec_file->min_i = 1;
            break;
          case 'v':
            spec_file->min_v = 1;
            break;
          case 'c':
            spec_file->min_c = 1;
            break;
          case 'l':
            spec_file->min_l = 1;
            break;
          case 'n':
            spec_file->min_n = 1;
            break;
          case 'f':
            spec_file->min_f = 1;
            break;
          case 'h':
            spec_file->min_h = 1;
            break;
          case 'o':
            spec_file->min_o = 1;
            break;
          case 's':
            spec_file->min_s = 1;
            break;
          default:
            exit(i);
            break;
        }
      }
    }
    if (argv[i][0] != '-' && flag) {
      if (spec_file->min_f == 1) {
        regular_f_and_e = 3;
        FILE *file_reg = fopen(argv[i], "r");
        if (file_reg != NULL) {
          int len_reg = 255;
          fgets(spec_file->reg_name, len_reg, file_reg);
          fclose(file_reg);
        } else {
          if (!spec_file->min_s) {
            printf("grep: %s: No such file or directory\n", argv[i]);
          }
          exit(7);
        }
      } else {
        spec_file->reg_name = argv[i];
        strcat(spec_file->reg_name, "\0");
      }
      flag = 0;
    }
    if (argv[i][0] != '-' && flag == 0) {
      file_names[*num_files] = argv[i];
      (*num_files)++;
    }
  }

  if (*num_files == 0) {
    fprintf(stderr, "grep: no files to search\n");
    exit(1);
  }
}
