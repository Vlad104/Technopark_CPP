#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

 // преобразует строку в число с плавающей точкой
double make_digit(const char * line, size_t *p_str_index) {
	size_t str_index = *p_str_index;
	size_t old_str_index = str_index;
	// находим индекс начала и конца записи числа в строке
	if (line[str_index] == '-') {
		str_index++;		
	}
	while( line[str_index] != '\0' && (isdigit(line[str_index]) || line[str_index] == '.')) {
		str_index++;
	}
	char * temp = (char * ) malloc( (str_index - old_str_index + 1) * sizeof(char)); // выделяем память для строковой записи числа
	if (errno == ENOMEM) {
		printf("[error]");
		free(temp);
		return 0;
	}
	for (size_t i = 0; i < str_index - old_str_index; i++) { // копируем подстроку с числом из входно строки
		temp[i] = line[old_str_index + i];
	}
	temp[str_index - old_str_index] = '\0';
	double digit = atof(temp); // преобразуем строку в число с плавающей точкой
	if (errno == ERANGE) {
		printf("[error]");
		free(temp);
		return 0;
	}

	free(temp);
	*p_str_index = str_index;
	return digit;
}

// функция вычисляет значение выражения записанного в обратной польской нотации
double calculatePolishNotation(const char * line, size_t size) {
	double * stack = (double*) malloc(size * sizeof(double));		
	if (errno == ENOMEM) {
		printf("[error]");
		free(stack);
		return 0;
	}
	size_t stack_index = 0;	
	size_t str_index = 0;
	 // посимвольно проходим по строке, если символ знак выражеия - проводим вычисление над двумя последними значениями в стеке
	 // если символ - число, то записываем его в стек 
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
				if ( str_index > 0 && line[str_index-1] != ' ' ) {  // если это отдельно стоящий минус - то это знак операции, иначе это знак числа, тогда мы "проваливаемся" в default
					stack[stack_index-2] -= stack[stack_index -1] ;
					stack_index--;
					str_index++;
					break;
				}
			default: // преобразуем подстроку в число и записываем его в стек, индекс str_index устанавливаем после это числа во входно строке
				stack[stack_index] = make_digit(line, &str_index);
				stack_index++;
				break;
		}
	}
	double result = stack[stack_index - 1]; // 
	free(stack);
	return result;
}

// удаление всех пробелов из входной последовательности
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

// функция проверки корректности скобок, а также отсутствия символов отличных от цифр и арифметических операций во входной строке
int isCorrect(const char * line) {	
	int answer = 1;
	int unclosed_brackets = 0;
	int i = 0;
	while (line[i] != '\0') {
		if (line[i] == '(') {
			unclosed_brackets++;
		} else if (line[i] == ')') {
			if (unclosed_brackets <= 0) { // открытая скобка не может встретиться раньше закрытой
				return 0;
			}
			unclosed_brackets--;
		} else if (!isdigit(line[i]) && line[i] != ' ' && line[i] != '.' && line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/') {
			return 0;
		}
		i++;
	}
	if (unclosed_brackets != 0) { // если число открытых скорок != числу закрытых
		answer = 0;
	}
	return answer;
}

// перевод входной последовательности выражения (очищенной от пробелов) в запись в обратной польской нотации
char * toPolishNotation(const char * line, size_t size) {
	 // выделяем память вдвое большую, так как неможем гарантировать, что запись в польской нотации не превысит 
	 // размеров записи в обычной нотации (из-за вставки пробелов для различия разных чисел),
	 // но можем гарантировать, что не превысит вдвое увеличенного буффера
	char * result = (char *) malloc(2 * size * sizeof(char)); 
	if (errno == ENOMEM) {
		printf("[error]");
		free(result);
		return 0;
	}

	char * temp  = (char *) calloc('0', size * sizeof(char)); // временный буфер, нужен для хранения скобок и знаков операций, цифры попадают сразу в конечный буфер
	if (errno == ENOMEM) {
		printf("[error]");
		free(temp);
		return 0;
	}
	size_t line_index = 0;
	size_t res_index  = 0;
	size_t temp_index = 0;

	while (line[line_index] != '\0') { 
		// если цифра, то записываем её в конечный буффер
		if (isdigit(line[line_index]) || line[line_index] == '.') {
			result[res_index] = line[line_index];
			res_index++;
		} 
		// проверям, относится ли этот минус к знаку числа, если относится, то тоже его в конечную последовательность
		else if (line[line_index] == '-' && ( line_index == 0 || (line_index > 0 && !isdigit(line[line_index-1])))) {
			result[res_index] = line[line_index];
			res_index++;
		}
		// все остальные символы во временный буффер
		else {
			if (line[line_index] == '(')  { // и открывающуюся скобку тоже			
				temp[temp_index] = line[line_index];
				temp_index++;	
			} else if (line[line_index] == ')') { // если видим закрывающуюся скобку, то идем по временному буферу в обратном порядке до откр. скобки и копируем все знаки операций в конечный буфер
				while(temp_index > 0 && temp[temp_index] != '(') {
					temp_index--;
					if (temp[temp_index] == '+' || temp[temp_index] == '-' || temp[temp_index] == '*' || temp[temp_index] == '/') {
						result[res_index] = temp[temp_index];
						res_index++;
					}
				}
			} else {
				result[res_index] = ' ';  // добавляем пробел для разделения чисел
				res_index++;					
				temp[temp_index] = line[line_index];
				temp_index++;					
			}		
		}		
		line_index++;
	}

	while (temp_index > 0) { // когда обошли всю входную пследовательность, но во временном буффере еще остались символы, то переносим их от туда в конечный буффер
		temp_index--;
		if (temp[temp_index] == '+' || temp[temp_index] == '-' || temp[temp_index] == '*' || temp[temp_index] == '/') {
			result[res_index] = temp[temp_index];
			res_index++;
		}
	}
	result[res_index] = '\0';
	free(temp);

	return result;
}


int main() {
	errno = 0;
	char * line = NULL;
	size_t size = 0;
	ssize_t bytes = getline(&line, &size, stdin); // считываение входной строки
	if ( errno == EINVAL || errno == ENOMEM || bytes <= 0) { // если ошибка, то освобождаем память и завершаем программу	
		printf("[error]");
		free(line);
		return 0;
	}

	if (isCorrect(line) == 0) {  // проверка корректности записи скобок, а также отсутствия лишних символов
		printf("[error]");
		free(line);
		return 0;
	}

	char * inter_line = deleteSpace(line, size);  // удаление всех пробелов
	if (line != NULL) {
		free(line);
	}

	char * new_line = toPolishNotation(inter_line, size);  // перевод выражения в обратную польскую запись
	if (inter_line != NULL) {
		free(inter_line);
	}
	//printf("%s\n", new_line); // это если интересно взяглянуть на польскую запись

	double result = calculatePolishNotation(new_line, size);  // расчет выражения, представленного в польской записи
	if (new_line != NULL) {
		free(new_line);
	}

	printf("%.2f\n", result);

	return 0;
}