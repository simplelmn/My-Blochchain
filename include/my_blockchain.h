#ifndef MY_BLOCKCHAIN_H
#define MY_BLOCKCHAIN_H
#include "../include/my_string.h"
#include <stdbool.h>
#define SUCCESS_MSG "OK\n"
#define EXIST_NODE "nok: this node already exists\n"
#define NOT_EXIST_NODE "nok: node doesn't exist\n"
#define NOT_EXIST_BLOCK "nok: block doesn't exist\n"
#define EXIST_BLOCK "nok: this block already exists\n"
#define ERROR_NID "nok: nid should be integer\n"
#define ERROR_CMD "nok: command not found\n"
#define QUIT_MSG "nok: Backing up blockchian...\n"

typedef struct Block_node{
    char *bid;
    struct Block_node *next;
}block_node;

typedef struct List_node{
    int nid;
    block_node *block;
    struct List_node *next;
}list_node;

typedef enum Cmd{
    no_cmd = 0,
    add_node,
    rm_node,
    add_block,
    rm_block,
    ls,
    ls_l,
    SYNC,
    quit,
}list_cmd;

int search_node(list_node **node, int nid);
int search_block(block_node **block, char *bid);
bool insert_node(list_node **node, int nid);
bool remov_all_nodes(list_node **node);
bool remove_node(list_node **node, char *nid, int *node_count);
bool insert_block(list_node **node, char *bid, char *nid);
bool remove_block(list_node **node, char *bid);
bool add_to_all(list_node **node, char *bid);
bool check_nid(const char *string);
bool not_command(list_cmd command);
enum Cmd check_command(string_array *commands);
bool help_is_sync(list_node **prev, list_node **next);
bool is_sync(list_node **node);
bool check_sync(list_node **node, int node_count);
void sync_blocks(list_node **prev, list_node **next);
void synchronize(list_node **node);
bool send_sync(list_node **node, int node_count);
void print_ls(list_node **head);
void print_ls_l(list_node **head);
void write_file(list_node **head, int fd);
void fill_node(list_node **node, char *line);

#endif