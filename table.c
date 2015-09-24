#include "table.h"
#include "error.h"
#include "types.h"
#include "memory.h"
#include <stdio.h>

static inline u32 probe_count(table t, u32 key, u32 i)
{
   u32 result;
   if((key%t.length) > i)
      result = t.length+i-(key%t.length);
   else
      result = i-(key%t.length);
   return result;
}

u64 table_get_value(table t, u32 key)
{
   u32 n;
   table_entry entry;
   for(n=0; n<t.length; n++)
   {
      u32 i = (key+n)%t.length;
      entry = t.data[i];
      if(entry.key == 0)
         return 0;
      if(probe_count(t, entry.key, i) < n)
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
      u32 i = (key+n)%t.length;
      entry = t.data[i];
      if(entry.key == 0)
      {
         t.data[i].key = key;
         t.data[i].value = value;
         return;
      }
      if(entry.key == key)
      {
         t.data[i].value = value;
         return;
      }
      if(probe_count(t, entry.key, i) < n)
      {
         table_entry tmp = entry;
         t.data[i].key = key;
         t.data[i].value = value;
         key = tmp.key;
         value = tmp.value;
         n = probe_count(t, entry.key, i);
      }
   }
   error("Error: Too many elements in table! (data=%u)\n", t.data);
}

