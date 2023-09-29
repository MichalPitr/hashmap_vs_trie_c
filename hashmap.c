#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static unsigned int hash(const char* key, int length) {
    unsigned int hashValue = 0;
    for (int i = 0; i < length; i++) {
        hashValue = (hashValue << 5) + key[i];
    }
    return hashValue % HASHMAP_CAPACITY;
}

void initHashMap(HashMap* map) {
    for (int i = 0; i < HASHMAP_CAPACITY; i++) {
        map->table[i] = NULL;
    }
}

void hashMapInsert(HashMap* map, const char* key, TokenType value) {
    int length = 0;
    while (key[length] != '\0') {
        length++;
    }
    unsigned int index = hash(key, length);
    HashNode* newNode = (HashNode*) malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = map->table[index];
    map->table[index] = newNode;
}

TokenType hashMapGet(HashMap* map, const char* key, int length) {
    unsigned int index = hash(key, length);
    HashNode* node = map->table[index];
    while (node) {
        if (strlen(node->key) == length && strncmp(node->key, key, length) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return TOKEN_EOF;
}

void freeHashMap(HashMap* map) {
    for (int i = 0; i < HASHMAP_CAPACITY; i++) {
        HashNode* node = map->table[i];
        while (node) {
            HashNode* next = node->next;
            free(node->key);
            free(node);
            node = next;
        }
    }
}
