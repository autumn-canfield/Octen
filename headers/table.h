#ifndef _TABLE_H
#define _TABLE_H

#include "types.h"
#include "memory.h"

typedef struct {
   u64 value;
   u32 key;
} table_entry;

typedef struct {
   table_entry *address;
   u32 length;
} table;

static inline table create_table(u32 length)
{
   table t;
   // Allocate one extra entry at the end to store the max probe length.
   t.address = allocate(0, sizeof(table_entry)*(length+1), PAGE_RW);
   t.length = length;
   return t;
}
u64 table_get_value(table t, u32 key);
void table_set_value(table t, u32 key, u64 value);

#endif // _TABLE_H
