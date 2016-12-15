#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <stdio.h>
#include <string.h>

#include "encoding.h"

//#include "shared.h"
#include "core_uart_apb.h"

/******************************************************************************
 * CoreUARTapb instance data.
 *****************************************************************************/
UART_instance_t g_uart;

#undef errno
int errno;

char *__env[1] = { 0 };
char **environ = __env;

volatile uint64_t tohost __attribute__((aligned(64)));
volatile uint64_t fromhost __attribute__((aligned(64)));

extern UART_instance_t g_uart;

void write_hex(int fd, uint32_t hex){
  uint8_t ii;
  uint8_t jj;
  char towrite;
  write( fd , "0x", 2 );
  for (ii = 8 ; ii > 0; ii--){
    jj = ii-1;
    uint8_t digit = ((hex & (0xF << (jj*4))) >> (jj*4));
    towrite = digit < 0xA ? ('0' + digit) : ('A' +  (digit - 0xA));
    write( fd, &towrite, 1);
  }

}

               
void _exit(int code)
{
  const char * message = "\nProgam has exited with code:";
  
  write(STDERR_FILENO, message, strlen(message));
  write_hex(STDERR_FILENO, code);

  while (1);
    
}

void *sbrk(ptrdiff_t incr)
{
  extern char _end[];
  extern char _heap_end[];
  static char *curbrk = _end;

  if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
    return NULL - 1;

  curbrk += incr;
  return curbrk - incr;
}

static int stub(int err)
{
  errno = err;
  return -1;
}

int open(const char* name, int flags, int mode)
{
  return stub(ENOENT);
}

int openat(int dirfd, const char* name, int flags, int mode)
{
  return stub(ENOENT);
}

int close(int fd)
{
  return stub(EBADF);
}

int execve(const char* name, char* const argv[], char* const env[])
{
  return stub(ENOMEM);
}

int fork()
{
  return stub(EAGAIN);
}

int fstat(int fd, struct stat *st)
{
  if (isatty(fd)) {
    st->st_mode = S_IFCHR;
    return 0;
  }

  return stub(EBADF);
}

int getpid()
{
  return 1;
}

int isatty(int fd)
{
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    return 1;

  errno = EBADF;
  return 0;
}

int kill(int pid, int sig)
{
  return stub(EINVAL);
}

int link(const char *old_name, const char *new_name)
{
  return stub(EMLINK);
}

off_t lseek(int fd, off_t ptr, int dir)
{
  if (isatty(fd))
    return 0;

  return stub(EBADF);
}

ssize_t read(int fd, void* ptr, size_t len)
{
  if (isatty(fd))
    return UART_get_rx(&g_uart,
                       (uint8_t*) ptr,
                       len);

  return stub(EBADF);
}

int stat(const char* file, struct stat* st)
{
  return stub(EACCES);
}

clock_t times(struct tms* buf)
{
  return stub(EACCES);
}

int unlink(const char* name)
{
  return stub(ENOENT);
}

int wait(int* status)
{
  return stub(ECHILD);
}

ssize_t write(int fd, const void* ptr, size_t len)
{

  const uint8_t * current = (const uint8_t *) ptr;
  size_t jj;
  if (isatty(fd)) {
    
    for (jj = 0; jj < len; jj++){
      
      UART_send(&g_uart, current + jj, 1);
      if (current[jj] == '\n'){
        UART_send(&g_uart, (const uint8_t *)"\r", 1);
      }
    }
    return len;
  } 
  
  return stub(EBADF);
}
