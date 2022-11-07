@REM Ensure this is run in a context in which msbuild and cmake are defined
@REM Should be called from the root of the project: ./tools/build-windows.bat kvasir-tests

@REM Takes one argument: the project name being built. This also must be changed
@REM in the CMakeLists.txt file.

set arg1=%1

cmake .
msbuild %arg1%.sln