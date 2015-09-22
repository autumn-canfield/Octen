#include "memory.h"
#include "types.h"
#include "os.h"

void *allocate(void *address, size_t size, i32 protection)
{
#ifdef OS_WINDOWS
   return VirtualAlloc(address, size, MEM_COMMIT|MEM_RESERVE, (u32)protection);
#else
   return mmap(address, size, protection, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
}

bool deallocate(void *address, size_t size)
{
#ifdef OS_WINDOWS
   return VirtualFree(address, size, MEM_DECOMMIT);
#else
   i32 result = munmap(address, size);
   if(result == -1)
      return false;
   else
      return true;
#endif
}

bool change_protection(void *address, u32 size, i32 protection)
{
#ifdef OS_WINDOWS
   DWORD old_protection;
   return VirtualProtect(address, size, (u32)protection, &old_protection);
#else
   i32 result = mprotect(address, size, protection);
   if(result == -1)
      return false;
   else
      return true;
#endif
}

// Pages are almost always 4kib in x86, but 2mib and 1gib pages are supported
// in long mode. Additionally, 4mib pages are supported in 32 bit protected
// mode.
u32 page_size()
{
#ifdef OS_WINDOWS
   SYSTEM_INFO si;
   GetSystemInfo(&si);
   return si.dwPageSize;
#else
   return (u32)sysconf(_SC_PAGESIZE);
#endif
}

