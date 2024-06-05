#ifndef GREP_S21_GREP_H_
#define GREP_S21_GREP_H_
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grep_specificator {
  int min_e;  // Индикатор использования регулярных выражений
  int min_i;  // Индикатор игнорирования регистра
  int min_v;  // Индикатор инверсии результатов поиска
  int min_c;  // Индикатор вывода количества строк
  int min_l;  // Индикатор вывода имен файлов с совпадениями
  int min_n;  // Индикатор вывода номеров строк
  int min_f;  // Индикатор использования файла с шаблоном
  int min_h;  // Индикатор подавления имен файлов в выводе
  int min_o;  // Индикатор вывода только совпавших частей строк
  int min_s;  // Индикатор подавления сообщений об ошибках
  char *reg_name;  // Имя файла с шаблоном
} grep_spec;

// void parse_arguments(int argc, char *argv[], grep_spec *spec_file, char
// **file_names, int *num_files); void process_files(grep_spec spec_file, char
// *file_names[], int num_files); void spec_v_and_o(grep_spec spec_file, char*
// buffer, char* temp_name, int *reti, int *size_line);

void open_file(FILE *file, char *file_name, grep_spec spec_file,
               int *count_name);
void spec_v_and_o(grep_spec spec_file, char *buffer, char *temp_name, int *reti,
                  int *size_line);

#endif  // GREP_S21_GREP_H_