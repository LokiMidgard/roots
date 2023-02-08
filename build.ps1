param(
    [switch] $Debug
)

$rd = "$PSScriptRoot" # root directory
. "$rd\helper_functions.ps1"


$ninja                        = "$rd\tools\ninja.exe"
$build_directory              = "$rd\.build"
$external_directory           = "$rd\external"
$raylib_directory             = "$external_directory\raylib"
$libs_directory               = "$build_directory\libs"
$dist_directory               = "$rd\dist"
$release_directories          = @("$dist_directory\desktop-release",
                                  "$dist_directory\desktop-debug",
                                  "$dist_directory\web-release",
                                  "$dist_directory\web-debug")

create_directory("$build_directory")
# Make sure raylib is build
create_directory("$libs_directory")
if (-Not (Test-Path "$libs_directory\raylibd.lib"))
{
    Push-Location $build_directory
    cmake $raylib_directory -G "Ninja" -D BUILD_EXAMPLES=OFF -D CMAKE_MAKE_PROGRAM="$ninja" -D CMAKE_CXX_COMPILER="clang" -D CMAKE_C_COMPILER="clang"
    & $ninja
    cp ".\raylib\raylib.lib" "$libs_directory\raylibd.lib"
    Pop-Location
}

if (-Not (Test-Path "$libs_directory\raylib.lib"))
{
    Push-Location $build_directory
    cmake $raylib_directory -G "Ninja" -DCMAKE_BUILD_TYPE=Release -D BUILD_EXAMPLES=OFF -D CMAKE_MAKE_PROGRAM="$ninja" -D CMAKE_CXX_COMPILER="clang" -D CMAKE_C_COMPILER="clang"
    & $ninja
    cp ".\raylib\raylib.lib" "$libs_directory\raylib.lib"
    Pop-Location
}

create_directory("$dist_directory")
foreach ($directory in $release_directories)
{
    Write-Host "Creating $directory"
    create_directory("$directory")
}

if ($Debug)
{
    $optimisation = "-O0"

        & "clang" @("-I$raylib_directory\src", "-std=c17", "$optimisation", "-g", "-gcodeview", "-gno-column-info",
                    "-Wno-c2x-extensions", "-Wno-reserved-macro-identifier", "-fno-strict-aliasing", "-pedantic",
                    "--output=$build_directory\roots.exe",'-xc',"src\main.c",
                    "-L$libs_directory",
                    "-lmsvcrt", "-lraylibd", "-lopengl32", "-lgdi32", "-lwinmm", "-lkernel32", "-luser32", "-lshell32")

        if ($?)
        {
            cp "$build_directory\roots.exe" "$dist_directory\desktop-debug\"
            cp "$build_directory\roots.pdb" "$dist_directory\desktop-debug\"
            & "$dist_directory\desktop-debug\roots.exe"
        }
}
else
{
    $optimisation = "-O2"

        & "clang" @("-I$raylib_directory\src", "-std=c17", "$optimisation", "-g", "-gcodeview", "-gno-column-info",
                    "-Wno-c2x-extensions", "-Wno-reserved-macro-identifier", "-fno-strict-aliasing", "-pedantic",
                    "--output=$build_directory\roots.exe",'-xc',"src\main.c",
                    "-L$libs_directory",
                    "-lmsvcrt", "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lkernel32", "-luser32", "-lshell32")

        if ($?)
        {
            cp "$build_directory\roots.exe" "$dist_directory\desktop-release\"
            & "$dist_directory\desktop-release\roots.exe"
        }
}

