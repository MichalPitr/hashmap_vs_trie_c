#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hashmap.h"
#include "scanner.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
    HashMap keywords;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    initHashMap(&scanner.keywords);

    // populate hashmap with keywords!
    hashMapInsert(&scanner.keywords, "(", TOKEN_LEFT_PAREN);
    hashMapInsert(&scanner.keywords, ")", TOKEN_RIGHT_PAREN);
    hashMapInsert(&scanner.keywords, "{", TOKEN_LEFT_BRACE);
    hashMapInsert(&scanner.keywords, "}", TOKEN_RIGHT_BRACE);
    hashMapInsert(&scanner.keywords, ",", TOKEN_COMMA);
    hashMapInsert(&scanner.keywords, ".", TOKEN_DOT);
    hashMapInsert(&scanner.keywords, "-", TOKEN_MINUS);
    hashMapInsert(&scanner.keywords, "+", TOKEN_PLUS);
    hashMapInsert(&scanner.keywords, ";", TOKEN_SEMICOLON);
    hashMapInsert(&scanner.keywords, "/", TOKEN_SLASH);
    hashMapInsert(&scanner.keywords, "*", TOKEN_STAR);
    hashMapInsert(&scanner.keywords, "!", TOKEN_BANG);
    hashMapInsert(&scanner.keywords, "!=", TOKEN_BANG_EQUAL);
    hashMapInsert(&scanner.keywords, "=", TOKEN_EQUAL);
    hashMapInsert(&scanner.keywords, "==", TOKEN_EQUAL_EQUAL);
    hashMapInsert(&scanner.keywords, ">", TOKEN_GREATER);
    hashMapInsert(&scanner.keywords, ">=", TOKEN_GREATER_EQUAL);
    hashMapInsert(&scanner.keywords, "<", TOKEN_LESS);
    hashMapInsert(&scanner.keywords, "<=", TOKEN_LESS_EQUAL);
    hashMapInsert(&scanner.keywords, "and", TOKEN_AND);
    hashMapInsert(&scanner.keywords, "class", TOKEN_CLASS);
    hashMapInsert(&scanner.keywords, "else", TOKEN_ELSE);
    hashMapInsert(&scanner.keywords, "false", TOKEN_FALSE);
    hashMapInsert(&scanner.keywords, "for", TOKEN_FOR);
    hashMapInsert(&scanner.keywords, "fun", TOKEN_FUN);
    hashMapInsert(&scanner.keywords, "if", TOKEN_IF);
    hashMapInsert(&scanner.keywords, "nil", TOKEN_NIL);
    hashMapInsert(&scanner.keywords, "or", TOKEN_OR);
    hashMapInsert(&scanner.keywords, "print", TOKEN_PRINT);
    hashMapInsert(&scanner.keywords, "return", TOKEN_RETURN);
    hashMapInsert(&scanner.keywords, "super", TOKEN_SUPER);
    hashMapInsert(&scanner.keywords, "true", TOKEN_TRUE);
    hashMapInsert(&scanner.keywords, "var", TOKEN_VAR);
    hashMapInsert(&scanner.keywords, "while", TOKEN_WHILE);
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static char advance() {
    scanner.current++;
    // Using index with a pointer works as offset in C, so
    // this retrieves the element 1 item before that pointed to by current.
    return scanner.current[-1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;        
                }
                break;
            default:
                return;
        }
    }
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length && 
        // compare the rest of the string with currently parsed token.
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    } 
    return TOKEN_IDENTIFIER;
}

TokenType identifierTypeUsingHashMap() {
    int length = scanner.current - scanner.start;
    TokenType type = hashMapGet(&scanner.keywords, scanner.start, length);
    return type == TOKEN_EOF ? TOKEN_IDENTIFIER : type;
}

static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            // Make sure there is another letter 'f' 
            // could be a variable name on its own.
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch(scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    // Closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

static Token number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        // Consume '.' in fraction.
        advance();

        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '*': return makeToken(TOKEN_STAR);
        case '/': return makeToken(TOKEN_SLASH);
        case '!':
            return makeToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(
                match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(
                match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    return errorToken("Unexpected character.");
}
