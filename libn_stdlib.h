/*super basic functions to get us going up until newlib implementation*/

#include <stdarg.h>
#include <malloc.h>
#include <sys/types.h>
#include <limits.h>
#include <dir.h>
#include <errno.h>

extern char end __attribute__((section (".data")));
int errno __attribute__((weak));

static int   __strlen(const char*); 
static void* __malloc(size_t size);
static void  __memcpy(char*, char* b, size_t len);
static void  __memset(void*, char value, size_t size);
static char* __strdup(const char* );
static int   __strncmp(const char*, const char*, int);
static int   __strcmp(const char* a, const char*);
static char* __itoa(int num, int base);
static char  __toupper(char a);
static char  __tolower(char a);
extern "C" void  *sbrk(int incr);
extern "C" 
{
    void _exit(int){}
    int close( int fildes ){}
    int chown( const char *path, uid_t owner, gid_t group );
    int execve( char *name, char **argv, char **env );
    void exit( int rc );
    int fork( void );
    int fstat( int fildes, struct stat *st ){}
    int getpid( void ){}
    int gettimeofday( struct timeval *ptimeval, void *ptimezone );
    int isatty( int file ){}
    int kill( int pid, int sig ){}
    int link( char *existing, char *neww ){}
    int lseek( int file, int ptr, int dir ){}
    int open( char *file, int flags, int mode );
    int read( int file, char *ptr, int len ){}
    int readlink( const char *path, char *buf, size_t bufsize );
    int stat( const char *file, struct stat *st );
    int symlink( const char *path1, const char *path2 );
    clock_t times( struct tms *buf );
    int unlink( char *name );
    int wait( int *status );
    int write( int file, char *ptr, int len ){}
    void __assert(const char *, int, const char *);
    void __assert_func(
    const char *file, int line, const char *, const char *e) {
    __assert(file, line, e);
    }
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
    for( int i = 0; i<size; i++ )
    {
        a[i] = b[i];
    }
}

static void __memset(void* arr, char value, size_t size) 
{
    for(int i =0;i<size;i++) {
        *((char*)arr + i) = value;
    }
}

static char *__strdup( const char * in )
{
    if( !in ) { return 0; }

    char *ret = (char*)__malloc( __strlen( in ) + 1 );
    __memcpy( ret, (char*)in, __strlen( in ) + 1 );

    return ret;
}

static int __strncmp( const char* a, const char*  b, int len )
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

static int __strcmp( const char * const a, const char * const b )
{
    return __strncmp( a, b, -1 );
}

static char __toupper(char a) 
{
    if(a >= 0x61 && a <= 0x7A)
        return a - 0x20;
    else   
        return 0x00;
}

static char __tolower(char a) 
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

