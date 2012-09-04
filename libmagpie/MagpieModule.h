/****************************************************************************
 * MagpieModule.h : Declaration of the CMagpieModule
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once
#include "Magpie_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include "MagpieRequire.h"
#include "MagpieModuleExports.h"

// forwards
class CMagpieActiveScript;
class CMagpieApplication;

class CMagpieModule;
typedef CComObject<CMagpieModule> CMagpieModuleComObject;

/*============================================================================
 * class CMagpieModule
 * Implements a module.
 */
class ATL_NO_VTABLE CMagpieModule :
	public CComObjectRootEx<CComSingleThreadModel>,
  public IMagpieModuleRestricted,
	public IDispatchImpl<IMagpieModule, &IID_IMagpieModule, &LIBID_MagpieLib,
                      0xffff, 0xffff>
{
public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
  DECLARE_NOT_AGGREGATABLE(CMagpieModule)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieModule)
	  COM_INTERFACE_ENTRY(IMagpieModuleRestricted)
	  COM_INTERFACE_ENTRY(IMagpieModule)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // static creator function
  // WATCH IT! On success the created object takes ownership of bsScriptSource
  // to prevent unnecessary copies!
  static HRESULT CreateObject(CMagpieApplication  &       application,
                              LPCOLESTR                   lpsModuleID,
                              CComBSTR                 &  bsScriptSource,
                              CMagpieModuleComObject  *&  pRet);

public:
  // -------------------------------------------------------------------------
  // COM standard methods
  HRESULT FinalConstruct();
	void FinalRelease();

public:
  // -------------------------------------------------------------------------
  // public members

  // The Run method runs this module and takes care that the module is ran
  //  only once.
  HRESULT Run();

  // This method is called by the corresponding require object to run a
  //  foreign module from within this module.
  //  See CMagpieApplication::RunModule()
  HRESULT RunModule(LPCOLESTR                   lpszModuleID,
                    CMagpieModuleComObject  **  ppModule = NULL);

public:
  // -------------------------------------------------------------------------
  // Getters.
  IDispatch *GetRequire()
                  {return m_Require;}

  HRESULT GetExports(IDispatchEx ** ppDispEx);

  CMagpieApplication *GetApplication()
                  {return m_pApplication;}

  void GetID(CString & sModuleID)
                  {sModuleID = m_sID;}

  LPCOLESTR GetScriptSource()
                  {ATLASSERT((LPCOLESTR)m_bsScriptSource);
                    return m_bsScriptSource;}

public:
  // -------------------------------------------------------------------------
  // IMagpieModule methods. See .idl for description.
  STDMETHOD(get_id)(VARIANT* pVal);
  STDMETHOD(get_exports)(VARIANT* pVal);
  STDMETHOD(put_exports)(LPDISPATCH pVal);

  // -------------------------------------------------------------------------
  // IMagpieModuleRestricted methods. See .idl for description.
  STDMETHOD(GetExportsObject)(LPDISPATCH * ppRet);

protected:
  // -------------------------------------------------------------------------
  // ctor
	CMagpieModule();

private:
  // -------------------------------------------------------------------------
  // Private methods.

  // Init: WATCH IT! Takes ownership of bsScriptSource to prevent unnecessary
  // copies!
	HRESULT Init(CMagpieApplication & application,
                LPCOLESTR           lpsModuleID,
                CComBSTR          & bsScriptSource);

private:
  // -------------------------------------------------------------------------
  // Private members.

  // Pointer to the application.
  CMagpieApplication
          * m_pApplication;

  // Flag indicating that the module ran already.
  //  This flag is used to prevent circular inclusions. Immediately before 
  //  this module runs the flag is set (inside Run() method), so calling
  //  Run() again does nothing.
  BOOL
            m_bDidRun;

  // the module's absolute ID
  CString
            m_sID;

  // script source
  CComBSTR
            m_bsScriptSource;

  // the module's require object
  CComPtr<CMagpieRequireComObject>
            m_Require;

  // the module's exported API
#ifdef USE_MODULES_EXPORT_OBJECT
  CComPtr<CMagpieModuleExportsComObject>
            m_Exports;
#else
  CComQIPtr<IDispatchEx>
            m_Exports;
#endif // def USE_MODULES_EXPORT_OBJECT
};

