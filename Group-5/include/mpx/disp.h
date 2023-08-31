#ifndef MPX_DISP_H
#define MPX_DISP_H

#include <stddef.h>
#include <mpx/device.h>
#include <stdarg.h>
#include <string.h>
#include <sys_req.h>
#include <mpx/serial.h>
#include <mpx/io.h>
void disp(char *input);
void disp_int(int i);
void disp_ptr(int i);

#endif
