#pragma once
#ifndef _ChangeFunc_H_
#define _ChangeFunc_H_

#include <iostream>
#include <stdio.h>
#include <windows.h>

extern "C" __declspec(dllexport) void find_change_string(char* ProcName, char *OldStr, char* NewStr);
extern "C" __declspec(dllexport) void show_modules(HANDLE process);

#endif