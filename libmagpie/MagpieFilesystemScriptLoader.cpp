/****************************************************************************
 * MagpieFilesystemScriptLoader.cpp : Implementation of
 * CMagpieFilesystemScriptLoader.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "StdAfx.h"
#include "MagpieFilesystemScriptLoader.h"

/*============================================================================
 * class CMagpieFilesystemScriptLoader
 */

//----------------------------------------------------------------------------
//  static CreateObject
HRESULT CMagpieFilesystemScriptLoader::CreateObject(
  LPCOLESTR                                   lpsRootPath,
  CMagpieFilesystemScriptLoaderComObject  *&  pRet)
{
  CMagpieFilesystemScriptLoaderComObject *newObject = pRet = NULL;
  IF_FAILED_RET(CMagpieFilesystemScriptLoaderComObject
              ::CreateInstance(&newObject));
  newObject->AddRef();

  HRESULT hr = E_FAIL;
  hr = newObject->Init(lpsRootPath);
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
CMagpieFilesystemScriptLoader::CMagpieFilesystemScriptLoader()
{
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieFilesystemScriptLoader::FinalConstruct()
{
	return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieFilesystemScriptLoader::FinalRelease()
{
  int asd = 0;
}

HRESULT CMagpieFilesystemScriptLoader::Init(LPCOLESTR lpsRootPath)
{
  if (!PathFileExists(lpsRootPath))
  {
    return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
  }
  m_sRootPath = lpsRootPath;
  if (m_sRootPath.IsEmpty())
  {
    return E_INVALIDARG;
  }
  if (m_sRootPath[m_sRootPath.GetLength()-1] != _T('\\'))
  {
    m_sRootPath += _T('\\');
  }
  return S_OK;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieScriptLoader implementation

//----------------------------------------------------------------------------
//  GetModuleScript
STDMETHODIMP CMagpieFilesystemScriptLoader::GetModuleScript(
  const OLECHAR* lpszModuleID, BSTR * pbsScript)
{
  ENSURE_RETVAL(pbsScript)

  // create a real path from the module ID...
  CString sModulePath, sModuleIDPath(lpszModuleID);
  sModuleIDPath.Replace(_T('/'), _T('\\'));
  sModulePath = m_sRootPath + sModuleIDPath;

  // ...and append the file extension 'js' in case no extension is given
  // @TODO: Might change in future.
  LPCTSTR lpszExtension = PathFindExtension(sModulePath);
  if (!_tcslen(lpszExtension))
  {
    sModulePath += _T(".js");
  }

  if (!PathFileExists(sModulePath))
  {
    return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
  }

  CAtlFile f;

  IF_FAILED_RET(f.Create(
    sModulePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING));

  ULONGLONG nLen = 0;
  HRESULT hr = E_FAIL;

  IF_FAILED_RET(f.GetSize(nLen));
  // limit script size to 4GB
  if (nLen > 0x00000000ffffffff)
	  return E_OUTOFMEMORY;

  DWORD dwLen = (DWORD)(nLen & 0x00000000ffffffff);
  CStringA sImpl;
  LPVOID lpv = sImpl.GetBuffer(dwLen);
  if (!lpv)
	  return E_OUTOFMEMORY;
  hr = f.Read(lpv, dwLen);
  sImpl.ReleaseBuffer(dwLen);
  f.Close();
  IF_FAILED_RET(hr);

  (*pbsScript) = SysAllocString(CA2WEX<4096>(sImpl /*, CODEPAGE*/));
  return (*pbsScript) ? S_OK : E_OUTOFMEMORY;
}

