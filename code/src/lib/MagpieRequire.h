/****************************************************************************
 * MagpieRequire.h : Declaration of the CMagpieRequire
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once
#include "Magpie_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// forwards
class CMagpieModule;
class CMagpieRequire;
typedef CComObject<CMagpieRequire>  CMagpieRequireComObject;

/*============================================================================
 * class CMagpieRequire
 * Implements the require object for a module.
 */
class ATL_NO_VTABLE CMagpieRequire :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IMagpieRequire, &IID_IMagpieRequire, &LIBID_MagpieLib,
                      0xffff, 0xffff>
{
public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
  DECLARE_NOT_AGGREGATABLE(CMagpieRequire)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieRequire)
	  COM_INTERFACE_ENTRY(IMagpieRequire)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // static creator function
  static HRESULT CreateObject(CMagpieModule           &   module,
                              CMagpieRequireComObject *&  pRet);

public:
  // -------------------------------------------------------------------------
  // COM standard methods
  HRESULT FinalConstruct();
	void FinalRelease();

public:
  // -------------------------------------------------------------------------
  // IMagpieRequire methods
  STDMETHOD(require)(BSTR       moduleID,
                     VARIANT  * pvtExports);

  STDMETHOD(get_main)(VARIANT * pVal);

protected:
  // -------------------------------------------------------------------------
  // ctor
	CMagpieRequire();

private:
  // -------------------------------------------------------------------------
  // Private methods.
  HRESULT Init(CMagpieModule  & module);

private:
  // -------------------------------------------------------------------------
  // Private members.

  // a pointer to the module we belong to
  CMagpieModule * m_pModule;
};
