#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static uint32_t hash(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i]; // xor
        hash *= 16777619; // scatter data around
    }
    return hash % HASHMAP_CAPACITY;
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
    uint32_t index = hash(key, length);
    HashNode* newNode = (HashNode*) malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->keyLength = strlen(newNode->key);
    newNode->value = value;
    newNode->next = map->table[index];
    map->table[index] = newNode;
}

TokenType hashMapGet(HashMap* map, const char* key, int length) {
    uint32_t index = hash(key, length);
    HashNode* node = map->table[index];
    while (node) {
        if (node->keyLength == length && strncmp(node->key, key, length) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return TOKEN_IDENTIFIER;
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
