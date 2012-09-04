/****************************************************************************
 * MagpieApplication.cpp : Implementation of CMagpieApplication
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "stdafx.h"
#include "MagpieApplication.h"
#include "DispExIdManager.h"
#include "MagpieFilesystemScriptLoader.h"
#include "MagpieResourceScriptLoader.h"

/*============================================================================
 * class CMagpieApplication
 */

//----------------------------------------------------------------------------
//  CTOR
CMagpieApplication::CMagpieApplication() :
  m_Console(*this),
  m_ScriptEngine(*this),
  m_ConsolePtr(NULL)
{
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieApplication::FinalConstruct()
{
  // prepare the aggregated pointer
  m_Console.QueryInterface(IID_IUnknown, (void**)&m_ConsolePtr);
  return Init();
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieApplication::FinalRelease()
{
  Shutdown();
  if (m_ConsolePtr)
  {
    m_ConsolePtr->Release();
    m_ConsolePtr = NULL;
  }
}

//----------------------------------------------------------------------------
//  ResolveModuleID
HRESULT CMagpieApplication::ResolveModuleID(
  CMagpieModule * pSrcModule,
  LPCOLESTR       lpszModuleID,
  CString       & sAbsoluteModuleID)
{
  if (!lpszModuleID || !_tcslen(lpszModuleID))
  {
    // empty id? no way!
    return E_FAIL;
  }

  // if ID is already absolute...
  if (lpszModuleID[0] != _T('.'))
  {
    // ...return it as it is...
    sAbsoluteModuleID = lpszModuleID;
    return S_OK;
  }

  HRESULT hr = E_FAIL;
  CString sSrcModuleID;
  // if no source module is given id will be 
  // relative to root (means sSrcModuleID will be empty)
  if (pSrcModule)
  {
    pSrcModule->GetID(sSrcModuleID);
  }

  CAtlList<CString> srcIdList, idList;

  SplitString(lpszModuleID, _T("/"), idList);
  SplitString(sSrcModuleID, _T("/"), srcIdList);
  if (!srcIdList.IsEmpty())
  {
    // Remove last part, it is the "filename" part of the srcID.
    // dstID should be relative to the "folder" of srcID
    srcIdList.RemoveTailNoReturn();
  }

  // Adjust the srcIdList according to the parts in idList.
  // Any './' will change nothing.
  // Any '../' will pop off the tail of srcIdList.
  // Anything else will be appended to srcIdList.
  POSITION pos = idList.GetHeadPosition();
  while(pos)
  {
    const CString & cur = idList.GetNext(pos);
    if (cur == _T("."))
    {
      // current folder, do nothing
      continue;
    }
    if (cur == _T(".."))
    {
      // one up, remove tail from srcIdList
      if (srcIdList.IsEmpty())
      {
        // Oops, already top level.
        // Return an error.
        return E_FAIL;
      }
      srcIdList.RemoveTailNoReturn();
    }
    else
    {
      // append
      srcIdList.AddTail(cur);
    }
  }

  // Now compose final id.
  // sAbsoluteModuleID does NOT get emptied before, allowing to pass in
  // a parent path.
  pos = srcIdList.GetHeadPosition();
  while(pos)
  {
    sAbsoluteModuleID.Append(idList.GetNext(pos));
    if (pos)
    {
      sAbsoluteModuleID.AppendChar(_T('/'));
    }
  }
  return S_OK;
}

//----------------------------------------------------------------------------
//  GetModule
HRESULT CMagpieApplication::GetModule(
  LPCOLESTR          lpszModuleID,
  IMagpieModule  *&  pRet)
{
  CComPtr<IMagpieModule> module;
  if (!lpszModuleID)
  {
    // No module ID, get first loaded ("main") module:
    POSITION pos = m_Modules.GetStartPosition();
    if (!pos)
    {
      return E_FAIL;
    }
    module = m_Modules.GetValueAt(pos);
  }
  else if (!m_Modules.Lookup(lpszModuleID, module))
  {
    // requested module not found
    return E_FAIL;
  }
  pRet = module.Detach();
  return S_OK;
}

//----------------------------------------------------------------------------
//  LoadModule
HRESULT CMagpieApplication::LoadModule(
  CMagpieModule           *   pSrcModule,
  LPCOLESTR                   lpszModuleID,
  LPCOLESTR                   lpszModuleSource,
  CMagpieModuleComObject  *&  pRet)
{
  CString sModuleID;
  IF_FAILED_RET(ResolveModuleID(
    pSrcModule, lpszModuleID, sModuleID));

  CComPtr<IMagpieModule> module;

  // is the module already loaded?
  HRESULT hr = E_FAIL;
  if (SUCCEEDED(GetModule(sModuleID, module.p)))
  {
    // yes, return module
    pRet = static_cast<CMagpieModuleComObject*>(module.Detach());
    return S_FALSE;  // means: already loaded
  }

  CComBSTR bsScriptSource;
  if (lpszModuleSource)
  {
    // the script source came in lpszModuleSource
    bsScriptSource = lpszModuleSource;
    hr = S_OK;
  }
  else
  {
    // ask script loaders for the module's source
    size_t loaderCount = m_ScriptLoaders.GetCount();
    for(size_t n = 0; n < loaderCount; n++)
    {
      hr = m_ScriptLoaders[n]->GetModuleScript(lpszModuleID, &bsScriptSource);
      if (SUCCEEDED(hr))
      {
        break;
      }
    }
  }
  if (SUCCEEDED(hr))
  {
    // WATCH IT! The created object takes ownership of bsScriptSource!
    CMagpieModuleComObject * pModuleObject;
    IF_FAILED_RET(CMagpieModule::CreateObject(
      *this, sModuleID, bsScriptSource, pModuleObject));

    m_Modules[sModuleID] = pModuleObject;
    pRet = pModuleObject;
    return S_OK;
  }
  // no script found for this ID
  return E_FAIL;
}

//----------------------------------------------------------------------------
//  RunModule
HRESULT CMagpieApplication::RunModule(
  CMagpieModule           *   pSrcModule,
  LPCOLESTR                   lpszModuleID,
  CMagpieModuleComObject  **  ppRet)
{
  // load the module
  CComPtr<CMagpieModuleComObject> module;
  IF_FAILED_RET(LoadModule(pSrcModule, lpszModuleID, NULL, module.p));

  if (ppRet)
  {
    module.CopyTo(ppRet);
  }

  // and run
  return module->Run();
}

//----------------------------------------------------------------------------
//  RunScriptAsModule
HRESULT CMagpieApplication::RunScriptAsModule(
  CMagpieModule           *   pSrcModule,
  LPCOLESTR                   lpszModuleID,
  LPCOLESTR                   lpszModuleSource,
  CMagpieModuleComObject  **  ppRet)
{
  // load the module
  CComPtr<CMagpieModuleComObject> module;
  IF_FAILED_RET(LoadModule(pSrcModule, lpszModuleID, lpszModuleSource, module.p));

  if (ppRet)
  {
    module.CopyTo(ppRet);
  }

  // and run
  return module->Run();
}

//----------------------------------------------------------------------------
//  EnterModule
void CMagpieApplication::EnterModule(
  LPCOLESTR lpszModuleID)
{
  m_Console.EnterModule(lpszModuleID);
}

//----------------------------------------------------------------------------
//  ExitModule
void CMagpieApplication::ExitModule()
{
  m_Console.ExitModule();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieApplication implementation

//----------------------------------------------------------------------------
//  Init
STDMETHODIMP CMagpieApplication::Init()
{
  // shutdown if running
  Shutdown();

  // init script engine
  // TODO: generate an appID somehow
  HRESULT hr = m_ScriptEngine.Init(_T("Magpie"));
  IF_FAILED_RET(hr);

  // prepare CommonJS objects

  // console
  IF_FAILED_RET(m_Console.Init());
  IF_FAILED_RET(m_ScriptEngine.AddNamedItem(
    L"console", &m_Console, SCRIPTITEM_ISPERSISTENT|SCRIPTITEM_ISVISIBLE));

  return S_OK;
}

//----------------------------------------------------------------------------
//  AddScriptLoader
STDMETHODIMP CMagpieApplication::AddScriptLoader(
  IMagpieScriptLoader * pLoader)
{
  if (!pLoader)
  {
    return E_INVALIDARG;
  }
  m_ScriptLoaders.Add(pLoader);
  return S_OK;
}

//----------------------------------------------------------------------------
//  AddFilesystemScriptLoader
STDMETHODIMP CMagpieApplication::AddFilesystemScriptLoader(
  const OLECHAR* lpszRootPath)
{
  CComPtr<CMagpieFilesystemScriptLoaderComObject> loader;
  IF_FAILED_RET(CMagpieFilesystemScriptLoader::CreateObject(
    lpszRootPath, loader.p));
  m_ScriptLoaders.Add(loader.p);
  return S_OK;
}

//----------------------------------------------------------------------------
//  AddResourceScriptLoader
STDMETHODIMP CMagpieApplication::AddResourceScriptLoader(
  HANDLE_PTR hModule)
{
  CComPtr<CMagpieResourceScriptLoaderComObject> loader;
  IF_FAILED_RET(CMagpieResourceScriptLoader::CreateObject(
    (HMODULE)hModule, loader.p));
  m_ScriptLoaders.Add(loader.p);
  return S_OK;
}

//----------------------------------------------------------------------------
//  Run
STDMETHODIMP CMagpieApplication::Run(
  const OLECHAR* lpszModuleID)
{
  // add and run the main module
  return RunModule(NULL, lpszModuleID);
}

//----------------------------------------------------------------------------
//  RunScript
STDMETHODIMP CMagpieApplication::RunScript(
  const OLECHAR* lpszModuleID, const OLECHAR* lpszScript)
{
  return RunScriptAsModule(NULL, lpszModuleID, lpszScript);
}

//----------------------------------------------------------------------------
//  AddExtension
STDMETHODIMP CMagpieApplication::AddExtension(
  const OLECHAR* lpszExtensionName, IDispatch* pDispExtension)
{
  if (!lpszExtensionName || !pDispExtension || !wcslen(lpszExtensionName))
  {
    return E_INVALIDARG;
  }
  IF_FAILED_RET(m_ScriptEngine.AddNamedItem(
    lpszExtensionName, pDispExtension, SCRIPTITEM_ISPERSISTENT|SCRIPTITEM_ISVISIBLE));
  return S_OK;
}

//----------------------------------------------------------------------------
//  GetModuleObject
STDMETHODIMP CMagpieApplication::GetModuleObject(const OLECHAR* lpszModuleID, IMagpieModuleRestricted ** ppRet)
{
  ENSURE_RETVAL(ppRet);
  CComPtr<IMagpieModule> module;
  if (!m_Modules.Lookup(lpszModuleID, module))
  {
    return E_FAIL;
  }
  return module.QueryInterface(ppRet);
}

//----------------------------------------------------------------------------
//  Shutdown
STDMETHODIMP CMagpieApplication::Shutdown()
{
  // script engine first...
  m_ScriptEngine.Shutdown();
  // ...then modules...
  m_Modules.RemoveAll();
  // ...the console...
  m_Console.Shutdown();
  // ...then remove all script loaders...
  m_ScriptLoaders.RemoveAll();
  // ...the DISPID manager
  CDispExIdManager::Get().Clear();
  return S_OK;
}

