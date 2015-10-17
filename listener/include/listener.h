#ifndef _OCTEN_H
#define _OCTEN_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef _TYPES_H
typedef enum{ false, true } bool;
#endif
#endif

void connect_to_compiler();
bool update_code();
void disconnect_from_compiler();

#ifdef __cplusplus
}
#endif

#endif // _OCTEN_H
