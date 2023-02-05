#ifndef ROOTS_CONSOLE_H
#define ROOTS_CONSOLE_H

#if !defined(PLATFORM_WEB)
#include <conio.h>
#endif

void InitConsole();
void CloseConsole();

#endif