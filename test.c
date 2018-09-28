#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>



int main(int argc, char *argv[]) {
	char * line = NULL;
	size_t size = 0;
	ssize_t bytes = getline(&line, &size, stdin);

	printf("%ld  %ld", bytes, size);
	free(line);
	return 0;
}

/*
	char ** lines = (char **)malloc(sizeof(char *));
	if (errno == ENOMEM) {
		printf("[error]");
		return 0;
	}
	char symbol = getchar();
	int str_num = 0;
	while ( symbol != EOF ) {
		lines[str_num] = (char *)malloc(sizeof(char));
		if (errno == ENOMEM) {
			printf("[error]");
			return 0;
		}
		
		int str_len = 0;
		while ( symbol != '\n') {
			lines[str_num][str_len] = symbol;
			str_len++;
			lines[str_num] = (char *) realloc(lines[str_num], (str_len+1)*sizeof(char));
			if (errno == ENOMEM) {
				printf("[error]");
				return 0;
			}
			symbol = getchar();
		}		
		lines[str_num][str_len] = '\0';

		str_num++;
		lines = (char **) realloc(lines, (str_num + 1) * sizeof(char*));
		if (errno == ENOMEM) {
			printf("[error]");
			return 0;
		}
		symbol = getchar();
	}
	*/