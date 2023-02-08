. "$PSScriptRoot\helper_functions.ps1"

$external_libraries_directory = "./external"
$raylib_dir = "$external_libraries_directory/raylib"
$external_libraries = @("raylib.lib")

$dist_directory = "./dist"

create_directory("$dist_directory")
create_directory("$dist_directory/desktop-release")
create_directory("$dist_directory/desktop-debug")
create_directory("$dist_directory/web-release")
create_directory("$dist_directory/web-debug")

$optimisation = "-O2"

& "clang" @("-I.\raylib\src", "-std=c17", "$optimisation", "-g", "-gcodeview", "-gno-column-info",
            "-Wno-c2x-extensions", "-Wno-reserved-macro-identifier", "-fno-strict-aliasing", "-pedantic",
            "--output=.\dist\RELEASE\roots.exe",'-xc',"src\main.c",
            "-L.\raylib\raylib\build\raylib\",
            "-lmsvcrt", "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lkernel32", "-luser32", "-lshell32")

if ($?)
{
    ./dist/RELEASE/roots.exe
}





