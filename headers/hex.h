#ifndef _HEX_H
#define _HEX_H

#include"types.h"

u8 parse_nibble(char n);
u8 parse_hex_u8(char *str);
u16 parse_hex_u16(char *str);
u32 parse_hex_u32(char *str);
u64 parse_hex_u64(char *str);
bool is_valid_hex_string(char *str);

#endif // _HEX_H
