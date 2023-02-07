#if !defined(PLATFORM_WEB)

int __stdcall AllocConsole(void);
void __stdcall FreeConsole(void);

void InitConsole() {
    FILE *ConsoleIn  = 0;
    FILE *ConsoleOut = 0;
    FILE *ConsoleErr = 0;
    AllocConsole();
    freopen_s(&ConsoleIn, "CONIN$", "r", stdin);
    freopen_s(&ConsoleOut, "CONOUT$", "w", stdout);
    freopen_s(&ConsoleErr, "CONOUT$", "w", stderr);
}

void CloseConsole() {
    FreeConsole();
}
#else

void InitConsole() {}
void CloseConsole() {}

#endif
