/***************************
* Encoding: GB2312
***************************/
// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
HMODULE hinst;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	hinst = hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

