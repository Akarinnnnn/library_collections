// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <combaseapi.h>
#include <wincodec.h>
#include <wil/com.h>
extern wil::com_ptr<IWICImagingFactory> WIC_Factory;

void ErrorMsgbox(const wchar_t * format, const HRESULT hr,int exitcode)
{
	wchar_t message[64]{ 0 };
	wsprintfW(message, format, hr);
	MessageBoxW(nullptr, message, L"什么情况？？？", MB_ICONERROR | MB_OK);
	exit(exitcode);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	HRESULT hr = 0;
	wchar_t message[64]{ 0 };
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
		if (FAILED(hr))
		{
			ErrorMsgbox(L"加载COM失败(hresult = %#08X)，即将退出", hr, 10);
		}
		if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_ALL, IID_IWICImagingFactory2, (LPVOID*)&WIC_Factory)))
		{
			hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_ALL, IID_IWICImagingFactory, (LPVOID*)&WIC_Factory);
			if (FAILED(hr))
			{
				ErrorMsgbox(L"创建WIC工厂失败(hresult = %#08X)，即将退出\n这个问题常见于精简版系统", hr, 11);
			}
		}
		WIC_Factory->AddRef();
	}
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		CoUninitialize();
        break;
    }
    return TRUE;
}

