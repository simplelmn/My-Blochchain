#include "../include/blockchain.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool new_blockchain(int fd){
    printf("No Backup Found: Starting New Blockchain\n");
    list_node *node = NULL;
    char *input =(char*)malloc(MAX_LINE * sizeof(char));
    string_array *commands = (string_array*)malloc(100 * sizeof(string_array));
    int node_count = 0, in = 1;
    char s = 's';
    do{
        printf("[%c%d]>", s, node_count);
        fflush(stdout);
        input = my_readline(0);
        if(input == NULL) return NULL;
        commands = my_split(input, " ");
        list_cmd cmds = check_command(commands); 
        not_command(cmds);
        switch(cmds){
            case add_node:
                if(insert_node(&node, my_atoi(commands->array[2]))){
                    node_count++;
                    s = !check_sync(&node, node_count) ? '-' : 's';
                    printf(SUCCESS_MSG);
                    fflush(stdout);
                }else{
                    printf(EXIST_NODE);
                }
            break;
            case rm_node:
                if(my_strcmp(commands->array[2], "*") == 0){
                    remov_all_nodes(&node);
                    node_count = 0;
                    fflush(stdout);
                }
                else{
                    if(remove_node(&node, commands->array[2], &node_count)){
                        s = !check_sync(&node, node_count) ? '-' : 's';
                        printf(SUCCESS_MSG);
                        fflush(stdout);
                    }else{
                        printf(NOT_EXIST_NODE);
                    }
                }
            break;
            case add_block:
                if(my_strcmp(commands->array[3], "*") == 0){
                    add_to_all(&node, commands->array[2]);
                    printf(SUCCESS_MSG);
                    fflush(stdout);
                }else{
                    if(insert_block(&node, commands->array[2], commands->array[3])){
                        s = !check_sync(&node, node_count) ? '-' : 's';
                        printf(SUCCESS_MSG);
                        fflush(stdout);
                    }else{
                        printf(EXIST_BLOCK);
                    }
                }
            break;
            case rm_block:
                if(remove_block(&node, commands->array[2])){
                    s = !check_sync(&node, node_count) ? '-' : 's';
                    printf(SUCCESS_MSG);
                    fflush(stdout);
                }else{
                    printf(NOT_EXIST_BLOCK);
                }
            break;
            case ls:
                print_ls(&node);
            break;
            case ls_l:
                print_ls_l(&node);
            break;
            case SYNC:
                send_sync(&node, node_count);
                s = 's';
                printf(SUCCESS_MSG);
            break;
            case quit:
                printf(QUIT_MSG);
                write_file(&node, fd);
                in = 0;
            break;
            case no_cmd:
                printf(ERROR_CMD);
            break;
        }
    }while(in);
   
    fflush(stdout);
    return true;
}

bool restore_blockchain(int fd){
    printf("Restoring From Backup\n");
    list_node *node = NULL;
    int node_count = 0;
    char *line = (char*)malloc(MAX_LINE * sizeof(char));
    while((line = my_readline(fd))){
        fill_node(&node, line);
        node_count++;
    }
    free(line);
    close(fd);
    fd = open("DATA", O_WRONLY |O_TRUNC | O_APPEND);
     char *input =(char*)malloc(MAX_LINE * sizeof(char));
     string_array *commands = (string_array*)malloc(100 * sizeof(string_array));
     int in = 1;
     char s = 's';
    do{
        printf("[%c%d]>", s, node_count);
        fflush(stdout);
        input = my_readline(0);
        if(input == NULL) return NULL;
        commands = my_split(input, " ");
        list_cmd cmds = check_command(commands);
        not_command(cmds);
        switch(cmds){
            case add_node:
                if(insert_node(&node, my_atoi(commands->array[2]))){
                    node_count++;
                    s = !check_sync(&node, node_count) ? '-' : 's';
                    printf(SUCCESS_MSG);
                    fflush(stdout);
                }else{
                    printf(EXIST_NODE);
                }
            break;
            case rm_node:
                if(my_strcmp(commands->array[2], "*") == 0){
                    remov_all_nodes(&node);
                    node_count = 0;
                    fflush(stdout);
                }
                else{
                    if(remove_node(&node, commands->array[2], &node_count)){
                        s = !check_sync(&node, node_count) ? '-' : 's';
                        printf(SUCCESS_MSG);
                        fflush(stdout);
                    }else{
                        printf(NOT_EXIST_NODE);
                    }
                }
            break;
            case add_block:
                if(my_strcmp(commands->array[3], "*") == 0){
                    add_to_all(&node, commands->array[2]);
                    printf(SUCCESS_MSG);
                    fflush(stdout);
                }else{
                    if(insert_block(&node, commands->array[2], commands->array[3])){
                        s = !check_sync(&node, node_count) ? '-' : 's';
                        printf(SUCCESS_MSG);
                        fflush(stdout);
                    }else{
                        printf(EXIST_BLOCK);
                    }
                }
            break;
            case rm_block:
                if(remove_block(&node, commands->array[2])){
                    s = !check_sync(&node, node_count) ? '-' : 's';
                    printf(SUCCESS_MSG);
                    fflush(stdout);
                }else{
                    s = !check_sync(&node, node_count) ? '-' : 's';
                    printf(NOT_EXIST_BLOCK);
                }
            break;
            case ls:
                print_ls(&node);
            break;
            case ls_l:
                print_ls_l(&node);
            break;
            case SYNC:
                send_sync(&node, node_count);
                s = 's';
                printf(SUCCESS_MSG);
            break;
            case quit:
                printf(QUIT_MSG);
                write_file(&node, fd);
                in = 0;
            break;
            case no_cmd:
                printf(ERROR_CMD);
            break;
        }
    }while(in);

    fflush(stdout);
    return true;
}

void my_blockchain(){
    int fd = open("DATA", O_RDONLY | O_CREAT, 0000644);
    int size = file_size(fd);
    close(fd);
    if(size == 0){
        fd = open("DATA", O_WRONLY | O_APPEND);
        new_blockchain(fd);
        close(fd);
    }else{
        fd = open("DATA", O_RDONLY);
        restore_blockchain(fd);
        close(fd);
    }
}