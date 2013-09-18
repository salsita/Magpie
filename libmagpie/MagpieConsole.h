/****************************************************************************
 * MagpieConsole.h : Declaration of the CMagpieConsole
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

#include "Magpie_i.h"
#include "_IMagpieLoggerEvents_CP.H"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// forwards
class CMagpieApplication;

/*============================================================================
 * class CMagpieConsole
 * Console injected into the global script namespace.
 */
class ATL_NO_VTABLE CMagpieConsole :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IMagpieConsole, &IID_IMagpieConsole, &LIBID_MagpieLib,
                       0xffff, 0xffff>,
  public IConnectionPointContainerImpl<CMagpieConsole>,
  public CProxy_IMagpieLoggerEvents<CMagpieConsole>
{
public:
  // -------------------------------------------------------------------------
  // ctor
	CMagpieConsole(CMagpieApplication & application);

public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_ONLY_AGGREGATABLE(CMagpieConsole)

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieConsole)
	  COM_INTERFACE_ENTRY(IMagpieConsole)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // COM connection point map
  BEGIN_CONNECTION_POINT_MAP(CMagpieConsole)
    CONNECTION_POINT_ENTRY(__uuidof(_IMagpieLoggerEvents))
  END_CONNECTION_POINT_MAP()

public:
  // -------------------------------------------------------------------------
  // COM standard methods
  HRESULT FinalConstruct();
	void FinalRelease();

public:
  // -------------------------------------------------------------------------
  // public methods
  HRESULT Init();
  HRESULT Shutdown();

  // see MagpieApplication.h
  void EnterModule(LPCOLESTR lpszModuleID);
  void ExitModule();


public:
  // -------------------------------------------------------------------------
  // IMagpieConsole methods. See .idl for description.
  STDMETHOD(log)(SAFEARRAY* pVals);
  STDMETHOD(debug)(SAFEARRAY* pVals);
  STDMETHOD(info)(SAFEARRAY* pVals);
  STDMETHOD(warn)(SAFEARRAY* pVals);
  STDMETHOD(error)(SAFEARRAY* pVals);

private:
  // -------------------------------------------------------------------------
  // Private members.

  // sent to the attached loggers in case no module is selected
  static LPCOLESTR EMPTY_MODULE_ID;

  // Reference to the application.
  CMagpieApplication &  m_Application;

  // current module id sent to loggers
  CComBSTR              m_sCurrentModuleID;

  // stack used by EnterModule() and ExitModule to keep trace of the modules
  // running
  CAtlList<CComBSTR>    m_ModuleStack;
};

