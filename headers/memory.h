#pragma once
#include "types.h"
#include "os.h"

#ifdef OS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   
   #define PAGE_NONE PAGE_NOACCESS
   #define PAGE_R PAGE_READONLY
   #define PAGE_X PAGE_EXECUTE
   #define PAGE_RW PAGE_READWRITE
   #define PAGE_RX PAGE_EXECUTE_READ
   #define PAGE_RWX PAGE_EXECUTE_READWRITE
#else
   #include <unistd.h>
   #include <sys/mman.h>

   #define PAGE_NONE PROT_NONE
   #define PAGE_R PROT_READ
   #define PAGE_X PROT_EXEC
   #define PAGE_RW PROT_READ|PROT_WRITE
   #define PAGE_RX PROT_READ|PROT_EXEC
   #define PAGE_RWX PROT_READ|PROT_WRITE|PROT_EXEC
#endif

void *allocate(void *address, size_t size, i32 protection);
bool deallocate(void *address, size_t size);
bool change_protection(void *address, u32 size, i32 protection);
u32 page_size();

