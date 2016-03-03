#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef _bool_defined
#define _bool_defined
typedef enum{ false, true } bool;
#endif
#endif

void connect_to_compiler();
bool update_code();
void disconnect_from_compiler();

#ifdef __cplusplus
}
#endif

