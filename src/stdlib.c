#include <string.h>
#include <stdio.h>
#include <dir.h>
#include <errno.h>
#include <libn/stdlib.h>
#include <libn/types.h>
#include <libn/vi_display.h>
#include <limits.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <sys/types.h>

u32 consolex = 10, consoley = 10;

extern char end __attribute__((section(".data")));
int errno __attribute__((weak));

int lseek([[maybe_unused]] int file, [[maybe_unused]] int ptr, [[maybe_unused]]  int dir) {
	return 0;
}
int open( [[maybe_unused]] char *file,  [[maybe_unused]] int flags, [[maybe_unused]] int mode) {
	return 0;
}
int read( [[maybe_unused]] int file, [[maybe_unused]]  char *ptr,  [[maybe_unused]] int len) {
	return 0;
}

void _exit(int) {
	exit(1);
}

int close( [[maybe_unused]] int fildes) {
	return 0;
}

int getpid(void) {
	return 1001;
}

int isatty( [[maybe_unused]] int file) {
	return 0;
}

int kill(int pid,  [[maybe_unused]] int sig) {
	if (pid == 1001) exit(1);
	else
		return 0;
}


int link( [[maybe_unused]]char *existing,  [[maybe_unused]] char *neww)
{
	return 0;
}

int unlink( [[maybe_unused]] const char *path){
	return 0;
}

int write([[maybe_unused]] int file, [[maybe_unused]] char *ptr,  [[maybe_unused]] int len) {

	return 0;
}

void ResetConsole() {
	consolex = 10;
	consoley = 10;
}

void _prnt_advance(const s8 * str)
{
	u32 lines		= 1;

	for (const s8 *c = str; *c != '\0'; c++) {
		if (*c == '\n') { lines++; }
	}
	consoley += 8 * lines;
}

int printf(const s8 *format, ...) {
	va_list va;
	va_start(va, format);

	u32 alloc_size = sizeof(format) + sizeof(va);
	s8 *buffer = calloc(0, alloc_size);
	vsnprintf(buffer, alloc_size, format, va);
	Display_DrawText(consolex, consoley, buffer);
	
	va_end(va);

	_prnt_advance(format);
	return 1;
}

void __assert_func(const char *file, int line,  [[maybe_unused]] const char *function, const char *e) {
	printf("ASSERTION FAILED");
	printf("File %s", file);
	printf("Line %d", line);
	printf("%s", e);
	while (1) {}
}

int __strlen(const char *str) {
	if (!str) { return 0; }

	int len = 0;
	while (str[len] != 0) { len++; }

	return len;
}

void *__malloc(size_t size) {
	void *request = sbrk(size);
	return request;
}

int __atoi(char *str) {
	int res = 0;
	for (int i = 0; str[i] != '\0'; ++i) res = res * 10 + str[i] - '0';

	return res;
}

void __memcpy(char *a, char *b, size_t size) {
	for (size_t i = 0; i < size; i++) { a[i] = b[i]; }
}

void __memset(void *arr, char value, size_t size) {
	for (size_t i = 0; i < size; i++) { *((char *)arr + i) = value; }
}

char *__strdup(const char *in) {
	if (!in) { return 0; }

	char *ret = (char *)__malloc(__strlen(in) + 1);
	__memcpy(ret, (char *)in, __strlen(in) + 1);

	return ret;
}

int __strncmp(const char *a, const char *b, int len) {
	if (!a || !b) { return 0; }

	int cur = 0;
	while (cur != len) {
		if (a[cur] != b[cur]) { return 1; }
		if (a[cur] == 0 && b[cur] == 0) { return 0; }
		if (a[cur] == 0 || b[cur] == 0) { return 1; }

		cur++;
	}
	return 0;
}

int __strcmp(const char *const a, const char *const b) {
	return __strncmp(a, b, -1);
}

char __toupper(char a) {
	if (a >= 0x61 && a <= 0x7A) return a - 0x20;
	else
		return 0x00;
}

char __tolower(char a) {
	if (a >= 0x41 && a <= 0x5A) return a + 0x20;
	else
		return 0x00;
}

/*borrowed from libdragon*/
void *sbrk(int incr) {
	static char *heap_end = 0;
	static char *heap_top = 0;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &end;
		heap_top = (char *)((void *)0x80000000) + (*(int *)0xA0000318) - 0x10000;
	}

	prev_heap_end = heap_end;
	heap_end += incr;

	if (heap_end > heap_top) {
		heap_end -= incr;
		prev_heap_end = (char *)-1;
		errno	      = ENOMEM;
	}

	return (void *)prev_heap_end;
}