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
	int i = 0, j = 0;
	while (line[i] != '\0') {
		if (line[i] != ' ') {
			result[j] = line[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	return result;
}

int isCorrect(const char * line) {
	
	int answer = 1;
	int unclosed_brackets = 0;
	int i = 0;
	while (line[i] != '\0') {
		if (line[i] == '(') {
			unclosed_brackets++;
		} else if (line[i] == ')') {
			if (unclosed_brackets <= 0) {
				return 0;
			}
			unclosed_brackets--;
		} else if (!isdigit(line[i]) && line[i] != ' ' && line[i] != '.' && line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/') {
			return 0;
		}
		i++;
	}
	if (unclosed_brackets != 0) {
		answer = 0;
	}
	return answer;
}

char * toPolishNotation(const char * line, size_t size) {

	char * result = (char *) malloc(2 * size * sizeof(char));
	if (errno == ENOMEM) {
		printf("[error]");
		free(result);
		return 0;
	}
	//char * temp   = (char *) malloc(size * sizeof(char));
	char * temp   = (char *) calloc('0', size * sizeof(char));
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
		else {
			if (line[line_index] == '(')  {			
				temp[temp_index] = line[line_index];
				temp_index++;	
			} else if (line[line_index] == ')') {
				while(temp_index > 0 && temp[temp_index] != '(') {
					temp_index--;
					if (temp[temp_index] == '+' || temp[temp_index] == '-' || temp[temp_index] == '*' || temp[temp_index] == '/') {
						result[res_index] = temp[temp_index];
						res_index++;
					}
				}
			} else {
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
	//printf("%s\n", temp);
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

	if (isCorrect(line) == 0) {
		printf("[error]");
		free(line);
		return 0;
	}

	char * inter_line = deleteSpace(line, size);
	if (line != NULL) {
		free(line);
	}

	char * new_line = toPolishNotation(inter_line, size);
	if (inter_line != NULL) {
		free(inter_line);
	}
	//printf("%s\n", new_line);

	//return 0;

	double result = calculatePolishNotation(new_line, size);
	if (new_line != NULL) {
		free(new_line);
	}

	printf("%.2f\n", result);

	return 0;
}