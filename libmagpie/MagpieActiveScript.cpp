/****************************************************************************
 * MagpieActiveScript.cpp : Implementation of CMagpieActiveScript
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "stdafx.h"
#include "MagpieApplication.h"
#include "MagpieModule.h"


/*============================================================================
 * class CMagpieApplication
 */

//----------------------------------------------------------------------------
//  CTOR
CMagpieActiveScript::CMagpieActiveScript(CMagpieApplication & application) :
  mJscriptVersion(5), m_Application(application)
{
}

STDMETHODIMP CMagpieActiveScript::createObject(BSTR aObjectName, LPDISPATCH * aRetObject)
{
  ATLASSERT(m_ScriptEngine);
  if (!m_ScriptEngine) {
    return E_UNEXPECTED;
  }
  ENSURE_RETVAL(aRetObject)
  CIDispatchHelper script;
  IF_FAILED_RET(m_ScriptEngine->GetScriptDispatch(NULL, &script));
  return script.CreateObject(aObjectName, aRetObject);
}

//----------------------------------------------------------------------------
//  Init
HRESULT CMagpieActiveScript::Init(LPCOLESTR appId, const CLSID & aClsidScriptEngine)
{
  mJscriptVersion = 5;
  if (CLSID_JScript9 == aClsidScriptEngine) {
    mJscriptVersion = 9;
  }
#ifdef SCRIPTDEBUG_
  IF_FAILED_RET(InitializeDebugInterface(appId));
#endif
  IF_FAILED_RET(LoadScriptEngine(aClsidScriptEngine));
  IF_FAILED_RET(m_ScriptEngine->SetScriptState(SCRIPTSTATE_INITIALIZED));
  return S_OK;
}

//----------------------------------------------------------------------------
//  Shutdown
HRESULT CMagpieActiveScript::Shutdown()
{
  UnloadScriptEngine();
#ifdef SCRIPTDEBUG_
  UninitializeDebugInterface();
#endif
  m_NamedItems.RemoveAll();
  return S_OK;
}

//----------------------------------------------------------------------------
//  RunModule
HRESULT CMagpieActiveScript::RunModule(
  CMagpieModule* pModule)
{
  ATLASSERT(m_ScriptEngine);
  ATLASSERT(pModule);
  if (!m_ScriptEngine) {
    return E_UNEXPECTED;
  }
  if (!pModule) {
    return E_INVALIDARG;
  }
  CComPtr<IDispatch> pModuleRequireObject(pModule->GetRequire());
  CComPtr<IDispatchEx> pModuleExportsObject;
  IF_FAILED_RET(pModule->GetExports(&pModuleExportsObject));
  if (!pModuleRequireObject || !pModuleExportsObject) {
    return E_UNEXPECTED;
  }

  CString sFilename, sModuleID;
  pModule->GetID(sModuleID);
  pModule->GetFilename(sFilename);

  m_ScriptEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);

  // add namespace for module
  IF_FAILED_RET(m_ScriptEngine->AddNamedItem(sModuleID, SCRIPTITEM_CODEONLY));

  HRESULT hr = E_FAIL;
  if (9 == mJscriptVersion) {
    // jscript9 lacks module support. We have to handle this differently.
    // This includes wrapping the module in a closure and loading it as an expression.
    // The expression will "return" the closure which we will then execute with
    // "require, module, exports" as arguments.
    // load the module as an expression
    m_Application.EnterModule(sModuleID);

    // Note that the parent debug context is 0. Modules are in the debugger shown
    // at top level.
    DWORD_PTR dwSourceContext = 0;

    CComVariant scriptResult;
    hr = CActiveScriptT::AddScript(pModule->GetScriptSource(), NULL, sFilename, &dwSourceContext, &scriptResult);
    if (SUCCEEDED(hr)) {
      // set module's source context
      pModule->SetSourceContext(dwSourceContext);
      m_ScriptEngine->SetScriptState(SCRIPTSTATE_CONNECTED);

      // now get the closure of that module
      hr = TYPE_E_TYPEMISMATCH;
      if (VT_DISPATCH == scriptResult.vt && scriptResult.pdispVal) {
        CIDispatchHelper moduleClosure(scriptResult.pdispVal);
        ATLASSERT(moduleClosure);

        // create a "this" context
        CIDispatchHelper globalscript(m_ScriptGlobal);
        CComPtr<IDispatch> contextObject;
        hr = globalscript.CreateObject(L"Object", &contextObject);
        if(SUCCEEDED(hr)) {
          // call closure with arguments "require, module, exports"
          // and created context
          CComVariant args[4] = {contextObject, pModuleExportsObject, pModule, pModuleRequireObject};
          DISPID didThis = DISPID_THIS;
          DISPPARAMS params = {args, &didThis, _countof(args), 1};
          CComVariant newExportsObject;
          hr = moduleClosure.Call(NULL, &params, &newExportsObject);
          // Put the new exports object to the module. This is required
          // because the module might replace the exports object:
          //   exports = {...};
          // Normally the module would return, when asked for "exports", what
          // the script dispatch (the named item) for this module will
          // return. Since we don't have a script dispatch the module will
          // return its own m_Exports. So set this here.
          if (SUCCEEDED(hr) && VT_DISPATCH == newExportsObject.vt) {
            pModule->put_exports(newExportsObject.pdispVal);
          }
        }
      }
    }
    m_Application.ExitModule();
    return hr;
  }
  else {
    // dispatch for module's namespace
    CIDispatchHelper script;
    IF_FAILED_RET(m_ScriptEngine->GetScriptDispatch(sModuleID, &script));
    // inject CommonJS objects
    script.SetPropertyByRef(L"require", CComVariant(pModuleRequireObject));
    script.SetPropertyByRef(L"exports", CComVariant(pModuleExportsObject));
    script.SetPropertyByRef(L"module", CComVariant(pModule));

    // run the module
    m_Application.EnterModule(sModuleID);

    // Note that the parent debug context is 0. Modules are in the debugger shown
    // at top level.
    DWORD_PTR dwSourceContext = 0;
    HRESULT hr = CActiveScriptT::AddScript(pModule->GetScriptSource(), sModuleID, sFilename, &dwSourceContext);
    if (SUCCEEDED(hr)) {
      // set module's source context
      pModule->SetSourceContext(dwSourceContext);
      m_ScriptEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
    }

    m_Application.ExitModule();
    return hr;
  }

  // any other error
  return hr;
}


