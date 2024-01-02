#include <ctime>
#include <cstring>
#include <errno.h>
#include <libn/stdlib.hpp>
#include <libn/types.hpp>
#include <libn/vi.hpp>
#include <limits.h>
#include <malloc.h>
#include <source_location>
#include <stdarg.h>
#include <string>
#include <sys/types.h>

using namespace LibN64;
u32 consolex = 10, consoley = 10;

extern char end __attribute__((section(".data")));
int errno __attribute__((weak));

int lseek([[maybe_unused]] int file, [[maybe_unused]] int ptr, [[maybe_unused]] int dir) {
	return 0;
}
int open([[maybe_unused]] char *file, [[maybe_unused]] int flags, [[maybe_unused]] int mode) {
	return 0;
}
int read([[maybe_unused]] int file, [[maybe_unused]] char *ptr, [[maybe_unused]] int len) {
	return 0;
}

void _exit(int) {
	exit(1);
}

int close([[maybe_unused]] int fildes) {
	return 0;
}

int fstat([[maybe_unused]] int fildes, [[maybe_unused]] struct stat *st) {
	return 0;
}

int getpid(void) {
	return 1001;
}

int isatty([[maybe_unused]] int file) {
	return 0;
}

int kill(int pid, [[maybe_unused]] int sig) {
	if (pid == 1001) exit(1);
	else
		return 0;
}

int gettimeofday ( [[maybe_unused]] struct timeval *tp , [[maybe_unused]] struct timezone *tz )
{
	return 0;
}

clock_t times( [[maybe_unused]] struct tms *buf);

int link([[maybe_unused]] char *existing, [[maybe_unused]] char *neww)
{
	return 0;
}

int unlink([[maybe_unused]] const char *path){
	return 0;
}

int write(int file, char *ptr, [[maybe_unused]] int len) {
	s8 buffer[32];
	memcpy(buffer, ptr, 32);
	buffer[31]	 = '\0';
	std::string copy = buffer;

	if (file == 1) { // stdout, very bugged
		printf(copy.c_str());
	} else { // stderr, working fine
		printf(copy.c_str());
	}
	return 0;
}

void ResetConsole() {
	consolex = 10;
	consoley = 10;
}

void _prnt_advance(const s8 * str)
{
	auto lines		= 1;
	std::string_view localformat = str;
	for (auto &n : localformat) {
		if (n == '\n') { lines++; }
	}
	consoley += 8 * lines;
}

int printf(const s8 *format, ...) {
	va_list va;
	va_start(va, format);
	s8 buffer[100];
	vsnprintf(buffer, sizeof(buffer), format, va);
	Display::DrawText({consolex, consoley}, buffer);
	va_end(va);

	_prnt_advance(format);
	return 1;
}

int printf(const std::string_view formatted_string) {
	Display::DrawText({consolex, consoley}, formatted_string);
	_prnt_advance(formatted_string.data());
	return 1;
}

/*This is the print for fmt:: library*/
int stdvprint(const s8 *ptr) {
	Display::DrawText({consolex, consoley}, ptr);
	_prnt_advance(ptr);
	return 1;
}

void __assert_func(const char *file, int line, [[maybe_unused]] const char *function, const char *e) {
	printf("ASSERTION FAILED");
	printf("File %s", file);
	printf("Line %d", line);
	printf("%s", e);
	while (1) {}
}

void __assert_func_cpp(std::string file, u32 line, [[maybe_unused]] std::string function, std::string exp,
		       std::string reason) {
	printf("ASSERTION FAILED!\n\n");
	printf("File: %s\n\n", file.c_str());
	printf("Line: %u\n\n", line);
	printf("\"%s\" %s", exp.c_str(), reason.c_str());
	while (1) {}
}

#ifdef CONSOLE_ALT

std::vector<std::string> consoleTextBuffer;
void ClearConsole() {
	consoleTextBuffer.clear();
}

void RenderConsole() {
	consolex   = 10;
	consoley   = 10;
	auto lines = 1;
	for (auto &str : consoleTextBuffer) {
		Display::DrawText({consolex, consoley}, str);
		consoley += 8 * lines;
	}
	ClearConsole();
}

int printf(const char *format, ...) {
	va_list va;
	va_start(va, format);

	char buffer[100];
	vsnprintf(buffer, sizeof(buffer), format, va);
	consoleTextBuffer.push_back(std::string(buffer));
	va_end(va);
	return 0;
}

void __assert_func(const char *file, int line, const char *, const char *e) {
	ClearConsole();
	printf("ASSERTION FAILED");
	printf("File %s", file);
	printf("Line %d", line);
	printf("%s", e);
	RenderConsole();
	while (1) {}
}

void __assert_func_cpp(std::string file, u32 line, [[maybe_unused]] std::string function, std::string exp,
		       std::string reason) {
	ClearConsole();
	printf("ASSERTION FAILED!\n\n");
	printf("File: %s\n\n", file.c_str());
	printf("Line: %u\n\n", line);
	printf("\"%s\" %s", exp.c_str(), reason.c_str());
	RenderConsole();
	while (1) {}
}
#endif

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
extern "C" void *sbrk(int incr) {
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
