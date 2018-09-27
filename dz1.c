#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

void foo( char *line, int n ) {
	//char * result = (char *)malloc(n*sizeof(char));
	//memcpy(result, line, n*sizeof(char));
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
	printf("result = %s\n", result);
	return;
}

int main() {
	while ( 1 == 1 ) {
		char * line = (char *)malloc(sizeof(char));
		char sym;
		int i = 0;
		
		while ( (sym = getchar()) != '\n') {
			line[i] = sym;
			i++;
			line = (char *) realloc(line, i*sizeof(char));
		}
		line[i] = '\0';
		printf("input  = %s\n", line);
		foo(line, i);
		free(line);
	}
	
	getchar();	
	return 0;
}