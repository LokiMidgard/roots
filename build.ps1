param(
    [switch] $Debug,
    [switch] $NoRun,
    [switch] $Html
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
#if (-Not (Test-Path "$libs_directory\raylibd.lib"))
#{
#    Push-Location $build_directory
#    cmake $raylib_directory -G "Ninja" -D BUILD_EXAMPLES=OFF -D CMAKE_MAKE_PROGRAM="$ninja" -D CMAKE_CXX_COMPILER="clang" -D CMAKE_C_COMPILER="clang"
#    & $ninja
#    cp ".\raylib\raylib.lib" "$libs_directory\raylibd.lib"
#    Pop-Location
#}

if (-Not (Test-Path "$libs_directory\raylib.lib"))
{
    Push-Location $build_directory
    cmake $raylib_directory -G "Ninja" -DCMAKE_BUILD_TYPE=Release -D BUILD_EXAMPLES=OFF -D CMAKE_MAKE_PROGRAM="$ninja" -D CMAKE_CXX_COMPILER="clang" -D CMAKE_C_COMPILER="clang"
    & $ninja
    cp ".\raylib\raylib.lib" "$libs_directory\raylib.lib"
    Pop-Location
}

if (-Not (Test-Path "$libs_directory\libraylib.a"))
{
    Push-Location $build_directory
    emcc -c "$raylib_directory/src/rcore.c" -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
    emcc -c "$raylib_directory/src/rshapes.c" -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
    emcc -c "$raylib_directory/src/rtextures.c" -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
    emcc -c "$raylib_directory/src/rtext.c" -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
    emcc -c "$raylib_directory/src/rmodels.c" -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
    emcc -c "$raylib_directory/src/utils.c" -Os -Wall -DPLATFORM_WEB
    emcc -c "$raylib_directory/src/raudio.c" -Os -Wall -DPLATFORM_WEB
    emar rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o
    cp libraylib.a "$libs_directory/libraylib.a"
    Pop-Location
}

create_directory("$dist_directory")
foreach ($directory in $release_directories)
{
    Write-Host "Creating $directory"
    create_directory("$directory")
}


if ($Html)
{
    $optimisation = "-O2"
        & "emcc" @("-I$raylib_directory\src", "-std=c17", "$optimisation", "--memory-init-file", "0", "-DPLATFORM_WEB",
                   "-Wno-c2x-extensions", "-Wno-reserved-macro-identifier", "-fno-strict-aliasing", "-pedantic",
                   "-o", "$build_directory\roots.html", "src\main.c", "$libs_directory\libraylib.a", "-s", "EXPORTED_FUNCTIONS=['_malloc', '_main']",
                   "--preload-file", "resources", "-s", "TOTAL_MEMORY=32MB", "-s", "ALLOW_MEMORY_GROWTH=1",
                   "-s", "USE_GLFW=3", "-L$libs_directory")
        cp "$build_directory\roots.html" "$dist_directory\web-release\"
        cp "$build_directory\roots.js" "$dist_directory\web-release\"
        cp "$build_directory\roots.wasm" "$dist_directory\web-release\"
        cp "$build_directory\roots.data" "$dist_directory\web-release\"
}
else
{
    if ($Debug)
    {
        $optimisation = "-O0"

            & "clang" @("-I$raylib_directory\src", "-std=c17", "$optimisation", "-g", "-gcodeview", "-gno-column-info",
                        "-Wno-c2x-extensions", "-Wno-reserved-macro-identifier", "-fno-strict-aliasing", "-pedantic",
                        "--output=$build_directory\roots.exe",'-xc',"src\main.c",
                        "-L$libs_directory",
                        "-lmsvcrt", "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lkernel32", "-luser32", "-lshell32")

            if ($? -and (-not $NoRun))
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

            if ($? -and (-not $NoRun))
            {
                cp "$build_directory\roots.exe" "$dist_directory\desktop-release\"
                    & "$dist_directory\desktop-release\roots.exe"
            }
    }
}
