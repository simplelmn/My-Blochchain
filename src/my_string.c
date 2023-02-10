#include "../include/my_string.h"
#include <stdlib.h>

int my_size(const char *string){
    int i = 0;
    while(string[i]) i++;
    return i;
}

string_array* my_split(char *string, char *delimiter){
    string_array *response = (string_array*)malloc(sizeof(string_array));
    int length[my_size(string) - 1], ind = 0, count = 0;
    for(int i = 0; string[i] != '\0'; i++){
        if(string[i] == delimiter[0]){
            length[ind++] = count;
            count = 0;
        }else{
            count++;
            if(i == my_size(string) - 1)
                length[ind++] = count;
        }
    }

    char **container = (char**)malloc(ind *sizeof(char*));
    int ii = 0, jj = 0;

    for(int i = 0; i < ind; i++)
        container[i] = (char*)malloc(length[i] * sizeof(char));

    for(int i = 0; string[i] != '\0'; i++){
        if(string[i] == delimiter[0]){
            container[ii++][jj] = '\0';
            jj = 0;
        }else{
            if(string[i] != ':' && string[i] != ','){
                container[ii][jj] = string[i];
                jj++;
            }
            if(i == my_size(string) - 1)
                container[ii++][jj] = '\0';
        }
    }

    response->size = ind;
    response->array = container;
    return response;
}

char* my_readline(int fd){
    char *line = (char*)malloc(MAX_LINE * sizeof(char));
    int i = 0;
    char ch;
    int j = 1;
    do{
        j = read(fd, &ch, 1);
        if(j != 0){
            if(ch != '\n'){
                line[i++] = ch;
            }else{
                line[i] = '\0';
                return line;
            }
        }else{
            line[i] = '\0';
            if(my_size(line) > 0){
                return line;
            }else{
                return NULL;
            }
        }
    }while(j);
    return NULL;
}

int count_line(int fd){
    int count = 0;
    char ch;
    int j;
    do{
        j = read(fd, &ch, 1);
        if(ch == '\n'){
            count++;
        }else{
            if(j == 0) count++;
        }
    }while(j);
    return count;
}

int file_size(int fd){
    int size = 0;
    char ch;
    while(read(fd, &ch, 1)) size++;
    return size;
}

int my_strcmp(const char* string_1, const char* string_2){
    for(int i = 0; string_1[i] != '\0' || string_2[i] != '\0'; i++){
        if(string_1[i] != string_2[i]) return -1;
        if(string_1[i] == '\0') return 0;
        if(string_2[i] == '\0') return -1;
    }
    return 0;
}

int my_atoi(const char *string){
    int ans = 0;
    for(int i = 0; string[i] != '\0'; i++)
        ans = ans * 10 + string[i] - '0';
    return ans;
}

char *int_to_char(int n){
    int m = n;
    int i = 0;
    while(m > 0){
        i++;
        m /= 10;
    }
    char *ans = (char*)malloc(i * sizeof(char));
    i--;
    while(n > 0){
        ans[i--] = n % 10 + '0';
        n /= 10;
    }
    return ans;
}