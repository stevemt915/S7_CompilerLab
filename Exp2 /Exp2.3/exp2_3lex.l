%{
#include<stdio.h>
#include<ctype.h>
%}

%%
abc   { printf("ABC"); }
.     { putchar(yytext[0]); }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter the String: ");
    printf("\nThe text to be converted: ");
    yylex();   // Will process the input and print converted output
    printf("\n");
    return 0;
}
