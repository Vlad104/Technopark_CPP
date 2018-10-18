/**
Задача B-3. Парсер
Time limit:	14 s
Memory limit:	64 M
Требуется написать программу, которая способна вычислять арифметические выражения. 
Выражения могут содержать: 
1) знаки операций '+', '-', '/', '*' 
2) Скобки '(', ')' 
3) Целые и вещественные числа, в нотации '123', '123.345', все операции должны быть вещественны, а результаты выведены с точностю до двух знаков после запятой в том числе целые '2.00' 
4) необходимо учитывать приоритеты операций, и возможность унарного минуса, пробелы ничего не значат 
5) Если в выражении встретилась ошибка требуется вывести в стандартный поток вывода "[error]" (без кавычек) 
**/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

char* read_line();
int is_correct(const char * line);
char* delete_space(const char * line, size_t size );
char* convert_to_polish_notation(const char * line, size_t size);
int make_digit(const char * line, size_t *p_str_index, double* result);
int calculate_polish_notation(const char * line, size_t size, double* result);

int main() {
	size_t size = 0;
	char* line = read_line(&size); // считывание входной последовательности
	if (line == NULL) {
		printf("[error]");
		return 0;
	}
	
	// проверка корректности входной строки: корректные скобки, отсутствие лишних символов
	if (is_correct(line) != 1) {  
		printf("[error]");
		free(line);
		return 0;
	}

  	// удаление всех пробелов
	char* inter_line = delete_space(line, size);
	free(line);
	if (inter_line == NULL) {
		printf("[error]");
		return 0;
	}

	// перевод выражения в обратную польскую запись
	char* new_line = convert_to_polish_notation(inter_line, size);
	free(inter_line);  
	if (new_line == NULL) {
		printf("[error]");
		return 0;
	}

  	// расчет выражения, представленного в польской записи  
	double result = 0.0;
	int correct = calculate_polish_notation(new_line, size, &result);
	free(new_line);
	if (!correct) {
		printf("[error]");
		return 0;
	}

	printf("%.2lf\n", result);

	return 0;
}

// считываем входную строку
char* read_line(size_t* p_size) {
	if (p_size == NULL) {
		return NULL;	
	}
	char* line = NULL;
	size_t size = 0;
	ssize_t bytes = getline(&line, &size, stdin); // считываение входной строки
	if ( errno == EINVAL || errno == ENOMEM || bytes <= 0) { // если ошибка, то освобождаем память и завершаем программу	
		free(line);
		return NULL;
	}
	*p_size = size;
	return line;
}

// функция проверки корректности скобок, а также отсутствия символов отличных от цифр и арифметических операций во входной строке
int is_correct(const char* line) {
	if (line == NULL) {
		return 0;	
	}
	int unclosed_brackets = 0;
	int i = 0;
	while (line[i] != '\0') {
		if (line[i] == '(') {
			unclosed_brackets++;
		} 
		else if (line[i] == ')') {
			if (unclosed_brackets <= 0) { // открытая скобка не может встретиться раньше закрытой
				return 0;
			}
			unclosed_brackets--;
		} 
		else if (!isdigit(line[i]) && line[i] != ' ' && line[i] != '.' && line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/') {
			return 0;
		}
		i++;
	}
	if (unclosed_brackets != 0) { // если число открытых скорок != числу закрытых
		return 0;
	}
	return 1;
}

// удаление всех пробелов из входной последовательности
char* delete_space(const char* line, size_t size ) {
	if (line == NULL) {
		return NULL;	
	}
	char* result = (char*) malloc(size * sizeof(char));
	if (errno == ENOMEM) {
		return NULL;
	}
	int i = 0;
	int j = 0;
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

// перевод входной последовательности выражения (очищенной от пробелов) в запись в обратной польской нотации
char* convert_to_polish_notation(const char* line, size_t size) {
	if (line == NULL) {
		return NULL;	
	}
	 // выделяем память вдвое большую, так как неможем гарантировать, что запись в польской нотации не превысит 
	 // размеров записи в обычной нотации (из-за вставки пробелов внутри этой функции для различия разных чисел),
	 // но можем гарантировать, что не превысит вдвое увеличенного буффера
	char* result = (char*) malloc(2 * size * sizeof(char)); 
	if (errno == ENOMEM) {
		return NULL;
	}

	char* temp  = (char*) calloc('0', size * sizeof(char)); // временный буфер, нужен для хранения скобок и знаков операций, цифры попадают сразу в конечный буфер
	if (errno == ENOMEM) {
		return NULL;
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


// преобразует строку в число с плавающей точкой
// возвращает 0 в случае ошибок и 1 в их отсутствии
int make_digit(const char* line, size_t* p_str_index, double* result) {
	if (line == NULL || p_str_index == NULL || result == NULL) {
		return 0;
	}
	size_t str_index = *p_str_index;
	size_t old_str_index = str_index;
	// далее находим индекс начала и конца записи числа в строке
	if (line[str_index] == '-') { // если перый символ минус - то он относится к знаку числа
		str_index++;		
	}
	while( line[str_index] != '\0' && (isdigit(line[str_index]) || line[str_index] == '.')) {
		str_index++;
	}
	char* temp = (char* ) malloc( (str_index - old_str_index + 1) * sizeof(char)); // выделяем память для строковой записи числа
	if (errno == ENOMEM) {
		return 0;
	}
	for (size_t i = 0; i < str_index - old_str_index; i++) { // копируем подстроку с записью числа из входной строки
		temp[i] = line[old_str_index + i];
	}
	temp[str_index - old_str_index] = '\0';
	double digit = atof(temp); // преобразуем строку в число с плавающей точкой
	free(temp);
	if (errno == ERANGE) {
		return 0;
	}

	*p_str_index = str_index;
	*result = digit;
	return 1;
}

// функция вычисляет значение выражения записанного в обратной польской нотации
// возвращает 0 в случае ошибок и 1 в их отсутствии
int calculate_polish_notation(const char* line, size_t size, double* result) {
	if (line == NULL || result == NULL) {
		return 0;		
	}
	double* stack = (double*) malloc(size* sizeof(double));		
	if (errno == ENOMEM) {
		return 0;
	}
	size_t stack_index = 0;	
	size_t str_index = 0;
	double digit = 0.0;
	 // посимвольно проходим по строке, если символ знак выражеия - проводим вычисление над двумя последними значениями в стеке
	 // если символ - число, то записываем его в стек 
	while (line[str_index] != '\0') {
		switch (line[str_index]) {
			case ' ':
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
			default:
				digit = 0.0;
				int correct = make_digit(line, &str_index, &digit);
				if (!correct) { // если произошла ошибка в make_digit
					free(stack);
					return 0;
				}
				stack[stack_index] = digit;
				stack_index++;
				break;
		}
	}
	*result = stack[stack_index - 1];
	free(stack);
	return 1;
}
