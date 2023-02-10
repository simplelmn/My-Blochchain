#ifndef MY_STRING_H
#define MY_STRING_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_LINE 512

#ifndef STRUCT_STRING_ARRAY
#define STRUCT_STRING_ARRAY
typedef struct s_string_array{
    int size;
    char **array;
}string_array;
#endif

int my_size(const char *string);
string_array* my_split(char *string, char *delimiter);
char* my_readline(int fd);
int count_line(int fd);
int file_size(int fd);
int my_strcmp(const char* str1, const char* str2);
int my_atoi(const char *string);
char *int_to_char(int n);

#endif