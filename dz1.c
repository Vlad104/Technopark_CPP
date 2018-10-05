#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// удаление лишних пробелов
char ** deleteMultiSpace( char * const * const lines, size_t str_num ) {
	char ** result = (char **) malloc(str_num*sizeof(char*));
	if (errno == ENOMEM) { // в случае ошибки освоболить выделенную память, вернуть нулевой указатель
		for (int i = 0; i < str_num; i++) {
			free(result[i]);
		}
		free(result);
		return NULL;
	}

	for (size_t i = 0; i < str_num; i++) {
		size_t str_len = 0, new_str_len = 0;  // str_len - индекс входной строки, new_str_len индекс новой строки, 
		result[i] = (char *)malloc(sizeof(char));
		if (errno == ENOMEM) { 
			for (int i = 0; i < str_num; i++) {
				free(result[i]);
			}
			free(result);
			return NULL;
		}
		while (lines[i][str_len] != '\0') {
			// если текущий и предыдущий символы пробелы переходим на следующий символ входной последовательности
			if ( str_len > 0 && lines[i][str_len] == ' ' && lines[i][str_len-1] == ' ') {
				str_len++;
			}
			else { 
				// копирование остальных символов
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
		result[i][new_str_len] = '\0';
	}
	return result;
}

int main(int argc, char *argv[]) {
	errno = 0;
	char ** lines = (char **) malloc(sizeof(char*));
	if (errno == ENOMEM) { // в случае ошибки выделения памяти - освободить память, завершить программу
		//free(lines[0]);
		free(lines);
		printf("[error]");
		return 0;
	}

	size_t str_num = 0;	// кол-во строк
	ssize_t bytes = 0;

	// цикл чтения массива строк
	do {
		lines[str_num] = NULL;
		size_t size = 0;
		bytes = getline(&lines[str_num], &size, stdin); // считывание вхдной строки, getline динамически выделяет буфер, если он иницилизирован нулевым указателем
		if ( errno == EINVAL || errno == ENOMEM) {	
			printf("[error]");
			for (int i = 0; i < str_num; i++) {
				free(lines[i]);
			}
			free(lines);
			return 0;
		}
		
		str_num++;
		lines = (char **) realloc(lines, (str_num + 1) * sizeof(char*)); // реаллоцирование памяти для следующей строки
		if (errno == ENOMEM) {
			for (int i = 0; i < str_num; i++) {
				free(lines[i]);
			}
			free(lines);
			printf("[error]");
			return 0;
		}

	} while (bytes > 0);
	str_num--; // так как в цикле инкрементируется лишний раз

	char **result = deleteMultiSpace(lines, str_num); // возвращаем строки без лишних пробелов
	if (result == NULL) { // если возвращен нулевой указатель - память очистить, программу завершить
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

	for (size_t i = 0; i < str_num; i++) { // вывод результирующего массива строк
		printf("%s", result[i]);
	}

	for (size_t i = 0; i <= str_num; i++) { // очистка входного массива строк
		free(lines[i]);
	}
	free(lines);
	for (size_t i = 0; i < str_num; i++) { // очистка результирующего массива строк
		free(result[i]);
	}
	free(result);
	
	return 0;
}