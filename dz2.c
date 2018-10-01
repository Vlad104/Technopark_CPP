#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

float make_digit(const char * line, size_t *p_str_index) {
	size_t str_index = *p_str_index;
	size_t old_str_index = str_index;
	while(isdigit(line[str_index]) || line[str_index] == '.') { /// proverka na granici
		str_index++;
	}
	char * temp = (char * ) malloc( (str_index - old_str_index) * sizeof(char));
	for (size_t i = 0; i < str_index - old_str_index; i++) {
		temp[i] = line[old_str_index + i];
	}
	float digit = atof(temp);
	free(temp);
	*p_str_index = str_index;
	return digit;
}

float polyak(const char * line) {
	float stack[10];
	for (int i = 0; i < 10; i++) {
		stack[i] = 0.0;
	}
	size_t stack_index = 0;	
	size_t str_index = 0;
	while (line[str_index] != '\0') {
		char symbol = line[str_index];
		switch (symbol) {
			case ' ':
				str_index++;
				break;
			case '=':
				return stack[stack_index - 1];
				stack_index--;
				str_index++;
				break;
			case '+':
				stack[stack_index-2] += stack[stack_index -1];
				stack_index--;
				str_index++;
				break;
			case '-':
				stack[stack_index-2] -= stack[stack_index -1];
				stack_index--;
				str_index++;
				break;
			case '*':
				stack[stack_index-2] *= stack[stack_index -1];
				stack_index--;
				str_index++;
				break;
			case '/':
				stack[stack_index-2] /= stack[stack_index -1];
				stack_index--;
				str_index++;
				break;
			default:
				stack[stack_index] = make_digit(line, &str_index);
				stack_index++;
				break;
		}

		/*
		printf("sym = %c, ind = %ld, stack = %ld\n", symbol, str_index, stack_index);
		for (int i = 0; i < 10; i++) {
			printf("%.2f ", stack[i]);
		}
		printf("\n");
		*/

	}
	return stack[stack_index - 1];
}

char * make_polish(const char * line) {

	char * result = (char *) malloc(sizeof(char));
	char * temp   = (char *) malloc(sizeof(char));
	size_t line_index = 0;
	size_t res_index  = 0;
	size_t temp_index = 0;
	while (line[line_index] != '\0') {
		if (isdigit[line[line_index]] || line[line_index] == '.') {
			result[res_index] = line[line_index];
			res_index++;
		} else if (line[line_index] != ' ') {
			if (line[line_index] == ')') {
				result[res_index] = temp[temp_index-1];
				res_index++;				
			} else if (line[line_index] != '(') {
				temp[temp_index] = line[line_index];
				temp_index++;					
			}		
		}		
		line_index++;
	}

	return result;
}


int main() {
	errno = 0;
	char * line = NULL;
	size_t size = 0;
	ssize_t bytes = getline(&line, &size, stdin);

	//new_line = make_polish(line);
	float result = polyak(line);
	printf("%.2f\n", result);


	return 0;
}