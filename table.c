#include "table.h"
#include "error.h"
#include "types.h"
#include "memory.h"
#include <stdio.h>

static inline u32 probe_count(table t, u32 key, u32 n)
{
   u32 result;
   if(key%t.length>n)
      result = t.length+(key%t.length)-n;
   else
      result = n-(key%t.length);
   return result;
}

u64 table_get_value(table t, u32 key)
{
   u32 i = key%t.length;
   u32 n;
   table_entry entry;

   u32 max_probe = t.address[t.length].key;

   for(n=0; n<=max_probe; n++)
   {
      entry = t.address[(i+n)%t.length];
      if(entry.key == 0)
         return 0;
      if(probe_count(t, entry.key, n) < n)
         return 0;
      if(entry.key == key)
         return entry.value;
   }

   return 0;
}

void table_set_value(table t, u32 key, u64 value)
{
   u32 n;
   table_entry entry;

   for(n=0; n<t.length; n++)
   {
      u32 i = ((key%t.length)+n)%t.length;

      if(t.address[t.length].key < n) // Update max probe length.
         t.address[t.length].key = n;

      entry = t.address[i];

      if(entry.key == 0)
      {
         t.address[i].key = key;
         t.address[i].value = value;
         return;
      }

      if(entry.key == key)
      {
         t.address[i].value = value;
         return;
      }

      if(probe_count(t, entry.key, n) < n)
      {
         table_entry tmp = entry;
         t.address[i].key = key;
         t.address[i].value = value;
         key = tmp.key;
         value = tmp.value;
      }
   }

   error("Error: Too many elements in table! (address=%u)\n", t.address);
}

