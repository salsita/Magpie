// Magpie.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "Magpie_i.h"
#include "dllmain.h"
#include "MagpieApplication.h"

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user
//              per machine.	
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = _T("user");

    if (pszCmdLine != NULL)
    {
    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
    	{
    		AtlSetPerUserRegistration(true);
    	}
    }

    if (bInstall)
    {	
    	hr = DllRegisterServer();
    	if (FAILED(hr))
    	{	
    		DllUnregisterServer();
    	}
    }
    else
    {
    	hr = DllUnregisterServer();
    }

    return hr;
}


STDAPI CreateMagpieInstance(IMagpieApplication** ppRet)
{
  if (!ppRet) {
    return E_POINTER;
  }
  (*ppRet) = NULL;
  CComPtr<IClassFactory> classFactory;
  HRESULT hr = _AtlModule.DllGetClassObject(CLSID_MagpieApplication, IID_IClassFactory, (void**)&classFactory.p);
  if (FAILED(hr)) {
    return hr;
  }
  return classFactory->CreateInstance(NULL, IID_IMagpieApplication, (void**)ppRet);
}