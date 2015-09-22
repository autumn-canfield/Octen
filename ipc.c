#include "ipc.h"
#include "types.h"
#include "os.h"
#include "error.h"

#ifndef OS_WINDOWS
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

channel create_channel(const char *name, u32 out_buffer_size,
      u32 in_buffer_size, u32 timeout)
{
   channel c;
#ifdef OS_WINDOWS
   c = CreateNamedPipe(name, PIPE_ACCESS_DUPLEX,
         PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1,
         out_buffer_size, in_buffer_size, timeout, NULL);
   if(c == INVALID_HANDLE_VALUE)
      error("Couldn't create communication channel!\n");
   if(!ConnectNamedPipe(c, NULL))
   {
      CloseHandle(c);
      error("Couldn't connect!\n");
   }
#else
   // Windows wants buffer sizes and a timeout, but we don't need them here.
   (void)out_buffer_size;
   (void)in_buffer_size;
   (void)timeout;
   int tmp_sock;
   struct sockaddr_un local, remote;
   if((tmp_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
      error("socket\n");
   local.sun_family = AF_UNIX;
   strcpy(local.sun_path, name);
   unlink(name);
   if(bind(tmp_sock, (struct sockaddr *)&local, strlen(name)+sizeof(local.sun_family)) == -1)
      error("bind\n%s\n", strerror(errno));
   if(listen(tmp_sock, 1) == -1)
      error("listen\n");
   u32 t = sizeof(remote);
   if ((c = accept(tmp_sock, (struct sockaddr *)&remote, &t)) == -1)
      error("accept\n");
   close(tmp_sock);
#endif
   return c;
}

void close_channel(channel c)
{
#ifdef OS_WINDOWS
   CloseHandle(c);
#else
   close(c);
#endif
}

channel connect_to_channel(const char *name)
{
   channel c;
#ifdef OS_WINDOWS
   c = CreateFile(name, GENERIC_READ|GENERIC_WRITE,0, NULL, OPEN_EXISTING,
         0, NULL);
   if(c != INVALID_HANDLE_VALUE)
      return c;
   if(GetLastError() != ERROR_PIPE_BUSY)
      error("Couldn't connect!\n gle=%lu\n", GetLastError());
   if(!WaitNamedPipe(name, 20000))
      error("Couldn't connect! 20 second wait timed out.\n");
   return c;
#else
   struct sockaddr_un remote = {.sun_family = AF_UNIX};
   if((c = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
      error("socket\n");
   strcpy(remote.sun_path, name);
   if(connect(c, (struct sockaddr *)&remote, strlen(name) + sizeof(remote.sun_family)) == -1)
      error("connect\n");
   return c;
#endif
}

void disconnect_channel(channel c)
{
#ifdef OS_WINDOWS
   FlushFileBuffers(c);
   DisconnectNamedPipe(c);
#else
   FILE *f = fdopen(c, "rwb");
   fflush(f);
   fclose(f);
#endif
   close_channel(c);
}

// Returns false if channel has been closed
bool read_channel(channel c, void *buffer, u32 buffer_length)
{
#ifdef OS_WINDOWS
   unsigned long num_bytes_read;
   bool result = ReadFile(c, buffer, buffer_length, &num_bytes_read, NULL);
   if(num_bytes_read == 0)
      error("Read from channel failed!\n");
   return result;
#else
   int result = recv(c, buffer, (size_t)buffer_length, 0);
   if(result == 0 && buffer_length != 0)
      return false;
   else
      return true;
#endif
}

// Returns false if channel has been closed
// Actually on unix it returns false if there was an error...
bool write_channel(channel c, void *buffer, u32 buffer_length)
{
#ifdef OS_WINDOWS
   unsigned long num_bytes_written;
   bool result = WriteFile(c, buffer, buffer_length, &num_bytes_written, NULL);
   if(num_bytes_written != buffer_length)
      error("Write to channel failed!\nWrote %u of %u bytes.\n",
            num_bytes_written, buffer_length);
   return result;
#else
   int result = send(c, buffer, (size_t)buffer_length, 0);
   if(result == -1)
   {
      printf("Write to channel failed: %s\n", strerror(errno));
      return false;
   }
   else
   {
      return true;
   }
#endif
}

u32 bytes_in_channel(channel c)
{
#ifdef OS_WINDOWS
   unsigned long result;
   if(!PeekNamedPipe(c, NULL, 0, NULL, &result, NULL))
      error("Couldn't get bytes left in channel!\n GetLastError() = %u\n",
            GetLastError());
   return (u32) result;
#else
   int result;
   ioctl(c, FIONREAD, &result);
   return result;
#endif
}

