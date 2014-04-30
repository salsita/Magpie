/****************************************************************************
 * MagpieModuleExports.h : Declaration of the CMagpieModuleExports
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once
#include "Magpie_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


/****************************************************************************
 * USE_MODULES_EXPORT_OBJECT and MODULES_HAVE_EXPORT_PROPERTY
 *
 * This a compatibility switch for underscore.js. Underscore and probably
 * other modules violate the CommonJS Modules 1.1 specification by
 * _replacing_ the exports object instead of _adding_ their API to it:
 *
 * "In a module, there is a free variable called 'exports', that is an object
 *  that the module may add its API to as it executes."
 *  (http://wiki.commonjs.org/wiki/Modules/1.1)
 *
 * underscore in turn does the following:
 *  exports = module.exports = _;
 *
 * To allow underscore to work properly without patching it the following
 * requirements have to be fulfilled:
 * 1) 'exports' must be replacable and
 *    'module' must have a writable property 'exports'
 * OR
 * 2) 'module' MUST NOT have a property 'exports' AT ALL.
 *
 * In 1) underscore REPLACES 'exports' and also adds its API to 'exports' as
 * '_'.
 * In 2) underscore ONLY adds its API to 'exports' as '_'.
 *
 * If the USE_MODULES_EXPORT_OBJECT switch is defined the module implementation
 * uses this class as 'exports'. This class can be locked so that modules
 * requiring a module can't modify the 'exports' object any more. Also
 * 'module' does not accept the 'exports' property to be replaced. This is
 * a cleaner and closer-to-the-specs-implementation, but will break
 * underscore and also some modules who want to do
 *   var _ = require("underscore.js");
 * instead of
 *   var _ = require("underscore.js")._;
 *
 * By default the switch is not defined. This means 'exports' is a normal
 * global JS object variable in the module. It can be replaced or even set to
 * null or undefined.
 *
 * So in the end you can use this switch together with
 * MODULES_HAVE_EXPORT_PROPERTY (see .idl file) to adjust the behaviour of
 * magpie:
 *
 *------------------------------------------------------------------------------*
 * ..EXPORT_OBJECT | ..EXPORT_PROPERTY | module.exports         | exports var   *
 *------------------------------------------------------------------------------*
 *         NO      |         NO        | not present            | replacable    *
 *------------------------------------------------------------------------------*
 *         NO      |         YES       | present, replacable    | replacable    * < DEFAULT
 *------------------------------------------------------------------------------*
 *         YES     |         NO        | not present            | nonreplacable *
 *------------------------------------------------------------------------------*
 *         YES     |         YES       | present, nonreplacable | nonreplacable *
 *------------------------------------------------------------------------------*
 *
 * If you want to change these settings do it in both projects libmagpie and
 * Magpie - both for C++ and MIDL.
 ****************************************************************************/
#ifdef USE_MODULES_EXPORT_OBJECT

// forwards
class CMagpieModuleExports;
typedef CComObject<CMagpieModuleExports>  CMagpieModuleExportsComObject;

/*============================================================================
 * class CMagpieModuleExports
 * Implements the export object for a module.
 */
class ATL_NO_VTABLE CMagpieModuleExports :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IDispatchEx, &IID_IDispatchEx, &LIBID_MagpieLib,
                      0xffff, 0xffff>
{
public:
  // -------------------------------------------------------------------------
  // COM standard stuff
  DECLARE_NO_REGISTRY();
  DECLARE_NOT_AGGREGATABLE(CMagpieModuleExports)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
  // -------------------------------------------------------------------------
  // COM interface map
  BEGIN_COM_MAP(CMagpieModuleExports)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(IDispatchEx)
  END_COM_MAP()

public:
  // -------------------------------------------------------------------------
  // static creator function
  static HRESULT CreateObject(CMagpieModuleExportsComObject *&pRet);

public:
  // -------------------------------------------------------------------------
  // COM standard methods
  HRESULT FinalConstruct();
	void FinalRelease();

public:
  // -------------------------------------------------------------------------
  // Public methods.

  // locks the object for writing. Used after the exporting module has run.
  void SetReadonly(BOOL bReadOnly = TRUE)
                  {m_bReadOnly = bReadOnly;}

public:
  // -------------------------------------------------------------------------
  // IDispatchEx methods
  STDMETHOD(GetDispID)(BSTR     bstrName,
                       DWORD    grfdex,
                       DISPID * pid);

  STDMETHOD(InvokeEx)(DISPID              id,
                      LCID                lcid,
                      WORD                wFlags,
                      DISPPARAMS        * pdp,
                      VARIANT           * pvarRes,
                      EXCEPINFO         * pei,
                      IServiceProvider  * pspCaller);

  STDMETHOD(DeleteMemberByName)(BSTR  bstrName,
                                DWORD grfdex);

  STDMETHOD(DeleteMemberByDispID)(DISPID id);

  STDMETHOD(GetMemberProperties)(DISPID   id,
                                 DWORD    grfdexFetch,
                                 DWORD  * pgrfdex);

  STDMETHOD(GetMemberName)(DISPID id,
                           BSTR * pbstrName);

  STDMETHOD(GetNextDispID)(DWORD    grfdex,
                           DISPID   id,
                           DISPID * pid);

  STDMETHOD(GetNameSpaceParent)(IUnknown ** ppunk);

protected:
  // -------------------------------------------------------------------------
  // ctor
  CMagpieModuleExports();

private:
  // -------------------------------------------------------------------------
  // Private methods.

  // Cleanup (remove all exported properties when shutting down)
  void Clear();

private:
  // -------------------------------------------------------------------------
  // Private defs.
  typedef CAtlMap<DISPID, CComVariant*>
                          CAtlDispIDMap;

private:
  // -------------------------------------------------------------------------
  // Private members.

  // can properties be added to this object?
  BOOL          m_bReadOnly;

  // our data (exported properties of the module, the module's API)
  CAtlDispIDMap m_Data;
};

#endif // def USE_MODULES_EXPORT_OBJECT
