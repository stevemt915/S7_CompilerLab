#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// List of keywords
int isKeyword(char buffer[]) {
    char keywords[32][10] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    for (int i = 0; i < 32; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            return 1;
        }
    }

    return 0;
}

// Function to check if the word is a constant (Integer or Float)
int isConstant(char buffer[]) {
    int i = 0, hasDot = 0;

    if (!isdigit(buffer[0]))  // Constants must start with a digit
        return 0;

    for (i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '.') {
            if (hasDot) return 0;  // Only one dot is allowed
            hasDot = 1;
        } else if (!isdigit(buffer[i])) {
            return 0;  // Not a digit, invalid constant
        }
    }

    return 1;
}

int main() {
    char ch, buffer[100], operators[] = "+-*/%=<>!", separators[] = ";:(){}", temp;
    FILE *fp;
    int i, j = 0;

    fp = fopen("lex.txt", "r");  // Open file for reading
    if (fp == NULL) {
        printf("Error while opening the file\n");
        exit(0);
    }

    while ((ch = fgetc(fp)) != EOF) {

        // Skip single-line comments
        if (ch == '/') {
            temp = fgetc(fp);
            if (temp == '/') {  // Single-line comment
                while ((ch = fgetc(fp)) != EOF && ch != '\n');
                continue;
            }
            // Handle multi-line comments
            else if (temp == '*') {  // Multi-line comment
                while (1) {
                    ch = fgetc(fp);
                    if (ch == EOF) break;
                    if (ch == '*') {
                        temp = fgetc(fp);
                        if (temp == '/') break;
                        else ungetc(temp, fp);  // Push back the character if it's not '/'
                    }
                }
                continue;
            } else {
                ungetc(temp, fp);  // Not a comment, put back the character
            }
        }

        // Check for operators (including >=, <=, ==, !=)
        for (i = 0; i < strlen(operators); ++i) {
            if (ch == operators[i]) {
                char next = fgetc(fp);
                if ((ch == '>' || ch == '<' || ch == '=' || ch == '!') && next == '=') {
                    printf("%c%c is operator\n", ch, next);  // Print combined operator (>=, <=, ==, !=)
                } else {
                    printf("%c is operator\n", ch);  // Print single operator
                    ungetc(next, fp);  // Push back the character
                }
                break;
            }
        }

        // Check for separators (punctuation characters)
        for (i = 0; i < strlen(separators); ++i) {
            if (ch == separators[i]) {
                printf("%c is separator\n", ch);  // Print separator
                break;
            }
        }

        // Collect characters for identifier, keyword, or constant
        if (isalnum(ch) || (ch == '.' && j > 0 && isdigit(buffer[j - 1]))) {
            buffer[j++] = ch;  // Append character to buffer
        }
        // If end of token (whitespace or non-alphanumeric), process the buffer
        else if ((ch == ' ' || ch == '\n' || ch == '\t') && j != 0) {
            buffer[j] = '\0';  // Null-terminate the token
            j = 0;  // Reset the buffer index
            if (isKeyword(buffer))
                printf("%s is keyword\n", buffer);  // Print keyword
            else if (isConstant(buffer))
                printf("%s is constant\n", buffer);  // Print constant
            else
                printf("%s is an identifier\n", buffer);  // Print identifier
        }
        // If the token ends abruptly due to a special character (i.e., it's not alphanumeric or a dot)
        else if (j != 0 && !isalnum(ch) && ch != '.') {
            buffer[j] = '\0';  // Null-terminate the token
            j = 0;  // Reset the buffer index
            if (isKeyword(buffer))
                printf("%s is keyword\n", buffer);  // Print keyword
            else if (isConstant(buffer))
                printf("%s is constant\n", buffer);  // Print constant
            else
                printf("%s is an identifier\n", buffer);  // Print identifier
        }
    }

    fclose(fp);  // Close the file
    return 0;
}




