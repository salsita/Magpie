/****************************************************************************
 * MagpieResourceScriptLoader.h : Declaration of
 * CMagpieResourceScriptLoader
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

#include "Magpie_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class CMagpieResourceScriptLoader;
typedef CComObject<CMagpieResourceScriptLoader>
                   CMagpieResourceScriptLoaderComObject;

class ATL_NO_VTABLE CMagpieResourceScriptLoader :
	public CComObjectRootEx<CComSingleThreadModel>,
  public IMagpieScriptLoader
{
public:
  // -------------------------------------------------------------------------
  // ctor
  CMagpieResourceScriptLoader(void);

public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieResourceScriptLoader)
	  COM_INTERFACE_ENTRY(IMagpieScriptLoader)
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // static creator function
  static HRESULT CreateObject(HMODULE         hModule,
                              CMagpieResourceScriptLoaderComObject
                                          *&  pRet);

public:
  // -------------------------------------------------------------------------
  // COM standard methods
  HRESULT FinalConstruct();
	void FinalRelease();

public:
  // -------------------------------------------------------------------------
  // public methods

public:
  // -------------------------------------------------------------------------
  // IMagpieScriptLoader methods. See .idl for description.
  STDMETHOD(HasModuleScript)(const OLECHAR* lpszModuleID);
  STDMETHOD(GetModuleScript)(const OLECHAR* lpszModuleID, BSTR * pbsScript);
  STDMETHOD(GetModuleScriptDecorated)(
          const OLECHAR* lpszModuleID,
          const OLECHAR* lpszScriptIntro,
          const OLECHAR* lpszScriptExtro,
          BSTR * pbsScript);

private:
  // -------------------------------------------------------------------------
  // Private methods.

	HRESULT ResolveModuleID(LPCOLESTR lpszModuleID, CString * psRet = NULL);
	HRESULT Init(HMODULE hModule);

private:
  // -------------------------------------------------------------------------
  // Private members.

  HMODULE m_hModule;
};
