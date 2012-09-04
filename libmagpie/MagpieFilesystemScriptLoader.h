/****************************************************************************
 * MagpieFilesystemScriptLoader.h : Declaration of
 * CMagpieFilesystemScriptLoader
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

#include "Magpie_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class CMagpieFilesystemScriptLoader;
typedef CComObject<CMagpieFilesystemScriptLoader>
                   CMagpieFilesystemScriptLoaderComObject;

class ATL_NO_VTABLE CMagpieFilesystemScriptLoader :
	public CComObjectRootEx<CComSingleThreadModel>,
  public IMagpieScriptLoader
{
public:
  // -------------------------------------------------------------------------
  // ctor
  CMagpieFilesystemScriptLoader(void);

public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieFilesystemScriptLoader)
	  COM_INTERFACE_ENTRY(IMagpieScriptLoader)
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // static creator function
  static HRESULT CreateObject(LPCOLESTR       lpsRootPath,
                              CMagpieFilesystemScriptLoaderComObject
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
  STDMETHOD(GetModuleScript)(const OLECHAR* lpszModuleID, BSTR * pbsScript);

private:
  // -------------------------------------------------------------------------
  // Private methods.

	HRESULT Init(LPCOLESTR lpsRootPath);

private:
  // -------------------------------------------------------------------------
  // Private members.

  CString m_sRootPath;
};
