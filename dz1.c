#include <stdio.h>
#include <stdlib.h>

void foo( char *str[], int n ) {
	////sada
}

int main() {
	char * line = (char *)malloc(sizeof(char));
	char sym;
	int i = 0;
	while ( (sym = getchar()) != '\n') {
		line[i] = sym;
		i++;
		line = (char *) realloc(line, i*sizeof(char));
	}
	line[i] = '\0';
	printf("%s\n", line);
	free(line);
	getchar();	
	return 0;
}