#include "listener.h"

int main()
{
   connect_to_compiler();
   while(update_code()){}
   disconnect_from_compiler();
}

