/****************************************************************************
 * MagpieResourceScriptLoader.cpp : Implementation of
 * CMagpieResourceScriptLoader.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "StdAfx.h"
#include "MagpieResourceScriptLoader.h"

/*============================================================================
 * class CMagpieResourceScriptLoader
 */

//----------------------------------------------------------------------------
//  static CreateObject
HRESULT CMagpieResourceScriptLoader::CreateObject(
  HMODULE                                   hModule,
  CMagpieResourceScriptLoaderComObject  *&  pRet)
{
  CMagpieResourceScriptLoaderComObject *newObject = pRet = NULL;
  IF_FAILED_RET(CMagpieResourceScriptLoaderComObject
              ::CreateInstance(&newObject));
  newObject->AddRef();

  HRESULT hr = E_FAIL;
  hr = newObject->Init(hModule);
  if(FAILED(hr))
  {
    newObject->Release();
    return hr;
  }

  pRet = newObject;
  return S_OK;
}

//----------------------------------------------------------------------------
//  CTOR
CMagpieResourceScriptLoader::CMagpieResourceScriptLoader() :
  m_hModule(NULL)
{
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieResourceScriptLoader::FinalConstruct()
{
	return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieResourceScriptLoader::FinalRelease()
{
  int asd = 0;
}

HRESULT CMagpieResourceScriptLoader::ResolveModuleID(LPCOLESTR lpszModuleID, CString * psRet)
{
  if (!m_hModule)
  {
    return E_UNEXPECTED;
  }

  // Adjust the module id. Forward slashes are not allowed in resource
  // identifiers, so we use '|' instead.
  CString sModuleIDPath(lpszModuleID);
  sModuleIDPath.Replace(_T('/'), _T('|'));

  HRSRC hRes = FindResource(
    m_hModule, sModuleIDPath, MAKEINTRESOURCE(RT_HTML));
  if(hRes)
  {
    if (psRet)
    {
      (*psRet) = sModuleIDPath;
    }
    return S_OK;
  }
  return S_FALSE;
}

HRESULT CMagpieResourceScriptLoader::Init(HMODULE hModule)
{
  m_hModule = hModule;
  return S_OK;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieScriptLoader implementation

//----------------------------------------------------------------------------
//  HasModuleScript
STDMETHODIMP CMagpieResourceScriptLoader::HasModuleScript(
  const OLECHAR* lpszModuleID)
{
  return ResolveModuleID(lpszModuleID);
}


//----------------------------------------------------------------------------
//  GetModuleScript
STDMETHODIMP CMagpieResourceScriptLoader::GetModuleScript(
  const OLECHAR* lpszModuleID, BSTR * pbsScript)
{
  ENSURE_RETVAL(pbsScript)
  if (!m_hModule)
  {
    return E_UNEXPECTED;
  }

  // Adjust the module id. Forward slashes are not allowed in resource
  // identifiers, so we use '|' instead.
  CString sModuleIDPath(lpszModuleID);
  sModuleIDPath.Replace(_T('/'), _T('|'));

  HRSRC hRes = FindResource(
    m_hModule, sModuleIDPath, MAKEINTRESOURCE(RT_HTML));
  if (hRes)
  {
	  DWORD dwSize = SizeofResource(m_hModule, hRes);
	  HGLOBAL hResLoad = LoadResource(m_hModule, hRes);
	  if (hResLoad)
	  {
		  LPVOID lpResLock = LockResource(hResLoad);
		  if (lpResLock)
		  {
        LPSTR lps = (LPSTR)new BYTE[dwSize + 1];
			  if (!lps)
				  return E_OUTOFMEMORY;
			  memcpy(lps, lpResLock, dwSize);
        lps[dwSize] = 0;
        (*pbsScript) = SysAllocString(CA2WEX<4096>(lps /*, CODEPAGE*/));
        delete [] lps;
        return (*pbsScript) ? S_OK : E_OUTOFMEMORY;
		  }
	  }
  }
  return HRESULT_FROM_WIN32(::GetLastError());
}

