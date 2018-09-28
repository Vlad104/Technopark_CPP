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
	for (size_t i = old_str_index; i < str_index; i++) {
		temp[i] = line[old_str_index + i];
	}
	float digit = atof(temp);
	free(temp);
	*p_str_index = str_index;
	return digit;
}

float polyak(const char * line) {
	float stack[1000];
	size_t stack_index = 0;	
	size_t str_index = 0;
	while (line[str_index] != '\0') {
		char symbol = line[str_index];
		switch (symbol) {
			case ' ':
				str_index++;
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
				break;
		}
	}
	return stack[stack_index - 1];
}




int main() {
	errno = 0;
	char * line = NULL;
	size_t size = 0;
	ssize_t bytes = getline(&line, &size, stdin);

	float result = polyak(line);
	printf("%.2f\n", result);


	return 0;
}