#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include "types.h"
#include "error.h"
#include "os.h"
#include "messages.h"
#include "hex.h"
#include "hash.h"
#include "table.h"
#include "memory.h"
#include "ipc.h"

#define COMPILE_BUFFER_SIZE 4194304 // 4mib

u8 *compile_buffer;

u32 read_token_to_buffer(const char *str, char *buffer, u32 buffer_size)
{
   u32 buffer_index = 0;
   while(str[buffer_index]!=' ' && str[buffer_index]!='\n' && str[buffer_index])
   {
      buffer[buffer_index] = str[buffer_index];
      ++buffer_index;
      if(buffer_index==buffer_size-1)
         error("Error: Token '%s...' exceeded maximum length!\n", buffer);
   }
   buffer[buffer_index]=0;
   return buffer_index;
}

void eval_string(const char *str, u8 *buffer)
{
   bool in_comment = false;
   u32 string_index = 0;
   u32 buffer_index = 0;

   char token_buffer[64];
   u32 token_length;

   while(str[string_index])
   {
      char c = str[string_index];

      if(c=='\\')
      {
         if(in_comment)
            in_comment = false;
         else
            in_comment = true;
      }else
      {
         if(in_comment)
            continue;

         token_length = read_token_to_buffer(&str[string_index], token_buffer,
               ARRAY_SIZE(token_buffer));

         switch(c)
         {
            case ',':
               string_index += token_length;
               buffer[buffer_index] = parse_hex_u8(&token_buffer[1]);
               buffer_index++;
               break;
            case '"':
               string_index++;
               printf("QUOTE\n");
               break;
            case ' ':
            case '\n':
               string_index++; // Ignore trailing whitespace.
               break;
            default:
               string_index += token_length;
               printf("TOKEN: '%s'\n", token_buffer);
               break;
         }
      }
   }
}

#ifdef OS_WINDOWS
static const char *program_channel_name = "\\\\.\\pipe\\octen_program_channel";
//static const char *editor_channel_name = "\\\\.\\pipe\\octen_editor_channel";
#else
static const char *program_channel_name = ".octen_program_channel";
//static const char *editor_channel_name = ".octen_editor_channel";
#endif

channel program_channel;

//,b8 ,2a ,00 ,00 ,00 ,c3
int main()
{
   u64 magic_number;
   octen_message message;

   program_channel = create_channel(program_channel_name, 0x4000, 0x4000, 30000);
   if(!read_channel(program_channel, &magic_number, sizeof(u64)))
      error("Client disconnected!\n");
   if(magic_number != 0x3e3e6b6f3c3c2000)
      error("Program didn't give the magic number!\n");
   if(!write_channel(program_channel, &magic_number, sizeof(u64)))
      error("Client disconnected!\n");

   compile_buffer = allocate(0, 0x1000, PAGE_RWX);

   void *program_function;
   message = create_message(0x1000, CMD_ALOC);
   if(!write_channel(program_channel, &message, sizeof(octen_message)))
      error("Client disconnected!\n");
   if(!read_channel(program_channel, &program_function, sizeof(void*)))
      error("Client disconnected!\n");

   while(true)
   {
      char input_buffer[256];
      printf("* ");
      //fflush(stdin);
      __fpurge(stdin);
      scanf("%255[^\n]", input_buffer);

      if(strcmp("quit", input_buffer)==0)
         break;

      if(strcmp("call", input_buffer)==0 && program_function)
      {
         message = create_message(0, CMD_CALL);
         if(!write_channel(program_channel, &message, sizeof(octen_message)))
            error("Client disconnected!\n");
         if(!write_channel(program_channel, &program_function, sizeof(void*)))
            error("Client disconnected!\n");
         u64 result;
         if(!read_channel(program_channel, &result, sizeof(u64)))
            error("Client disconnected!\n");
         printf("%u\n", (u32)result);
         continue;
      }

      eval_string(input_buffer, compile_buffer);
      message = create_message(0x1000, CMD_SET_EXEC);
      if(!write_channel(program_channel, &message, sizeof(octen_message)))
         error("Client disconnected!\n");
      if(!write_channel(program_channel, &program_function, sizeof(void*)))
         error("Client disconnected!\n");
      if(!write_channel(program_channel, compile_buffer, 0x1000))
         error("Client disconnected!\n");
      if(!read_channel(program_channel, &message, sizeof(octen_message)))
         error("Client disconnected!\n");
   }

   disconnect_channel(program_channel);
   return 0;
}

