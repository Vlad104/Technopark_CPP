#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char ** deleteMultiSpace( char ** lines, int str_num ) {
	char ** result = (char **) malloc(str_num*sizeof(char*));
	if (errno == ENOMEM) {
		printf("[error");
		return 0;
	}
		int str_len = 0, new_str_len =0;	
	for (int i = 0; i < str_num; i++) {
		//int str_len = 0, new_str_len =0;		
		result[i] = (char *)malloc(sizeof(char));
		while (lines[i][str_len] != '\0') {
			if ( str_len > 0 && lines[i][str_len] == ' ' && lines[i][str_len-1] == ' ') {
				str_len++;
			}
			else {
				result[i][new_str_len] = lines[i][str_len];
				str_len++;
				new_str_len++;
				result[i] = (char *)realloc(result, (new_str_len+1)*sizeof(char));
				if (errno == ENOMEM) {
					printf("[error]");
					return 0;
				}
			}
		}
		result[i][new_str_len] = '\n';
	}
	result[str_num-1][new_str_len] = '\0';
	return result;
}

int main() {
	errno = 0;
	char ** lines = (char **)malloc(sizeof(char *));
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
		symbol = getchar();
	}

	for (int i = 0; i < str_num; i++) {
		printf("%s\n", lines[i]);
	}


	char ** result = deleteMultiSpace(lines, str_num);

	for (int i = 0; i < str_num; i++) {
		int nbytes = printf("%s\n", result[i]);
		printf("bytes = %d\n", nbytes);
		if (errno == EILSEQ || errno == EINVAL) {
			printf("[error");
			return 0;
		}
	}
	
	for (int i = 0; i < str_num; i++) {
		free(lines[i]);
		free(result[i]);
	}
	
	//getchar();	
	return 0;
}