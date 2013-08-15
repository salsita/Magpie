/****************************************************************************
 * MagpieModule.cpp : Implementation of CMagpieModule
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "stdafx.h"
#include "MagpieModule.h"
#include "MagpieActiveScript.h"
#include "MagpieApplication.h"

/*============================================================================
 * class CMagpieModule
 */

//----------------------------------------------------------------------------
//  static CreateObject
HRESULT CMagpieModule::CreateObject(
  CMagpieApplication       &  application,
  LPCOLESTR                   lpsModuleID,
  IMagpieScriptLoader      *  pScriptLoader,
  LPCOLESTR                   lpszScriptSource,
  BOOL                        aDecorateScript,
  CMagpieModuleComObject  *&  pRet)
{
  CMagpieModuleComObject *newObject = pRet = NULL;
  IF_FAILED_RET(CMagpieModuleComObject::CreateInstance(&newObject));
  newObject->AddRef();

  HRESULT hr = E_FAIL;
  hr = newObject->Init(application, lpsModuleID, pScriptLoader, lpszScriptSource,
      aDecorateScript);
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
CMagpieModule::CMagpieModule() :
  m_pApplication(NULL),
  m_bDidRun(FALSE),
  m_dwScriptContext(0)
{
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieModule::FinalConstruct()
{
	return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieModule::FinalRelease()
{
  m_Exports.Release();
  m_Require.Release();
}

//----------------------------------------------------------------------------
//  Init
HRESULT CMagpieModule::Init(
  CMagpieApplication  & application,
  LPCOLESTR             lpsModuleID,
  IMagpieScriptLoader * pScriptLoader,
  LPCOLESTR             lpszScriptSource,
  BOOL                  aDecorateScript)
{
  ATLASSERT(pScriptLoader || lpszScriptSource);
  m_pApplication = &application;
  m_sID = lpsModuleID;
  if (lpszScriptSource)
  {
    m_bsScriptSource = lpszScriptSource;
  }
  else if(pScriptLoader)
  {
    // Decorating scripts: Due to the bug in jscript9 (see stdafx.h) there is
    // no module functionality. To emultate this we wrap certain scripts in
    // a closure. This regards currently all scripts, except the ones run from
    // CMagpieApplication::ExecuteGlobal(). These scripts are content scripts
    // and run in the website, they are ONLY TECHNICALLY MODULES, but are in
    // fact executed in a global context.
    if (aDecorateScript) {
      IF_FAILED_RET(pScriptLoader->GetModuleScriptDecorated(lpsModuleID,
          gJscript9ModuleWrapperIntro,
          gJscript9ModuleWrapperExtro,
          &m_bsScriptSource));
    }
    else {
      IF_FAILED_RET(pScriptLoader->GetModuleScript(lpsModuleID, &m_bsScriptSource));
    }
    // get additional data from script loader
    CComQIPtr<IMagpieScriptLoader2> loader2(pScriptLoader);
    if (loader2)
    {
      CComBSTR bsFileName;
      // We use a relative name here as a long name for the debugger. If we would
      // use the real filename JIT would understand this and open the actual file.
      // But in this case we would not be able to add scripts freely executed in
      // this module to appear in the debugger as part of the module script.
      // Sometime in the future someone might find a solution for this, then we should
      // use the real filepath.
      if (SUCCEEDED(loader2->GetProperty(lpsModuleID, L"relname", &bsFileName)))
      {
        m_sFilename = bsFileName;
      }
    }
  }

  // create "require" object
  IF_FAILED_RET(CMagpieRequire::CreateObject(*this, m_Require.p));

  // create "exports" object
#ifdef USE_MODULES_EXPORT_OBJECT
  IF_FAILED_RET(CMagpieModuleExports::CreateObject(m_Exports.p));
#else
  CComPtr<IDispatch> disp;
  IF_FAILED_RET(application.GetScriptEngine().GetScriptDispatch(NULL, &disp));
  CIDispatchHelper script = disp;
  CComPtr<IDispatch> exportsDisp;
  IF_FAILED_RET(script.CreateObject(L"Object", &exportsDisp));
  m_Exports = exportsDisp;
  return (m_Exports)
    ? S_OK
    : E_NOINTERFACE;
#endif // def USE_MODULES_EXPORT_OBJECT

  return S_OK;
}

//----------------------------------------------------------------------------
//  RunModule
HRESULT CMagpieModule::RunModule(
  LPCOLESTR                   lpszModuleID,
  CMagpieModuleComObject  **  ppModule)
{
  ATLASSERT(m_pApplication);
  return m_pApplication->RunModule(this, lpszModuleID, ppModule);
}

//----------------------------------------------------------------------------
//  GetExports
HRESULT CMagpieModule::GetExports(IDispatchEx ** ppDispEx)
{
#ifdef USE_MODULES_EXPORT_OBJECT
  return m_Exports.QueryInterface(ppDispEx);
#else
  if (!m_pApplication)
  {
    return E_UNEXPECTED;
  }
  CComPtr<IDispatch> disp;
  // this might fail - exports might not be set yet
  HRESULT hr = m_pApplication->GetScriptEngine().GetScriptDispatch(m_sID, &disp);
  if (FAILED(hr))
  {
    // so return the exports object we have
    return m_Exports.QueryInterface(ppDispEx);
  }

  CComVariant vt;
  IF_FAILED_RET(disp.GetPropertyByName(L"exports", &vt));
  if (vt.vt != VT_DISPATCH)
  {
    return E_NOINTERFACE;
  }
  CIDispatchHelper exp(vt.pdispVal);
  return vt.pdispVal->QueryInterface(IID_IDispatchEx, (void**)ppDispEx);
#endif // def USE_MODULES_EXPORT_OBJECT
}

//----------------------------------------------------------------------------
//  Run
HRESULT CMagpieModule::Run()
{
  if (m_bDidRun)
  {
    // we ran already.
    return S_FALSE;
  }
  ATLASSERT(m_pApplication);

  m_bDidRun = TRUE;
  IF_FAILED_RET(m_pApplication->GetScriptEngine().RunModule(this));

  // prevent changes to exports
#ifdef USE_MODULES_EXPORT_OBJECT
  m_Exports->SetReadonly(TRUE);
#endif // def USE_MODULES_EXPORT_OBJECT
  
  return S_OK;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieModule implementation

//----------------------------------------------------------------------------
//  get_id
STDMETHODIMP CMagpieModule::get_id(
  VARIANT* pVal)
{
  ENSURE_RETVAL(pVal);
  pVal->vt = VT_BSTR;
  pVal->bstrVal = m_sID.AllocSysString();
  return S_OK;
}

//----------------------------------------------------------------------------
//  get_exports
STDMETHODIMP CMagpieModule::get_exports(
  VARIANT* pVal)
{
  ENSURE_RETVAL(pVal);
#ifdef USE_MODULES_EXPORT_OBJECT
  IF_FAILED_RET(
    m_Exports->QueryInterface(IID_IDispatch, (void**)&pVal->pdispVal));
#else
  CComPtr<IDispatchEx> exports;
  IF_FAILED_RET(GetExports(&exports));
  IF_FAILED_RET(
    exports->QueryInterface(&pVal->pdispVal));
#endif // def USE_MODULES_EXPORT_OBJECT
  pVal->vt = VT_DISPATCH;

  return S_OK;
}

//----------------------------------------------------------------------------
//  put_exports
STDMETHODIMP CMagpieModule::put_exports(
  LPDISPATCH pVal)
{
#ifdef USE_MODULES_EXPORT_OBJECT
  return S_OK;
#else
  CComQIPtr<IDispatchEx> exports(pVal);
  if (exports)
  {
    m_Exports = exports;
    return S_OK;
  }
  return E_INVALIDARG;
#endif // def USE_MODULES_EXPORT_OBJECT
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieModuleRestricted implementation

//----------------------------------------------------------------------------
//  GetExports
STDMETHODIMP CMagpieModule::GetExportsObject(
  LPDISPATCH * ppRet)
{
  ENSURE_RETVAL(ppRet);
#ifdef USE_MODULES_EXPORT_OBJECT
  return m_Exports.QueryInterface(ppRet);
#else
  CComPtr<IDispatchEx> exports;
  IF_FAILED_RET(GetExports(&exports));
  return exports.QueryInterface(ppRet);
#endif // def USE_MODULES_EXPORT_OBJECT
}
