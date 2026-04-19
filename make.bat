@echo off
g++ main.cpp -o main -Iinclude -L. -lglfw3 -lglad -static-libgcc -static-libstdc++ -lSDL2 -lSDL2_image

if errorlevel 1 goto ERROR
if [%1] == [-r] main.exe 
exit /b 0

:ERROR
cmd /k
exit /b 1