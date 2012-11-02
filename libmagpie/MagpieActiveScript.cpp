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
  m_Application(application)
{
}

//----------------------------------------------------------------------------
//  Init
HRESULT CMagpieActiveScript::Init(LPCOLESTR appId)
{
#ifdef SCRIPTDEBUG_
  IF_FAILED_RET(InitializeDebugInterface(appId));
#endif
  IF_FAILED_RET(LoadScriptEngine(CLSID_JScript));
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
  CComPtr<IDispatch> pModuleRequireOb(pModule->GetRequire());
  CComPtr<IDispatchEx> pModuleExportsOb;
  IF_FAILED_RET(pModule->GetExports(&pModuleExportsOb));
  if (!pModuleRequireOb || !pModuleExportsOb)
  {
    return E_UNEXPECTED;
  }

  CString sFilename, sModuleID;
  pModule->GetID(sModuleID);
  pModule->GetFilename(sFilename);

  m_ScriptEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);

  // add namespace for module
  IF_FAILED_RET(m_ScriptEngine->AddNamedItem(sModuleID, SCRIPTITEM_CODEONLY));

  // dispatch for module's namespace
  CIDispatchHelper script;
  IF_FAILED_RET(m_ScriptEngine->GetScriptDispatch(sModuleID, &script));

  // inject CommonJS objects
  script.SetPropertyByRef(L"require", CComVariant(pModuleRequireOb));
  script.SetPropertyByRef(L"exports", CComVariant(pModuleExportsOb));
  script.SetPropertyByRef(L"module", CComVariant(pModule));

  // now run the module
  m_Application.EnterModule(sModuleID);
  HRESULT hr = E_FAIL;
  DWORD_PTR dwSourceContext = 0;
  // Note that the parent debug context is 0. Modules are in the debugger shown
  // at top level.
  hr = CActiveScriptT::AddScript(pModule->GetScriptSource(), sModuleID, sFilename, &dwSourceContext);
  if (SUCCEEDED(hr))
  {
    // set module's source context
    pModule->SetSourceContext(dwSourceContext);
    m_ScriptEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
  }
  m_Application.ExitModule();

  return S_OK;
}

//----------------------------------------------------------------------------
//  ExecuteScriptForModule
HRESULT CMagpieActiveScript::ExecuteScriptForModule(
  const OLECHAR* lpszScript,
  CMagpieModule* pModule)
{
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
  hr = CActiveScriptT::AddScript(lpszScript, lpszModuleID, NULL, &dwParentSourceContext);
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
  if (!m_ScriptEngine)
  {
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
