%{
	#include <stdio.h>
	#include <ctype.h>

	int lines = 0;
	int words = 0;
	int spaces = 0;
	int characters = 0;
	int in_word = 0;
%}

%%
\n { 
	lines++; 
	characters++;  
	in_word = 0;
}

[ \t] {  
	spaces++;  
	characters++;  
	in_word = 0;
}

[a-zA-Z0-9_] {  
	characters++; 
	if (!in_word) {
		words++;  
		in_word = 1;
	} 
}

. {  
	characters++; 
}

%%

int main(void) {
	FILE *file = fopen("myfile.txt", "r");  
	if (!file) {
		perror("Error opening file");
		return 1;
	}
	yyin = file;
	yylex();  
	fclose(file);  

	printf("This file contains:\n");
	printf("\t%d lines\n", lines);
	printf("\t%d words\n", words);
	printf("\t%d spaces\n", spaces);  
	printf("\t%d characters\n", characters);  
	return 0;
}

int yywrap(void) {
	return 1;
}


// for output make a file name myfile.txt
// give input "Hello, world!
// This is a test.
// Lex is fun."
