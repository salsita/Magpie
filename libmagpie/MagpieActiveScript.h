/****************************************************************************
 * MagpieActiveScript.h : Declaration of the CMagpieActiveScript
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

#include "Magpie_i.h"

#include "ActiveScriptT.h"
#include "ActiveScriptDebugT.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// forwards
class CMagpieApplication;
class CMagpieModule;

/*============================================================================
 * class CMagpieActiveScript
 * The magpie implementation for IActiveScriptSite based on CActiveScriptT.
 */
class ATL_NO_VTABLE CMagpieActiveScript :
	public CComObjectRootEx<CComSingleThreadModel>,
  public IMagpieObjectCreator,
  public CActiveScriptT<CMagpieActiveScript>
#ifdef SCRIPTDEBUG_
  ,public CActiveScriptDebugT<CMagpieActiveScript>
#endif
{
public:
  // -------------------------------------------------------------------------
  // ctor
  CMagpieActiveScript(CMagpieApplication & application);

public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
  DECLARE_NOT_AGGREGATABLE(CMagpieActiveScript)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieActiveScript)
	  COM_INTERFACE_ENTRY(IActiveScriptSite)
    COM_INTERFACE_ENTRY(IMagpieObjectCreator)
#ifdef SCRIPTDEBUG_
	  COM_INTERFACE_ENTRY(IActiveScriptSiteDebug)
#endif
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // COM standard methods
  HRESULT FinalConstruct();
	void FinalRelease();

public:
  // -------------------------------------------------------------------------
  // public methods

  // IMagpieObjectCreator methods
  STDMETHOD(createObject)(BSTR aObjectName, LPDISPATCH * aRetObject);

  // Init the script engine
  HRESULT Init(LPCOLESTR appId, const CLSID & aClsidScriptEngine);

  // Shutdown script engine and cleanup
  HRESULT Shutdown();

  // Run a module. Runs the module unconditionally, so this method should be
  //  used only by the module itself after checking if the module ran
  //  already.
  HRESULT RunModule(CMagpieModule* pModule);

  // Runs a script source in the context of a certain module
  HRESULT ExecuteScriptForModule(const OLECHAR* lpszScript, CMagpieModule* pModule);

  // Runs a script source in the global context
  HRESULT ExecuteScriptGlobal(const OLECHAR* lpszScript);

  // Runs a module in the global context
  HRESULT ExecuteGlobal(CMagpieModule* pModule);

  // Simply adds a named item to the script engine. Used to inject objects
  //  into the global namespace of the script engine.
  HRESULT AddNamedItem(LPCOLESTR    pstrName,
                       IDispatch  * pDisp,
                       DWORD        dwFlags);

  // forwards to m_ScriptEngine
  HRESULT GetScriptDispatch(LPCOLESTR pstrItemName, IDispatch **ppdisp);

public:
  // -------------------------------------------------------------------------
  // IActiveScriptSite methods overriding CActiveScriptT.
  STDMETHOD(GetItemInfo)(LPCOLESTR      pstrName,
                         DWORD          dwReturnMask,
                         IUnknown   **  ppiunkItem,
                         ITypeInfo  **  ppti);

public:
  // -------------------------------------------------------------------------
  // public members
  int mJscriptVersion;

private:
  // -------------------------------------------------------------------------
  // Private members.

  // Reference to the application.
  CMagpieApplication  & m_Application;

  // Map of named items. Used in GetItemInfo.
  CAtlMap<CString, CComPtr<IDispatch> >
                        m_NamedItems;
};
