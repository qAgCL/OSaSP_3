

#include "pch.h"
#include <iostream>
#include <string>
#include "ChangeFunc.h"
using namespace std;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call== DLL_PROCESS_ATTACH)
    {
	
    }
    return TRUE;
}

