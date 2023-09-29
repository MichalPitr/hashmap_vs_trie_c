// #include <stdbool.h>

// typedef struct {
//     char* word;
//     int length;
// } WordWithLength;

// bool checkKeyword(int start, int length, int strLen, const char* str, const char* rest) {
//     if (strLen == start + length && 
//         // compare the rest of the string with currently parsed token.
//         memcmp(str[start], rest, length) == 0) {
//         return true;
//     } 
//     return false;
// }

// bool isKeyword(WordWithLength word) {
//     switch (word.word[0]) {
//         case 'a': return checkKeyword(1, 2, word.length, word.word, "nd");
//         case 'c': return checkKeyword(1, 4, word.length, word.word, "lass");
//         case 'e': return checkKeyword(1, 3, word.length, word.word,"lse");
//         case 'f':
//             // Make sure there is another letter 'f' 
//             // could be a variable name on its own.
//             if (scanner.current - scanner.start > 1) {
//                 switch (scanner.start[1]) {
//                     case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
//                     case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
//                     case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
//                 }
//             }
//             break;
//         case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
//         case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
//         case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
//         case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
//         case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
//         case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
//         case 't':
//             if (scanner.current - scanner.start > 1) {
//                 switch(scanner.start[1]) {
//                     case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
//                     case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
//                 }
//             }
//         case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
//         case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
//     }

//     return TOKEN_IDENTIFIER;
// }



// int main() {
//     char* words[] = {"if", "var", "myName", "formula", "for", "thispute"};

//     // 2. Create an array of such structs
//     WordWithLength arrayOfStructs[sizeof(words)/sizeof(words[0])];

//     // 3. Initialize each struct with a string and its length
//     for (int i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
//         arrayOfStructs[i].word = words[i];
//         arrayOfStructs[i].length = strlen(words[i]);
//     }

//     checkKeyword()

//     return 0;
// }
