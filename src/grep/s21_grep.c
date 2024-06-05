#include "s21_grep.h"

int main(int argc, char *argv[]) {
  grep_spec spec_file = {0};
  FILE *file = NULL;
  char *buffer_reg = calloc(1000, sizeof(char));
  int count_name = 0;
  int exit = 0;
  int flag = 1;
  int regular_f_and_e = 2;
  for (int i = 1; i < argc; i++) {
    for (size_t j = 1; j < (strlen(*(argv + i))) && (argv[i][0] == '-') &&
                       (strlen(*(argv + i)) < 5);
         j++) {
      char menu = argv[i][j];
      if (argv[i][1] != '-') {
        switch (menu) {
          case 'e':
            spec_file.min_e = 1;
            break;
          case 'i':
            spec_file.min_i = 1;
            break;
          case 'v':
            spec_file.min_v = 1;
            break;
          case 'c':
            spec_file.min_c = 1;
            break;
          case 'l':
            spec_file.min_l = 1;
            break;
          case 'n':
            spec_file.min_n = 1;
            break;
          case 'f':
            spec_file.min_f = 1;
            break;
          case 'h':
            spec_file.min_h = 1;
            break;
          case 'o':
            spec_file.min_o = 1;
            break;
          case 's':
            spec_file.min_s = 1;
            break;
          default:
            exit = i;
            break;
        }
      }
    }
    if ((argv[i][0] != '-' && flag)) {
      if (spec_file.min_f == 1) {
        regular_f_and_e = 3;
        FILE *file_reg = NULL;
        file_reg = fopen(*(argv + i), "r");
        if (file_reg != NULL) {
          int len_reg = 255;
          fgets(buffer_reg, len_reg, file_reg);
          spec_file.reg_name = buffer_reg;
          flag = 0;
        } else {
          !spec_file.min_s
              ? printf("grep: %s: No such file or directory\n", *(argv + i))
              : spec_file.min_s;
          exit = 7;
          break;
        }
        fclose(file_reg);
      } else {
        spec_file.reg_name = (*(argv + i));
        strcat(spec_file.reg_name, "\0");
        flag = 0;
      }
    }
    if (argv[i][0] != '-' && flag == 0) {
      count_name++;
    }
  }
  if (exit == 0) {
    for (int i = regular_f_and_e; (i < argc); i++) {
      open_file(file, argv[i], spec_file, &count_name);
    }
  } else {
    if ((!spec_file.min_s) && (exit != 7)) {
      printf("grep: invalid option -%s\n", (*(argv + exit)));
      printf(
          "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
          "[-C[num]]\n");
      printf(
          "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
      printf(
          "\t[--context[=num]] [--directories=action] [--label] "
          "[--line-buffered]\n");
      printf("\t[--null] [pattern] [file ...]\n");
    }
  }
  free(buffer_reg);
  return 0;
}

void open_file(FILE *file, char *file_name, grep_spec spec_file,
               int *count_name) {
  file = fopen(file_name, "r");
  regex_t regular_value;
  size_t len = 0;
  int regflag = 0;
  int reti;
  int size_line = 1;
  int size_temp = 0;
  char temp_name[100] = {0};
  char *buffer = NULL;
  if (*count_name > 2 && (!spec_file.min_h)) {
    strcat(temp_name, file_name);
    strcat(temp_name, ":");
  }
  if ((file_name[0] != '-') && (strcmp(spec_file.reg_name, file_name))) {
    if (file != NULL) {
      while (getline(&buffer, &len, file) != -1) {
        regflag = (spec_file.min_i ? REG_ICASE : regflag);
        reti = regcomp(&regular_value, spec_file.reg_name, regflag);
        reti = regexec(&regular_value, buffer, 0, NULL, 0);
        regfree(&regular_value);
        spec_v_and_o(spec_file, buffer, temp_name, &reti, &size_line);
        if ((spec_file.min_c == 1) || (spec_file.min_o == 1)) {
          if (spec_file.min_v)
            size_temp = (reti ? size_temp + 1 : size_temp);
          else
            size_temp = (!reti ? size_temp + 1 : size_temp);
        }
        spec_file.min_l ? spec_file.min_l = (!reti ? 2 : spec_file.min_l)
                        : spec_file.min_l;
        size_line++;
      }
      free(buffer);
      if (spec_file.min_l) {
        size_temp = (size_temp > 0 ? 1 : 0);
        spec_file.min_c ? printf("%s%d\n", temp_name, size_temp)
                        : spec_file.min_c;
      } else {
        spec_file.min_c ? printf("%s%d\n", temp_name, size_temp)
                        : spec_file.min_c;
      }
      spec_file.min_l == 2 ? printf("%s\n", file_name) : spec_file.min_l;
    } else {
      !spec_file.min_s
          ? printf("grep: %s: No such file or directory\n", file_name)
          : spec_file.min_s;
    }
    fclose(file);
  }
}

void spec_v_and_o(grep_spec spec_file, char *buffer, char *temp_name, int *reti,
                  int *size_line) {
  if ((spec_file.min_c == 0) && (spec_file.min_l == 0)) {
    if (spec_file.min_n == 0) {
      if (spec_file.min_v) {
        *reti ? printf("%s%s", temp_name, buffer) : *reti;
      } else {
        if (!(*reti)) {
          !spec_file.min_o
              ? !(*reti) ? printf("%s%s", temp_name, buffer) : *reti
              : !spec_file.min_o;
          spec_file.min_o ? printf("%s%s\n", temp_name, spec_file.reg_name)
                          : spec_file.min_o;
        }
      }
    } else {
      if (spec_file.min_v) {
        *reti ? printf("%s%d:%s", temp_name, *size_line, buffer) : *reti;
      } else {
        if (!(*reti)) {
          !spec_file.min_o
              ? !(*reti) ? printf("%s%d:%s", temp_name, *size_line, buffer)
                         : *reti
              : !spec_file.min_o;
          spec_file.min_o
              ? printf("%s%d:%s\n", temp_name, *size_line, spec_file.reg_name)
              : spec_file.min_o;
        }
      }
    }
  }
}