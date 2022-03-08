/*super basic functions to get us going up until newlib implementation*/
#ifndef LIBN_STDLIB_H
#define LIBN_STDLIB_H

#include <string.h>
#include <stdio.h>
#include <dir.h>
#include <errno.h>
#include <libn/stdlib.h>
#include <libn/types.h>
#include <libn/vi.h>
#include <limits.h>
#include <malloc.h>
#include <stdarg.h>
#include <sys/types.h>

int __strlen(const char *src);
void *__malloc(size_t size);
void __memcpy(char *dest, char *src, size_t len);
void __memset(void *dest, char value, size_t size);
char *__strdup(const char *);
int __strncmp(const char *a, const char *b, int amt);
int __strcmp(const char *a, const char *b);
char __toupper(char);
char __tolower(char);

void *sbrk(int incr);
int printf(const s8 *, ...);
void RenderConsole();
void ClearConsole();
void _exit(int);
int close( int fildes);
//int fstat( int fildes,  struct stat *st);
int getpid(void);
int isatty( int file);
int kill(int pid,  int sig);
int write(int file, char *ptr,  int len);
void ResetConsole();
int printf(const char *format, ...);
int stdvprint(const s8 *ptr);
void __assert_func(const char *file, int line, const char *, const char *e);
void __assert(const char *, int, const char *);
int link(char *existing, char *neww);
int lseek( int file,  int ptr,  int dir);
int open( char *file,  int flags,  int mode);
int read( int file,  char *ptr,  int len);
int readlink(const char *path, char *buf, size_t bufsize);
//int stat(const char *file, struct stat *st);
int symlink(const char *path1, const char *path2);
//clock_t times(struct tms *buf);
int unlink(const char *name);
int wait(int *status);
int chown(const char *path, uid_t owner, gid_t group);
int execve(char *name, char **argv, char **env);
void exit(int rc);
int fork(void);
// int gettimeofday(struct timeval *ptimeval, void *ptimezone);
//int gettimeofday(struct timeval *tp, struct timezone *tz);


#endif