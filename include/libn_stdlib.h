/*super basic functions to get us going up until newlib implementation*/
#ifndef LIBN_STDLIB_H
#define LIBN_STDLIB_H

#include <stdarg.h>
#include <malloc.h>
#include <sys/types.h>
#include <limits.h>
#include <dir.h>
#include <errno.h>
#include <source_location>

u32 consolex = 10, consoley = 10;

using namespace LibN64;

extern char   end     __attribute__((section (".data")));
int           errno   __attribute__((weak));

static int    __strlen(const char*); 
static void*  __malloc(size_t size);
static void   __memcpy(char*, char* b, size_t len);
static void   __memset(void*, char value, size_t size);
static char*  __strdup(const char* );
static int    __strncmp(const char*, const char*, int);
static int    __strcmp(const char* a, const char*);
static char   __toupper(char a);
static char   __tolower(char a);

extern "C" void  *sbrk(int incr);
extern "C" int printf(const char*, ...);

extern "C" 
{
    void _exit(int) 
    {
         exit(1);
    }

    int close([[maybe_unused]] int fildes) 
    {
         return 0;
    }

    int fstat( [[maybe_unused]] int fildes, [[maybe_unused]] struct stat *st ) 
    {
         return 0; 
    }

    int getpid( void ) 
    { 
        return 1001;
    }

    int isatty( [[maybe_unused]] int file ) 
    { 
        return 0;
        }

    int kill( int pid, [[maybe_unused]] int sig) 
    { 
        if(pid == 1001) 
            exit(1);
         else 
            return 0; 
    }

    /*STDIO, STDIN, AND STDERR HOOKS*/
    int write( int file, char *ptr, [[maybe_unused]] int len ) 
    {
        char buffer[32];
        memcpy(buffer, ptr, 32);
        buffer[31] = '\0';

        if(file == 1) 
        { //stdout
            Display::DrawText({Display::cPos.x, Display::cPos.y}, buffer); 
        }
         else { //stderr
             Display::DrawText({Display::cPos.x, Display::cPos.y},std::to_string(file) + ptr);
            Display::cPos.y+=8; 
        }
         return 0;
    }

    void ResetConsole()
    {
        consolex = 10;
        consoley = 10;
    }

    int printf(const char* format, ...) 
    {
        va_list va;
        va_start(va, format);

        char buffer[100];
        vsnprintf(buffer, sizeof(buffer), format, va);

        Display::DrawText({consolex,consoley},buffer);

        auto lines = 1;
        std::string localformat = format;
        for(auto &n : localformat) {
            if(n == '\n') {
                lines++;
            }
        }
        consoley += 8 * lines;
        va_end(va);
        return 0;
    }

    void __assert_func(const char *file, int line, const char *, const char *e) 
    {
        printf("ASSERTION FAILED");
        printf("File %s", file);
        printf("Line %d", line);
        printf("%s", e);
        HALT();
    }

    void __assert_func_cpp(std::string file, u32 line, [[maybe_unused]] std::string function, std::string exp, std::string reason) 
    {
        printf("ASSERTION FAILED!\n\n");
        printf("File: %s\n\n", file.c_str());
        printf("Line: %u\n\n", line);
        printf("\"%s\" %s", exp.c_str(), reason.c_str());
        HALT();
    }

    void __assert(const char *, int, const char *);
    int link( char *existing, char *neww );
    int lseek( [[maybe_unused]]int file,   [[maybe_unused]]int ptr,   [[maybe_unused]]int dir ) {return 0;}
    int open(  [[maybe_unused]]char *file, [[maybe_unused]]int flags, [[maybe_unused]] int mode ){return 0;}
    int read(  [[maybe_unused]]int file,   [[maybe_unused]]char *ptr, [[maybe_unused]]int len ){return 0;}
    int readlink( const char *path, char *buf, size_t bufsize );
    int stat( const char *file, struct stat *st );
    int symlink( const char *path1, const char *path2 );
    clock_t times( struct tms *buf );
    int unlink( char *name );
    int wait( int *status );
    int chown( const char *path, uid_t owner, gid_t group );
    int execve( char *name, char **argv, char **env );
    void exit( int rc );
    int fork( void );
    int gettimeofday( struct timeval *ptimeval, void *ptimezone );

}

static int __strlen(const char *str)
{
    if( !str ) { return 0; }

    int len = 0;
    while( str[len] != 0 )
    {
        len++;
    }

    return len;
}

static void* __malloc(size_t size) 
{
  void *request = sbrk(size);
  return request;
}

int __atoi(char* str)
{
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
}

static void __memcpy(char* a, char* b, size_t size)
{
    for(size_t i = 0; i<size; i++ )
    {
        a[i] = b[i];
    }
}

[[maybe_unused]] static void __memset(void* arr, char value, size_t size) 
{
    for(size_t i = 0; i < size; i++) {
        *((char*)arr + i) = value;
    }
}

[[maybe_unused]] static char *__strdup( const char * in )
{
    if( !in ) { return 0; }

    char *ret = (char*)__malloc( __strlen( in ) + 1 );
    __memcpy( ret, (char*)in, __strlen( in ) + 1 );

    return ret;
}

[[maybe_unused]] static int __strncmp( const char* a, const char*  b, int len )
{
    if( !a || !b ) { return 0; }

    int cur = 0;
    while( cur != len )
    {
        if( a[cur] != b[cur] ) { return 1; }
        if( a[cur] == 0 && b[cur] == 0 ) { return 0; }
        if( a[cur] == 0 || b[cur] == 0 ) { return 1; }

        cur++;
    }
    return 0;
}

[[maybe_unused]] static int __strcmp( const char * const a, const char * const b )
{
    return __strncmp( a, b, -1 );
}

[[maybe_unused]] static char __toupper(char a) 
{
    if(a >= 0x61 && a <= 0x7A)
        return a - 0x20;
    else   
        return 0x00;
}

[[maybe_unused]] static char __tolower(char a) 
{
    if(a >= 0x41 && a <= 0x5A)
        return a + 0x20;
    else   
        return 0x00;
}

/*borrowed from libdragon*/
extern "C" void *sbrk( int incr )
{
    static char * heap_end = 0;
    static char * heap_top = 0;
    char *        prev_heap_end;

    if( heap_end == 0 )
    {
        heap_end = &end;
        heap_top = (char*)((void*)0x80000000) + (*(int*)0xA0000318)  - 0x10000;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    if (heap_end > heap_top)
    {
        heap_end -= incr;
        prev_heap_end = (char *)-1;
        errno = ENOMEM;
    }


    return (void *)prev_heap_end;
}

#endif