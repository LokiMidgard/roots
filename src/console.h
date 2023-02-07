#ifndef ROOTS_CONSOLE_H
#define ROOTS_CONSOLE_H

#if !defined(PLATFORM_WEB)
#include "conio.h"
#include "stdio.h"
#endif

void InitConsole(void);
void CloseConsole(void);

#endif
