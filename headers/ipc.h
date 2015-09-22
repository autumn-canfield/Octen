#ifndef _IPC_H
#define _IPC_H

#include "types.h"
#include "os.h"

#ifdef OS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>

   typedef HANDLE channel;
#else
   typedef int channel;
#endif

channel create_channel(const char *name, u32 out_buffer_size, u32 in_buffer_size, u32 timeout);
void close_channel(channel c);
channel connect_to_channel(const char *name);
void disconnect_channel(channel c);

// read_channel and write_channel return false if the channel has been closed.
bool read_channel(channel c, void *buffer, u32 buffer_length);
bool write_channel(channel c, void *buffer, u32 buffer_length);

u32 bytes_in_channel(channel c);

#endif // _IPC_H
