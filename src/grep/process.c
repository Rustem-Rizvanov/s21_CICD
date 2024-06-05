#include "s21_grep.h"

void process_files(grep_spec spec_file, char *file_names[], int num_files) {
  FILE *file = NULL;
  regex_t regular_value;
  int size_line = 1;
  int reti;
  char temp_name[100] = {0};
  char *buffer = NULL;
  int regflag = 0;

  for (int i = 0; i < num_files; i++) {
    file = fopen(file_names[i], "r");
    if (file == NULL) {
      if (!spec_file.min_s) {
        printf("grep: %s: No such file or directory\n", file_names[i]);
      }
    } else {
      if (num_files > 2 && !spec_file.min_h) {
        strcat(temp_name, file_names[i]);
        strcat(temp_name, ":");
      }
      while (getline(&buffer, &(size_t){0}, file) != -1) {
        regflag = (spec_file.min_i ? REG_ICASE : 0);
        reti = regcomp(&regular_value, spec_file.reg_name, regflag);
        reti = regexec(&regular_value, buffer, 0, NULL, 0);
        regfree(&regular_value);
        spec_v_and_o(spec_file, buffer, temp_name, &reti, &size_line);
        if (spec_file.min_c || spec_file.min_o) {
          size_line = (!reti && !spec_file.min_v) ? size_line + 1 : size_line;
        }
        if (spec_file.min_l && !reti) {
          printf("%s\n", temp_name);
          break;
        }
        size_line++;
      }
      fclose(file);
      if (buffer) free(buffer);
    }
  }
}

void spec_v_and_o(grep_spec spec_file, char *buffer, char *temp_name, int *reti,
                  int *size_line) {
  if (!spec_file.min_c && !spec_file.min_l) {
    if (!spec_file.min_n) {
      if (spec_file.min_v) {
        if (*reti) {
          printf("%s%s", temp_name, buffer);
        }
      } else {
        if (!*reti && !spec_file.min_o) {
          printf("%s%s", temp_name, buffer);
        }
        if (spec_file.min_o && !*reti) {
          printf("%s%s\n", temp_name, spec_file.reg_name);
        }
      }
    } else {
      if (spec_file.min_v) {
        if (*reti) {
          printf("%s%d:%s", temp_name, *size_line, buffer);
        }
      } else {
        if (!*reti && !spec_file.min_o) {
          printf("%s%d:%s", temp_name, *size_line, buffer);
        }
        if (spec_file.min_o && !*reti) {
          printf("%s%d:%s\n", temp_name, *size_line, spec_file.reg_name);
        }
      }
    }
  }
}
