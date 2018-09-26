#include <stdio.h>
#include <stdlib.h>

void foo( char *str[], int n ) {
	////sada
}

int main() {
	char * line = (char *)malloc(sizeof(char));
	char ** lines;
	char sym;
	int i = 0;
	while ( sym = getchar() != EOF ) {
		int j = 0;
		while ( (sym = getchar()) != '\n') {
			line[j] = sym;
			j++;
			line = (char *) realloc(line, j*sizeof(char));
		}
		line[j] = '\0';
		*lines[i] = (char *) malloc((j+1)*sizeof(char)); ///ss
		//printf("%s\n", line);
		free(line);
	}
	getchar();	
	return 0;
}