//----------------------------------------------------------------------------
//  ExecuteScriptForModule
HRESULT CMagpieActiveScript::ExecuteScriptForModule(
  const OLECHAR* lpszScript,
  CMagpieModule* pModule,
  VARIANT* result)
{
  ATLASSERT(m_ScriptEngine);
  if (!m_ScriptEngine) {
    return E_UNEXPECTED;
  }
  m_ScriptEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
  CString sModuleID;
  LPCTSTR lpszModuleID = NULL;
  DWORD_PTR dwParentSourceContext = 0;
  if (pModule)
  {
    // get id
    pModule->GetID(sModuleID);
    lpszModuleID = sModuleID;
    // get parent context
    dwParentSourceContext = pModule->GetSourceContext();
  }
  // lpszModuleID can be NULL now.
  // set debug context to the module or to global if lpszModuleID is NULL.
  m_Application.EnterModule(lpszModuleID);
  // run script
  HRESULT hr = E_FAIL;
  hr = CActiveScriptT::AddScript(lpszScript, lpszModuleID, NULL, &dwParentSourceContext, result);
  if (SUCCEEDED(hr))
  {
    m_ScriptEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
  }
  m_Application.ExitModule();
  return S_OK;
}

//----------------------------------------------------------------------------
//  ExecuteScriptGlobal
HRESULT CMagpieActiveScript::ExecuteScriptGlobal(const OLECHAR* lpszScript, VARIANT* result)
{
  ATLASSERT(m_ScriptEngine);
  if (!m_ScriptEngine) {
    return E_UNEXPECTED;
  }
  m_ScriptEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
  DWORD_PTR dwSourceContext = 0;

  // enter global context
  m_Application.EnterModule(NULL);
  // run script
  HRESULT hr = E_FAIL;
  hr = CActiveScriptT::AddScript(lpszScript, NULL, NULL, &dwSourceContext, result);
  if (SUCCEEDED(hr))
  {
    m_ScriptEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
  }
  m_Application.ExitModule();
  return S_OK;
}

//----------------------------------------------------------------------------
//  ExecuteScriptForModule
HRESULT CMagpieActiveScript::ExecuteGlobal(CMagpieModule* pModule, VARIANT* result)
{
  ATLASSERT(m_ScriptEngine);
  if (!m_ScriptEngine) {
    return E_UNEXPECTED;
  }
  m_ScriptEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
  DWORD_PTR dwSourceContext = 0;

  // enter global context
  m_Application.EnterModule(NULL);
  // run script
  HRESULT hr = E_FAIL;
  hr = CActiveScriptT::AddScript(pModule->GetScriptSource(), NULL, NULL, &dwSourceContext, result);
  if (SUCCEEDED(hr))
  {
    m_ScriptEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
  }
  m_Application.ExitModule();
  return S_OK;
}

//----------------------------------------------------------------------------
//  AddNamedItem
HRESULT CMagpieActiveScript::AddNamedItem(
  LPCOLESTR   pstrName,
  IDispatch * pDisp,
  DWORD       dwFlags)
{
  ATLASSERT(m_ScriptEngine);
  if (!m_ScriptEngine) {
    return E_UNEXPECTED;
  }
  if (m_NamedItems.Lookup(pstrName))
  {
    return S_FALSE;
  }
  m_NamedItems[pstrName] = pDisp;
  IF_FAILED_RET(m_ScriptEngine->AddNamedItem(pstrName, dwFlags));
  return S_OK;
}

//----------------------------------------------------------------------------
//  GetScriptDispatch
HRESULT CMagpieActiveScript::GetScriptDispatch(
  LPCOLESTR       pstrItemName,
  IDispatch   **  ppdisp)
{
  return (m_ScriptEngine)
    ? m_ScriptEngine->GetScriptDispatch(pstrItemName, ppdisp)
    : E_UNEXPECTED;
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieActiveScript::FinalConstruct()
{
	return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieActiveScript::FinalRelease()
{
  Shutdown();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IActiveScriptSite implementation

//----------------------------------------------------------------------------
// IActiveScriptSite
STDMETHODIMP CMagpieActiveScript::GetItemInfo(
  LPCOLESTR     pstrName,
  DWORD         dwReturnMask,
  IUnknown  **  ppiunkItem,
  ITypeInfo **  ppti)
{
  ATLASSERT(!(dwReturnMask & SCRIPTINFO_ITYPEINFO));
  if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
  {
    CComPtr<IDispatch> disp;
    if (m_NamedItems.Lookup(pstrName, disp))
    {
      *ppiunkItem = disp.Detach();
      return S_OK;
    }
  }
  return TYPE_E_ELEMENTNOTFOUND;
}
