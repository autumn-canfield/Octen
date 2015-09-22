#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include "types.h"
#include "os.h"
#include "error.h"
#include "messages.h"
#include "ipc.h"
#include "memory.h"
#include "library/include/octen.h"

#ifdef OS_WINDOWS
static const char *compiler_channel_name = "\\\\.\\pipe\\octen_program_channel";
#else
static const char *compiler_channel_name = ".octen_program_channel";
#endif

channel compiler_channel;

void connect_to_compiler()
{
   u64 magic_number = 0x3e3e6b6f3c3c2000;
   u64 response;

   compiler_channel = connect_to_channel(compiler_channel_name);
   if(!write_channel(compiler_channel, &magic_number, sizeof(u64)))
      error("Octen compiler disconnected!\n");
   if(!read_channel(compiler_channel, &response, sizeof(u64)))
      error("Octen compiler disconnected!\n");
   if(response != magic_number)
      error("Octen compiler didn't give the magic number!\n");
}

void *buffer;

// Returns true unless the compiler has disconnected.
bool update_code()
{
   if(bytes_in_channel(compiler_channel) == 0)
      return true;

   octen_message message;
   octen_message ack = create_message(0, CMD_ACK);
   if(!read_channel(compiler_channel, &message, sizeof(octen_message)))
      return false;
   u8 command = message_cmd(message);
   u32 size = message_size(message);
   void *address;

   switch(command)
   {
      case CMD_ALOC:
         address = allocate(0, size, PAGE_RW);
         if(!write_channel(compiler_channel, &address, sizeof(void*)))
            return false;
         break;
      case CMD_SET:
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         if(!read_channel(compiler_channel, &address, sizeof(void*)))
            return false;
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         if(!read_channel(compiler_channel, address, size))
            return false;
         break;
      case CMD_SET_EXEC:
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         if(!read_channel(compiler_channel, &address, sizeof(void*)))
            return false;
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         change_protection(address, size, PAGE_RW);
         if(!read_channel(compiler_channel, address, size))
         {
            printf("Failed reading block!\n");
            return false;
         }
         change_protection(address, size, PAGE_RX);
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         break;
      case CMD_GET:
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         if(!read_channel(compiler_channel, &address, sizeof(void*)))
            return false;
         change_protection(address, size, PAGE_RW);
         if(!write_channel(compiler_channel, address, size))
            return false;
         change_protection(address, size, PAGE_RX);
         break;
      case CMD_GET_EXEC:
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         if(!read_channel(compiler_channel, &address, sizeof(void*)))
            return false;
         if(!write_channel(compiler_channel, address, size))
            return false;
         break;
      case CMD_EVAL:
         printf("CMD_EVAL\n");
         break;
      case CMD_CALL:
         if(!write_channel(compiler_channel, &ack, sizeof(octen_message)))
            return false;
         if(!read_channel(compiler_channel, &address, sizeof(void*)))
            return false;
         u32 (*function)() = (u32 (*)())address;
         u64 result = function();
         printf("result: %u\n", (u32)result);
         if(!write_channel(compiler_channel, &result, sizeof(u64)))
            return false;
         break;
      case CMD_REFS:
         printf("CMD_REFS\n");
         break;
      case CMD_DEBUG:
         printf("CMD_DEBUG\n");
         break;
      default:
         printf("Compiler sent unknown message! (x%x)\n", message);
         return false;
         break;
   }

   return true;
}

void disconnect_from_compiler()
{
   close_channel(compiler_channel);
}

