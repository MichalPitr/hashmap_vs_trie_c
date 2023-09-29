#ifndef HASHMAP_H
#define HASHMAP_H

#include "scanner.h"  // For TokenType

#define HASHMAP_CAPACITY 100

typedef struct HashNode {
    char* key;
    TokenType value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* table[HASHMAP_CAPACITY];
} HashMap;

// Initializes the hashmap.
void initHashMap(HashMap* map);

// Inserts a key-value pair into the hashmap.
void hashMapInsert(HashMap* map, const char* key, TokenType value);

// Fetches the value associated with a given key from the hashmap.
// Returns `TOKEN_EOF` if key doesn't exist.
TokenType hashMapGet(HashMap* map, const char* key, int length);

// Frees resources associated with the hashmap.
void freeHashMap(HashMap* map);

#endif