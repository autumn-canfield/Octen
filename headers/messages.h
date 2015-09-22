#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "types.h"

typedef u32 octen_message;

// Compiler <-> Program commands
#define CMD_ACK   0 // Acknowledge previous message.
#define CMD_ALOC  1 // Allocate [size] bytes.
#define CMD_SET   2 // Set memory at [address] to [data].
#define CMD_GET   3 // Get [size] data at [address].
#define CMD_SET_EXEC 0x12 // Set executable memory at [address] to [data].
#define CMD_GET_EXEC 0x13 // Get executable [size] data at [address].
#define CMD_EVAL  4 //? Evaluate [data].
#define CMD_CALL  5 // Call function at [address].
#define CMD_REFS  6 // Update [size] references.
#define CMD_DEBUG 7 //? [size] data is an error message. Return: (msg) Ack.

// ================================= Protocol =================================
// Msg:              parameter(s):                       return/ack:
// ALOC  0xsize_001                                   -> (u64)address
// SET   0xsize_p02, (u64)address, u8[size]           -> ACK
// GET   0xsize_p03, (u64)address                     -> u8[size]
// EVAL  0xsize_004, u8[size]                         -> ACK? (Maybe return?)
// CALL  0x00000005, (u64)address                     -> (u64)return
// REFS  0xsize_006, (u32)delta, (u64)addresses[size] -> ACK
// DEBUG 0xsize_007, char[size]                       -> ACK

// Editor <-> Compiler commands (Neither implemented nor definite yet)
//#define CMD_ACK    0 // Acknowledge previous message.
//#define CMD_DISASM 1 // Request disassembly
//#define CMD_CODE   2 // Compile code

static inline u32 message_size(u32 m)
{
   return (m&0xfffff000) >> 12;
}

static inline u8 message_cmd(u32 m)
{
   return m&0xff;
}

static inline u32 create_message(u32 size, u8 cmd)
{
   return (size<<12) | cmd;
}

#endif // _MESSAGES_H
