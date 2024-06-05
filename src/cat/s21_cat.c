#include "s21_cat.h"

int main(int argc, char* argv[]) {
  cat_spec spec_file = {0};
  FILE* file = NULL;
  char* file_name;
  int step_exit = 0;
  int exit = 0;
  for (int i = 1; i < argc; i++) {
    for (int j = 1; j < (strlen(*(argv + i))) && (argv[i][0] == '-'); j++) {
      char menu = argv[i][j];
      if (argv[i][1] != '-') {
        switch (menu) {
          case 'b':
            spec_file.min_b = 1;
            break;  // Номера строк с непустыми строками
          case 'e':
            spec_file.min_e = 1;
            break;  // Показывать символ доллара в конце строк
          case 'n':
            spec_file.min_n = 1;
            break;  // Номера всех строк
          case 's':
            spec_file.min_s = 1;
            break;  // Удалять пустые строки
          case 't':
            spec_file.min_t = 1;
            break;  // Показывать знак табуляции вместо символа табуляции
          case 'v':
            spec_file.min_v = 1;
            break;  // Показывать непечатаемые символы
          case 'T':
            spec_file.min_T = 1;
            break;  // Номера символов табуляции в начале строки
          case 'E':
            spec_file.min_E = 1;
            break;  // Показывать доллар в конце строк
          default:
            exit = i;
            break;  // Неизвестная опция, сохраняем индекс аргумента
        }
      } else if (argv[1][0] == '-' && argv[1][1] == '-') {
        if (!strcmp("--number-nonblank", argv[1])) {
          spec_file.min_b = 1;
        } else if (!strcmp("--number", argv[1])) {
          spec_file.min_n = 1;
        } else if (!strcmp("--squeeze-blank", argv[1])) {
          spec_file.min_s = 1;
        } else {
          printf(
              "cat: illegal option -- -\nusage: cat [-benstuv] [file ...]\n");
          exit = 0;
        }
      }
      spec_file.min_n = (spec_file.min_b == 1 ? 0 : spec_file.min_n);
      spec_file.min_b = (spec_file.min_n == 1 ? 0 : spec_file.min_b);
    }
  }

  if (exit == 0) {
    for (int i = 1; i < argc; i++) {
      open_file(file, *argv, argv[i], spec_file);
    }
  } else {
    printf("s21_cat: illegal option -- %c\n", argv[exit][1]);
    printf("usage: cat [-benstuv] [file ...]\n");
  }

  return 0;
}

int open_file(FILE* file, char* argv, char* file_name, cat_spec spec_file) {
  unsigned char buffer;
  int line = 1;
  int flag = 0;
  int flag_s = 0;
  if (file_name[0] != '-') {
    file = fopen(file_name, "rt");
    if (file && (buffer = getc(file)) != -1) {
      while (!feof(file) && !ferror(file)) {
        if (buffer != EOF) {
          flag_s = (buffer != '\n' ? 0 : flag_s);
          if (spec_file.min_s && (buffer == '\n')) {
            flag_s++;
            flag_s == 3 ? buffer = '/' : flag_s;
          }
          if (((spec_file.min_b) && (flag == 0) && (buffer != '\n')) ||
              (spec_file.min_n) && (flag == 0)) {
            printf("%6d\t", line);
            line++;
            flag = 1;
          }
          if (spec_file.min_E) {
            buffer == '\n' ? printf("$") : buffer;
          }
          if (spec_file.min_e) {
            buffer == '\n' ? printf("$") : buffer;
            spec_file.min_v = 1;
          }
          if (spec_file.min_v || spec_file.min_t) {
            if (buffer < 9 || (buffer > 10 && buffer < 32)) {
              printf("^");
              printf("%c", buffer + 64);
            } else if (spec_file.min_t && buffer == '\t') {
              printf("^");
              buffer = 'I';
            } else if (buffer == 127) {
              printf("^?");
            } else if (buffer > 127 && buffer < 160) {
              printf("M-^%c", buffer - 64);
#if defined(__linux__)
            } else if (buffer > 159 && buffer < 255) {
              printf("M-%c", buffer - 128);
            } else if (buffer == 255) {
              printf("M-^?");
#endif
            }
          }
          if (spec_file.min_t && buffer == '\t') {
            printf("^");
            buffer = 'I';
          }
          flag_s < 3 ? putchar(buffer) : flag_s;
          flag = (buffer == '\n' ? 0 : 1);
          buffer = getc(file);
        }
      }
      fclose(file);
    } else {
      printf("s21_cat: te: No such file or directory\n");
    }
  }
  return 0;
}