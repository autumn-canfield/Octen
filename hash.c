#include "types.h"
#include "assert.h"
#include <string.h>

// This is a murmur2 hash.
u32 hash_string(const char* str)
{
	const u32 m = 0x5bd1e995;
	const i32 r = 24;
	u16 len = strlen(str);
	const u32 seed = 0x92c3cf54;
	u32 hash = len ^ seed;
	const u8 *data = (const u8 *)str;

	while (len >= 4)
	{
		u32 k = *(u32*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		hash *= m;
		hash ^= k;

		data += 4;
		len -= 4;
	}

	switch (len) {
		case 3: hash ^= data[2] << 16;
		case 2: hash ^= data[1] << 8;
		case 1: hash ^= data[0];
				hash *= m;
	}

	hash ^= hash >> 13;
	hash *= m;
	hash ^= hash >> 15;

   assert((hash!=0), 0, "Symbol '%s' has a hash of zero!\n", str);
	return hash;
}

u32 test_for_collisions(char** keys, u16 count, u32 table_size)
{
   u16 i, j;
   for(i = 0; i < count; ++i)
   {
      u32 x = hash_string(keys[i]);
      for(j = i + 1; j < count; ++j)
      {
         u32 y = hash_string(keys[j]);
         if(x % table_size == y % table_size)
         {
            u32 r = i << 16;
            return r | j;
         }
      }
   }
   return 0;
}

