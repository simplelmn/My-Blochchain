#include "../include/my_blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search_node(list_node **node, int nid){
    list_node *current = *node;
    int index = 0;
    while(current != NULL){
        if(current->nid == nid)
            return index;
        current = current->next;
        index++;
    }
    return -1;
}

int search_block(block_node **block, char *bid){
    block_node *current = *block;
    int index = 0;
    while(current != NULL){
        if(my_strcmp(current->bid, bid) == 0)
            return index;
        current = current->next;
        index++;
    }
    return -1;
}

bool insert_node(list_node **node, int nid){
    list_node *new_node = (list_node*)malloc(sizeof(list_node));
    list_node *last = *node;
    block_node *block = NULL;
    new_node->nid = nid;
    new_node->next = NULL;

    if(*node == NULL){
        *node = new_node;
        (*node)->block = block;
        return true;
    }

    if(last->next == NULL && last->nid == nid){
        return false;
    }
    while(last->next != NULL){
        if(search_node(&last, nid) >= 0){
            return false;
        }
        last = last->next;
    }

    last->next = new_node;
    new_node->block = block;
    return true;
}

bool remov_all_nodes(list_node **node){
    list_node *current = *node;
    list_node *del = current;
    *node = current->next;
    free(del);
    if(*node != NULL)
        remov_all_nodes(node);
    else
        printf(SUCCESS_MSG);
    return true;
}

bool remove_node(list_node **node, char *nid, int *node_count){
    list_node *current = *node;
    list_node *del = current;
    int index = search_node(node, my_atoi(nid));
    if(index == -1){
        return false;
    }
    if(index == 0){
        *node = current->next;
        free(del);
        (*node_count)--;
        return true;
    }
    for(int i = 0; i < index - 1; i++){
        current = current->next;
    }
    del = current->next;
    current->next = current->next->next;
    del->next = NULL;
    free(del);
    (*node_count)--;
    return true;
}

bool insert_block(list_node **node, char *bid, char *nid){
    block_node *new_block = (block_node*)malloc(sizeof(block_node));
    list_node *last_node = *node;
    new_block->bid = bid;
    new_block->next = NULL;
    int index = search_node(node, my_atoi(nid));
    while(index > 0){
        last_node = last_node->next;
        index--;
    }
    block_node *last_block = last_node->block;
    if(last_block == NULL){
        last_node->block = new_block;
        return true;
    }else{
        if(search_block(&last_block, bid) >= 0){
            return false;
        }
    }
    while(last_block->next != NULL)
        last_block = last_block->next;
    last_block->next = new_block;
     return true;
}

bool remove_block(list_node **node, char *bid){
    list_node *current = *node;
    if(*node == NULL)
        return false;

    while(current != NULL){
        int index = search_block(&current->block, bid);
        if(index == -1){
            printf("%s doesn't exist inside %d\n", bid, current->nid);
            return false;
        }
        block_node *block = current->block;
        if(index == 0){
            current->block = current->block->next;
            block->next = NULL;
            free(block);
        }else{
            for(int i = 1; i < index; i++){
                block = block->next;
            }
            if(index != -1){
                block_node *del = block->next;
                block->next = block->next->next;
                del->next = NULL;
                free(del);
            }
        }
        current = current->next;
    }
    return true;
}

bool add_to_all(list_node **node, char *bid){
    list_node *temp = *node;
    while(temp != NULL){
        insert_block(&temp, bid, int_to_char(temp->nid));
        temp = temp->next;
    }
    return true;
}

bool check_nid(const char *string){
    for(int i = 0; string[i] != '\0'; i++)
        if(string[i] < '0' || string[i] > '9'){
            printf(ERROR_NID);
            return false;
        }
    return true;
}
bool not_command(list_cmd command){
    if(command == no_cmd)
        return false;
    return true;
}

