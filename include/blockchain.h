#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include "../include/my_blockchain.h"
bool new_blockchain(int fd);
bool restore_blockchain(int fd);
void my_blockchain();
#endif