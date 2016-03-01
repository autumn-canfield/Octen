#pragma once
#include<stdio.h>
#include<stdlib.h>
#if defined DEBUG || defined DEVELOP
   #ifndef PARANOIA
      #define PARANOIA 1
   #endif
   
   #define assert(exp, level, message, ...) do{if(PARANOIA>=level&&!(exp)){\
      printf(message, ##__VA_ARGS__); exit(-1);}}while(0)
   
   #define __COMBINE_HELPER_1(x, y) x ## y
   #define __COMBINE_HELPER(x, y) __COMBINE_HELPER_1(x, y)
   #define UNIQUE_NAME(prefix) __COMBINE_HELPER(prefix, __LINE__)
   
   #define assert_once(exp, level, message, ...)\
      static unsigned char UNIQUE_NAME(assert_fired_p)=0;\
      do{if(UNIQUE_NAME(assert_fired_p)==0&&PARANOIA>=level&&!(exp)){\
      UNIQUE_NAME(assert_fired_p)=1; printf(message, ##__VA_ARGS__); exit(-1);}}while(0)
#else
   #define assert(...) ((void) 0)
   #define assert_once(...) ((void) 0)
#endif

