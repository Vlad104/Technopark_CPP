#include <stdio.h>
#include <stdlib.h>

char * foo( const char * line, int n ) {
	char * result = (char *)malloc(sizeof(char));
	int i = 0, j =0;
	while (i < n) {
		if ( i > 0 && line[i] == ' ' && line[i-1] == ' ') {
			i++;
		}
		else {
			result = (char *)realloc(result, (j+1)*sizeof(char));
			result[j] = line[i];
			i++;
			j++;
		}
	}
	result[j] = '\0';
	return result;
}

int main() {
	while ( 1 == 1 ) {

		char * line = (char *)malloc(sizeof(char));
		char sym;
		int i = 0;
		
		while ( (sym = getchar()) != '\n') {
			line = (char *) realloc(line, (i+1)*sizeof(char));
			line[i] = sym;
			i++;
		}
		
		/*
		char * line;
		scanf("%[^\n]%s", line);
		int i = sizeof(line) / sizeof(char);
		*/
		
		line[i] = '\0';
		printf("input  = %s\n", line);
		char * result = foo(line, i);
		printf("result = %s\n", result);
		free(line);
		free(result);
	}
	
	getchar();	
	return 0;
}