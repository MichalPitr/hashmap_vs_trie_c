#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"
#include "scanner.h"

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    // Seek end of file to compute the file size.
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file); // move to the start of the file

    // Allocates a memory chunk of size filesize+1 (for null byte) and 
    // returns the pointer to it. Since we'll be storing chars in it, we cast the pointer to
    // char* 
    char* buffer = (char*)malloc(fileSize+1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if(bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}


// Just a driver that scans the next token and assigns it to var passed by user.
static void advance(Token* token) {
    for (;;) {
        *token = scanToken();
        if (token->type != TOKEN_ERROR) break;
    }
}

static void runFile(const char* path) {
    char* source = readFile(path);
    initScanner(source);
    // Prime the scanner.
    Token token;
    while (token.type != TOKEN_EOF) {
        advance(&token);
        printf("token: %d\n", token.type);
    }

    // readfile allocates memory and passes freeing it to us.
    free(source);
}

int main(int argc, const char* argv[]) {
    if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }
    
    return 0;
}
