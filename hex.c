#include "types.h"

u8 parse_nibble(char n)
{
   if(n&0x10)
      return n&0x0f;
   else
      return n-0x57;
}

u8 parse_hex_u8(char *str)
{
   u8 tmp = 0;
   u8 i;
   for(i=0; str[i]&&i<2; ++i)
      tmp = (tmp<<4) | parse_nibble(str[i]);
   return tmp;
}

u16 parse_hex_u16(char *str)
{
   u16 tmp = 0;
   u8 i;
   for(i=0; str[i]&&i<4; ++i)
      tmp = (tmp<<4) | parse_nibble(str[i]);
   return tmp;
}

u32 parse_hex_u32(char *str)
{
   u32 tmp = 0;
   u8 i;
   for(i=0; str[i]&&i<8; ++i)
      tmp = (tmp<<4) | parse_nibble(str[i]);
   return tmp;
}

u64 parse_hex_u64(char *str)
{
   u64 tmp = 0;
   u8 i;
   for(i=0; str[i]&&i<16; ++i)
      tmp = (tmp<<4) | parse_nibble(str[i]);
   return tmp;
}

bool is_valid_hex_string(char *str)
{
   u32 i;
   for(i=0; str[i]; ++i)
   {
      char c = str[i];
      if(c<48||(c>57&&c<97)||c>102)
         return false;
   }
   return true;
}

