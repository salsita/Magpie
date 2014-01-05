/****************************************************************************
 * MagpieRequire.cpp : Implementation of CMagpieRequire
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "stdafx.h"
#include "MagpieApplication.h"
#include "MagpieModule.h"

/*============================================================================
 * class CMagpieRequire
 */

//----------------------------------------------------------------------------
//  static CreateObject
HRESULT CMagpieRequire::CreateObject(
  CMagpieModule           &   module,
  CMagpieRequireComObject *&  pRet)
{
  CMagpieRequireComObject *newObject = pRet = NULL;
  IF_FAILED_RET(CMagpieRequireComObject::CreateInstance(&newObject));
  newObject->AddRef();

  HRESULT hr = newObject->Init(module);
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
CMagpieRequire::CMagpieRequire() :
  m_pModule(NULL)
{
}

//----------------------------------------------------------------------------
//  Init
HRESULT CMagpieRequire::Init(
  CMagpieModule & module)
{
  m_pModule = &module;
  return S_OK;
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieRequire::FinalConstruct()
{
	return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieRequire::FinalRelease()
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieRequire implementation

//----------------------------------------------------------------------------
//  require
STDMETHODIMP CMagpieRequire::require(
  BSTR      moduleID,
  VARIANT * pvtExports)
{
  ATLASSERT(m_pModule);
  ENSURE_RETVAL(pvtExports);

  CComPtr<CMagpieModuleComObject> module;
  IF_FAILED_RET(m_pModule->RunModule(moduleID, &module));

  CComPtr<IDispatchEx> pExports;
  IF_FAILED_RET(module->GetExports(&pExports));
  if (!pExports)
  {
    return E_UNEXPECTED;
  }
  IF_FAILED_RET(pExports->QueryInterface(IID_IDispatch, (void**)&pvtExports->pdispVal));
  pvtExports->vt = VT_DISPATCH;
  return S_OK;
}

//----------------------------------------------------------------------------
//  get_main
STDMETHODIMP CMagpieRequire::get_main(
  VARIANT * pVal)
{
  ENSURE_RETVAL(pVal);
  ATLASSERT(m_pModule);

  CComPtr<IMagpieModule> mainModule;
  IF_FAILED_RET(m_pModule->GetApplication()->GetModule(NULL, mainModule.p));

  IF_FAILED_RET(mainModule->QueryInterface(&pVal->pdispVal));
  pVal->vt = VT_DISPATCH;
  return S_OK;
}
