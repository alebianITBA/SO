
#include <stdarg.h>
#include <types.h>
#include <syscalls.h>
#include <libc.h>
#include "string.h"

void* malloc(int len) {
	return sys_malloc(len);
}

void* calloc(int len) {
	return sys_calloc(len);
}

void free(void* m) {
	sys_free(m);
}

static void vfprintf(int fd, char* fmt, va_list ap) {

	char* str = calloc(MAX_PRINTF_LEN);
	int i = 0;
	int j = 0;

	// i: posicion en el fmt
	// j: posicion en el str
	while (fmt[i] != 0 && i < MAX_PRINTF_LEN - 1) {

		if (fmt[i] == '%') {

			bool flag_zero = FALSE;
			uint32_t width = 0;
			bool is_long = FALSE;

			i++;
			if (fmt[i] == 0) {
				//lo que le sigue al % es el final de la cadena
				str[j] = fmt[i];
				break;

			} else if (fmt[i] == '%') {

				str[j] = fmt[i];
				j++;
				i++;

			} else {
				// hay que procesar el siguiente caracter y actuar acorde
				bool flag;
				do {
					flag = FALSE;
					switch (fmt[i]) {
					case 's': {
						//lo que se desea es imprimir una cadena
						char* arg = va_arg(ap, char*);
						int k = 0;

						//k: posicion en el argumento

						while (arg[k] != 0) {
							str[j++] = arg[k++];
						}

						if (width > 0) {
							int spaces = width - strlen(arg);
							while (spaces > 0) {
								str[j++] = ' ';
								spaces--;
							}
						}

						i++;
						break;
					}

					case 'l':
						is_long = TRUE;
						i++;
						flag = TRUE;
						break;


					case 'd': {

						long int arg;

						if (is_long) {
							arg = va_arg(ap, long int);
						} else {
							arg = (long int)va_arg(ap, int);
						}

						char* number = itoc(arg);

						int k = 0;

						uint32_t numlen = strlen(number);

						// if (arg < 0) {
						// 	str[j++] = '-';
						// }

						//k: posicion en el argumento

						while (number[k] != 0) {
							str[j++] = number[k++];
						}

						if (numlen < width) {

							char chartowrite;
							int numtowrite = width - numlen;

							if (flag_zero) {
								chartowrite = '0';

							} else {
								chartowrite = ' ';
							}

							for (int i = 0; i < numtowrite; i++) {
								str[j++] = chartowrite;
							}
						}

						i++;
						break;
					}

					case 'c': {

						char arg = (char)va_arg(ap, int);

						str[j++] = arg;
						i++;
						break;
					}

					case 'x': {

						char buffer[128] = { 0 };

						uint64_t arg = va_arg(ap, uint64_t);
						uint32_t digits = uintToBase(arg, buffer, 16);

						for (uint32_t i = 0; i < digits; i++) {
							str[j++] = buffer[i];
						}

						i++;
						break;
					}

					case '0':
						if (width == 0) {
							if (!flag_zero) {
								flag_zero = TRUE;
								i++;
								flag = TRUE;
								break;
							}
							break;
						}


					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						if (width > 0) {
							width *= 10;
						}
						width += fmt[i] - '0';
						i++;
						flag = TRUE;
						break;

					}

				} while (flag);
			}

		} else if (fmt[i] != 0) {
			str[j] = fmt[i];
			j++;
			i++;
		} else {
			str[j] = fmt[i];
			break;
		}
	}

	sys_write(fd, str, j);

	free(str);
}

void fprintf(int fd, char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	vfprintf(fd, fmt, ap);

	va_end(ap);
}

void printf(char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	vfprintf(stdout, fmt, ap);

	va_end(ap);
}

void putchar(char c) {
	sys_write(stdout, &c, 1);
}

int getchar() {

	static char buffer[2] = {0};

	char read = sys_read(stdout, buffer, 1);

	if (read == EOF) {
		return EOF;
	}

	return (int) buffer[0];
}

int scanf(char* c, int len) {
	return (int)sys_read(stdin, c, len);
}

char* strcat(char* str1, char* str2) {
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	char* ans = calloc((len1 + len2) * sizeof(char));
	strcpy(ans, str1);
	strcpy(ans + len1, str2);
	return ans;
}

time_t* time() {

//FIXME
	//	time_t t = {0};

	//sys_rtc_get(&t);
	time_t* t = (time_t*)calloc(sizeof(time_t));

	sys_rtc_get(t);

	return t;
}

void set_time(time_t * t) {
	sys_rtc_set(t);
}

color_t get_color() {
	return sys_get_color();
}

void set_color(vga_color fg, vga_color bg) {
	color_t t = BUILD_COLOR(fg, bg);

	sys_set_color(t);
}

char* itoc(int number) {

	int i = 0;
	int j = 0;
	int cnt = 0;

	char* c = malloc(10);

	if (number < 0) {
		number = -number;
		c[i++] = '-';
	}

	while (number >= 10 ) {
		int dig = number % 10;
		number /= 10;
		c[i++] = dig + '0';
		cnt++;
	}
	c[i] = number + '0';

	while (cnt >= j) {
		char aux;
		aux = c[cnt];
		c[cnt--] = c[j];
		c[j++] = aux;
	}

	return c;
}

bool string_numeric(char* str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (!is_numeric(str[i])) {
			return FALSE;
		}
	}
	return TRUE;
}

bool is_numeric(char c) {
	return (c >= '0' && c <= '9');
}

int ctoi(char* c) {
	int ans = 0;
	int len = strlen(c);
	for (int i = 0; i < len; i++)
	{
		ans = ans * 10 + c[i] - '0';
	}
	return ans;
}

void clear_screen() {
	sys_clear_screen();
}

void exit() {
	sys_exit();
}
