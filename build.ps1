& "clang" @("-I.\raylib\raylib\src", "-O0", "-std=c17", "$optimisation", "-g", "-gcodeview", "-gno-column-info",
            "-Wno-c2x-extensions", "-Wno-reserved-macro-identifier", "-fno-strict-aliasing", "-pedantic",
            "--output=.\dist\RELEASE\roots.exe",'-xc',"src\main.c",
            "-L.\raylib\raylib\build\raylib\",
            "-lmsvcrt", "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lkernel32", "-luser32", "-lshell32")

if ($?)
{
    ./dist/RELEASE/roots.exe
}
