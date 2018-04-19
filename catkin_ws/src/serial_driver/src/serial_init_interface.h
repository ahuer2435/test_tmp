#ifndef SERIAL_INIT_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define SIGSET    1
#define SIGUNSET  0

#include "mcumsg_rasp.h"
#include "thread_of_lcd.h"

int serial_init();

#ifdef __cplusplus
}
#endif

#define SERIAL_INIT_INTERFACE_H
#endif
