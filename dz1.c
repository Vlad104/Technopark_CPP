#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// function delete multi spaces
char ** deleteMultiSpace( char * const * const lines, size_t str_num ) {
	char ** result = (char **) malloc(str_num*sizeof(char*));
	if (errno == ENOMEM) { // if malloc error then close programm
		for (int i = 0; i < str_num; i++) {
			free(result[i]);
		}
		free(result);
		return NULL;
	}

	for (size_t i = 0; i < str_num; i++) {
		size_t str_len = 0, new_str_len = 0;  // str_len index of input array, new_str_len index of output array, 
		result[i] = (char *)malloc(sizeof(char));
		if (errno == ENOMEM) {
			for (int i = 0; i < str_num; i++) {
				free(result[i]);
			}
			free(result);
			return NULL;
		}
		while (lines[i][str_len] != '\0') {
			// if symbol is space and previous symboll is space too
			if ( str_len > 0 && lines[i][str_len] == ' ' && lines[i][str_len-1] == ' ') {
				str_len++;
			}
			else { 
				// copy other symbols
				result[i][new_str_len] = lines[i][str_len];
				str_len++;
				new_str_len++;
				result[i] = (char *)realloc(result[i], (new_str_len+1)*sizeof(char));
				if (errno == ENOMEM) {
					for (int i = 0; i < str_num; i++) {
						free(result[i]);
					}
					free(result);
					return NULL;
				}
			}
		}
		//printf("%d %d\n", str_len, new_str_len);
		result[i][new_str_len] = '\0';
	}
	return result;
}

int main(int argc, char *argv[]) {
	errno = 0;
	char ** lines = (char **) malloc(sizeof(char*)); // pointer to string array
	if (errno == ENOMEM) { // if malloc error then close programm
		//free(lines[0]);
		free(lines);
		printf("[error]");
		return 0;
	}

	size_t str_num = 0;	// numbers of string
	ssize_t bytes = 0;

	// start reading input strings
	do {
		lines[str_num] = NULL;
		size_t size = 0;
		bytes = getline(&lines[str_num], &size, stdin);
		if ( errno == EINVAL || errno == ENOMEM) {		
			printf("[error]");
			for (int i = 0; i < str_num; i++) {
				free(lines[i]);
			}
			free(lines);
			return 0;
		}
		
		str_num++;
		lines = (char **) realloc(lines, (str_num + 1) * sizeof(char*));
		if (errno == ENOMEM) {
			for (int i = 0; i < str_num; i++) {
				free(lines[i]);
			}
			free(lines);
			printf("[error]");
			return 0;
		}

	} while (bytes > 0);
	str_num--;

	char **result = deleteMultiSpace(lines, str_num); // return pointer to modificated string array
	if (result == NULL) {
		for (int i = 0; i <= str_num; i++) {
			free(lines[i]);
		}
		free(lines);
		for (int i = 0; i < str_num; i++) {
			free(result[i]);
		}
		free(result);
		printf("[error]");
		return 0;

	}

	for (size_t i = 0; i < str_num; i++) { // print strings
		printf("%s", result[i]);
	}

	for (size_t i = 0; i <= str_num; i++) { // clear memory
		free(lines[i]);
	}
	free(lines);
	for (size_t i = 0; i < str_num; i++) { // clear memory
		free(result[i]);
	}
	free(result);
	
	return 0;
}