enum Cmd check_command(string_array *commands){
    if(commands->size == 1){
        if(my_strcmp(commands->array[0], "ls") == 0){
            return ls;
        } 
        if(my_strcmp(commands->array[0], "sync") == 0){
            return SYNC;
        } 
        if(my_strcmp(commands->array[0], "quit") == 0){
            return quit;
        }
        }else if(commands->size >= 2){
        if(my_strcmp(commands->array[0], "add") == 0 && my_strcmp(commands->array[1], "node") == 0 && check_nid(commands->array[2])){
           return add_node;
        }
        if(my_strcmp(commands->array[0], "add") == 0 && my_strcmp(commands->array[1], "block") == 0 && commands->size == 4){
           return add_block;
        }
        if(my_strcmp(commands->array[0], "rm") == 0 && my_strcmp(commands->array[1], "node") == 0){
           return rm_node;
        }
        if(my_strcmp(commands->array[0], "rm") == 0 && my_strcmp(commands->array[1], "block") == 0){
            return rm_block;
        }
        if(my_strcmp(commands->array[0], "ls") == 0 && my_strcmp(commands->array[1], "-l") == 0){
           return ls_l;
        }
    }
   return no_cmd;
}


bool help_is_sync(list_node **prev, list_node **next){
    list_node *p = *prev;
    list_node *n = *next;
    block_node *block = n->block;
    while(block != NULL){
        int index = search_block(&p->block, block->bid);
        if(index == -1)
            return false;
        block = block->next;
    }
    block_node *b = p->block;
    while(b != NULL){
        int index = search_block(&n->block, b->bid);
        if(index == -1)
            return false;
        b = b->next;
    }
    return true;
}

bool is_sync(list_node **node){
    list_node *first = *node;
    list_node *second = (*node)->next;
    while(first != NULL){
        if(!help_is_sync(&first, &second))
            return false;
        first = first->next;
    }
    return true;
}

bool check_sync(list_node **node, int node_count){
    if(node_count == 0 || node_count == 1)
        return true;
    while(node_count > 0){
        if(!is_sync(node))
            return false;
        node_count--;
    }
    return true;
}

void sync_blocks(list_node **prev, list_node **next){
    list_node *p = *prev;
    list_node *n = *next;
    block_node *block = n->block;
    while(block != NULL){
        int index = search_block(&p->block, block->bid);
        if(index == -1){
            insert_block(&p, block->bid, int_to_char(p->nid));
        }
        block = block->next;
    }
    block_node *block2 = p->block;
    while(block2 != NULL){
        int index = search_block(&n->block, block2->bid);
        if(index == -1){
            insert_block(&n, block2->bid, int_to_char(n->nid));
        }
        block2 = block2->next;
    }
}

void synchronize(list_node **node){
    list_node *first = *node;
    list_node *second = (*node)->next;
    while(first != NULL){
        sync_blocks(&first, &second);
        first = first->next;
    }
}

bool send_sync(list_node **node, int node_count){
    while(node_count > 0){
        synchronize(node);
        node_count--;
    }
    return true;
}

void print_ls(list_node **head){
    list_node *node = *head;
    while(node != NULL){
        printf("%d\n", node->nid);
        node = node->next;
    }
}

void print_ls_l(list_node **head){
    list_node *node = *head;
    while(node != NULL){
        printf("%d:", node->nid); 
        block_node *temp = node->block;
        if(temp != NULL)
            printf(" ");
        while(temp != NULL){
            printf("%s", temp->bid);
            if(temp->next != NULL) printf(", ");
            temp = temp->next;
        }
        node = node->next;
        printf("\n");
    }
}

void write_file(list_node **head, int fd){
    list_node *node = *head;
    while(node != NULL){
        write(fd, int_to_char(node->nid), my_size(int_to_char(node->nid)));
        write(fd, ": ", 2);
        block_node *block = node->block;
        while(block != NULL){
            write(fd, block->bid, my_size(block->bid));
            if(block->next != NULL){
                write(fd, ", ", 2);
            }
            block = block->next;
        }
        if(node->next != NULL)
            write(fd, "\n", 1);
        node = node->next;
    }
}

void fill_node(list_node **node, char *line){
    string_array *nid_bid = (string_array*)malloc(sizeof(string_array));
    nid_bid = my_split(line, " ");
    insert_node(node, my_atoi(nid_bid->array[0]));
    for(int i = 1; i < nid_bid->size; i++){
        insert_block(node, nid_bid->array[i], nid_bid->array[0]);
    }
}