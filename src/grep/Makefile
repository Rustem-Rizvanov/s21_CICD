OS := $(shell uname -s)
CC = gcc

ifeq ($(OS), Darwin)
	@FLAGS := -Wall -Wextra -Werror -std=c11
else
	@FLAGS := -Wall -Wextra -Werror -std=c11 -D_GNU_SOURCE
endif
all: clean s21_grep

s21_grep: s21_grep.c s21_grep.h
	@$(CC) $(FLAGS) s21_grep.c -o s21_grep

clean:
	@rm -f s21_grep

leaks: s21_grep
	@leaks --atExit -- ./s21_grep -benst test_file/all_flag_tests/*
	
cppcheck: s21_grep
	@cppcheck --enable=all --suppress=missingIncludeSystem . s21_grep.c s21_grep.h

check: style cppcheck

test: s21_grep
	@bash test.sh