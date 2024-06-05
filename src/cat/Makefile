OS := $(shell uname -s)
CC = gcc

ifeq ($(OS), Darwin)
	@FLAGS := -Wall -Wextra -Werror -std=c11
else
	@FLAGS := -Wall -Wextra -Werror -std=c11 -D_GNU_SOURCE
endif
all: clean s21_cat

s21_cat: s21_cat.c s21_cat.h
	@$(CC) $(FLAGS) s21_cat.c -o s21_cat

clean:
	@rm -f s21_cat

leaks: s21_cat
	@leaks --atExit -- ./s21_grep -benst test_file/all_flag_tests/*

cppcheck: s21_cat
	@cppcheck --enable=all --suppress=missingIncludeSystem . s21_cat.c s21_cat.h

check: style cppcheck

test: s21_cat
	@bash test_cat.sh
