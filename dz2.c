#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

double make_digit(const char * line, size_t *p_str_index) {
	size_t str_index = *p_str_index;
	size_t old_str_index = str_index;
	if (line[str_index] == '-') {
		str_index++;		
	}
	while(isdigit(line[str_index]) || line[str_index] == '.') { /// proverka na granici
		str_index++;
	}
	char * temp = (char * ) malloc( (str_index - old_str_index + 1) * sizeof(char));
	if (errno == ENOMEM) {
		printf("[error]");
		free(temp);
		return 0;
	}
	for (size_t i = 0; i < str_index - old_str_index; i++) {
		temp[i] = line[old_str_index + i];
	}
	temp[str_index - old_str_index] = '\0';
	double digit = atof(temp);
	if (errno == ERANGE) {
		printf("[error]");
		free(temp);
		return 0;
	}

	free(temp);
	*p_str_index = str_index;
	return digit;
}

double calculatePolishNotation(const char * line, size_t size) {
	double * stack = (double*) malloc(size * sizeof(double));		
	if (errno == ENOMEM) {
		printf("[error]");
		free(stack);
		return 0;
	}
	size_t stack_index = 0;	
	size_t str_index = 0;
	while (line[str_index] != '\0') {
		switch (line[str_index]) {
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
			case '-':
				if ( str_index > 0 && line[str_index-1] != ' ' ) {
					stack[stack_index-2] -= stack[stack_index -1] ;
					stack_index--;
					str_index++;
					break;
				}
			default:
				stack[stack_index] = make_digit(line, &str_index);
				stack_index++;
				break;
		}
	}
	double result = stack[stack_index - 1];
	free(stack);
	return result;
}

char * deleteSpace(const char * line, size_t size ) {
	char * result = (char *) malloc(size * sizeof(char));
	if (errno == ENOMEM) {
		printf("[error]");
		free(result);
		return 0;
	}
	int i = 0;
	while (line[i] != '\0') {
		if (line[i] != ' ') {
			result[i] = line[i];
		}
		i++;
	}
	result[i] = '\0';
	return result;
}

char * toPolishNotation(const char * input_line, size_t size) {

	char * line   = deleteSpace(input_line, size);
	char * result = (char *) malloc(2*size * sizeof(char));
	if (errno == ENOMEM) {
		printf("[error]");
		free(result);
		return 0;
	}
	char * temp   = (char *) malloc(size * sizeof(char));
	if (errno == ENOMEM) {
		printf("[error]");
		free(temp);
		return 0;
	}
	size_t line_index = 0;
	size_t res_index  = 0;
	size_t temp_index = 0;
	while (line[line_index] != '\0') {
		if (isdigit(line[line_index]) || line[line_index] == '.') {
			result[res_index] = line[line_index];
			res_index++;
		} 
		else if (line[line_index] == '-' && ( line_index == 0 || (line_index > 0 && !isdigit(line[line_index-1])))) {
			result[res_index] = line[line_index];
			res_index++;
		}
		//else if (line[line_index] != ' ') {
		else {
			if (line[line_index] == ')') {
				temp_index--;
				result[res_index] = temp[temp_index];
				res_index++;				
			} else if (line[line_index] != '(') {
				result[res_index] = ' ';  // add space for number separation
				res_index++;					
				temp[temp_index] = line[line_index];
				temp_index++;					
			}		
		}		
		line_index++;
	}
	while (temp_index > 0) {
		temp_index--;
		if (temp[temp_index] == '+' || temp[temp_index] == '-' || temp[temp_index] == '*' || temp[temp_index] == '/') {
			result[res_index] = temp[temp_index];
			res_index++;
		}
	}
	result[res_index] = '\0';
	free(line);
	free(temp);
	return result;
}


int main() {
	errno = 0;
	char * line = NULL;
	size_t size = 0;
	ssize_t bytes = getline(&line, &size, stdin);
	if ( errno == EINVAL || errno == ENOMEM || bytes <= 0) {		
		printf("[error]");
		free(line);
		return 0;
	}

	char * new_line = toPolishNotation(line, size);
	free(line);
	//printf("%s\n", new_line);

	double result = calculatePolishNotation(new_line, size);
	free(new_line);
	printf("%.2f\n", result);

	return 0;
}