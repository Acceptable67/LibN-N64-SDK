/* 
 * Super basic functions to get us going 
 * up until newlib implementation 
 */

#ifndef LIBN_STDLIB_H
#define LIBN_STDLIB_H

#include <cstring>
#include <libn/types.hpp>
#include <libn/vi.hpp>
#include <string>

using namespace LibN64;

// String Functions
int __strlen(const char *src);
char *__strdup(const char *);
int __strncmp(const char *a, const char *b, int amt);
int __strcmp(const char *a, const char *b);
char __toupper(char);
char __tolower(char);

// Memory Functions
void *__malloc(size_t size);
void __memcpy(char *dest, const char *src, size_t len);
void __memset(void *dest, char value, size_t size);

// I/O Functions
int printf(const std::string_view formatted_string);
extern "C" {
    void *sbrk(int incr);
    int printf(const s8 *, ...);
    void RenderConsole();
    void ClearConsole();
    void _exit(int);
    int close([[maybe_unused]] int fildes);
    int fstat([[maybe_unused]] int fildes, [[maybe_unused]] struct stat *st);
    int getpid(void);
    int isatty([[maybe_unused]] int file);
    int kill(int pid, [[maybe_unused]] int sig);
    int write(int file, char *ptr, [[maybe_unused]] int len);
    void ResetConsole();
    int printf(const char *format, ...);
    int stdvprint(const s8 *ptr);
    
    // Assertion Functions
    void __assert_func(const char *file, int line, const char *, const char *e);
    void __assert_func_cpp(std::string file, u32 line, 
                            [[maybe_unused]] std::string function, 
                            std::string exp, std::string reason);
    void __assert(const char *, int, const char *);

    // File Operations
    int link(char *existing, char *neww);
    int lseek([[maybe_unused]] int file, 
               [[maybe_unused]] int ptr, 
               [[maybe_unused]] int dir);
    int open([[maybe_unused]] char *file, 
              [[maybe_unused]] int flags, 
              [[maybe_unused]] int mode);
    int read([[maybe_unused]] int file, 
              [[maybe_unused]] char *ptr, 
              [[maybe_unused]] int len);
    int readlink(const char *path, char *buf, size_t bufsize);
    int stat(const char *file, struct stat *st);
    int symlink(const char *path1, const char *path2);
    int unlink(const char *name);

    // Process Management
    int fork(void);
    int execve(char *name, char **argv, char **env);
    int wait(int *status);
    
    // User and Time Management
    int chown(const char *path, uid_t owner, gid_t group);
    clock_t times(struct tms *buf);
    
    // Time Functions
    int gettimeofday(struct timeval *tp, struct timezone *tz);
}

#endif // LIBN_STDLIB_H
