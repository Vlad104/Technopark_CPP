#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

double makeDigit(const char * line2, int pos1, int pos2) {
	int n = pos2 - pos1 - 1;
	char * res = (char *) malloc (sizeof(char) * n);
	memcpy(res, line2, n*sizeof(char));
	return 	atof(res);
}

double calculator( const char * line2) {
	int i = 0;
	int pos1 = 0, pos2 = 0;
	double total = 0;
	while (line2[i] != '\0') {
		if (is_digit(line[i]) || (line[i] == '.')) {
			pos2 = i;
		} else { 
			//make digit
			pos1 = pos2;
		}
	}
}

char * deleteSpace( const char * line, int n ) {
	char * result = (char *)malloc(sizeof(char));
	int i = 0, j =0;
	while (i < n) {
		if ( i > 0 && line[i] == ' ') {
			i++;
		}
		else {
			result = (char *)realloc(result, (j+1)*sizeof(char));
			if (errno == ENOMEM) {
				return 0;
			}
			result[j] = line[i];
			i++;
			j++;
		}
	}
	result[j] = '\0';
	return result;
}


bool is_correct( const char * line, int n) {
	bool res = true;
	int open_brackets = 0;
	int close_brackets = 0;
	for (int i = 0; i < n; i++) {
		if (line[i] == '(') {
			open_brackets++;
		} else if (line[i] == ')') {
			close_brackets++;	
		}			
	}
	if (open_brackets != close_brackets) {
		res = false;
	}
	
	
	return res;
}


int main() {
	errno = 0;
	char * line = (char *)malloc(sizeof(char));
	if (errno == ENOMEM) {
		return false;
	}
	char sym;
	int i = 0;
	
	while ( (sym = getchar()) != '\n') {
		line = (char *) realloc(line, (i+1)*sizeof(char));
		if (errno == ENOMEM) {
			return false;
		}
		line[i] = sym;
		i++;
	}		
	line[i] = '\0';
	
	if (!is_correct(line, i)) {
		printf("[ERROR]");
	}
	char * line2 = deleteSpace(line, i);
	free(line);
	
	
	double result = calculator(line, i);
		
	

	
	getchar();	
	return 0;
}