/****************************************************************************
 * MagpieModuleExports.cpp : Implementation of CMagpieModuleExports
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "StdAfx.h"
#include "MagpieModuleExports.h"
#include "DispExIdManager.h"

#ifdef USE_MODULES_EXPORT_OBJECT

/*============================================================================
 * class CMagpieModuleExports
 */

//----------------------------------------------------------------------------
//  static CreateObject
HRESULT CMagpieModuleExports::CreateObject(
  CMagpieModuleExportsComObject *&  pRet)
{
  CMagpieModuleExportsComObject *newObject = pRet = NULL;
  IF_FAILED_RET(CMagpieModuleExportsComObject::CreateInstance(&newObject));
  newObject->AddRef();

  pRet = newObject;
  return S_OK;
}

//----------------------------------------------------------------------------
//  CTOR
CMagpieModuleExports::CMagpieModuleExports() :
  m_bReadOnly(FALSE)
{
}

//----------------------------------------------------------------------------
//  Clear
void CMagpieModuleExports::Clear()
{
  DISPID id;
  CComVariant* pvt;
  POSITION pos = m_Data.GetStartPosition();
  while(pos)
  {
    m_Data.GetNextAssoc(pos, id, pvt);
    if (pvt)
    {
      delete pvt;
    }
  }
  m_Data.RemoveAll();
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieModuleExports::FinalConstruct()
{
  return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieModuleExports::FinalRelease()
{
  Clear();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IDispatchEx implementation

//----------------------------------------------------------------------------
//  GetDispID
STDMETHODIMP CMagpieModuleExports::GetDispID(
  BSTR      bstrName,
  DWORD     grfdex,
  DISPID  * pid)
{
  ENSURE_RETVAL(pid);
	*pid = DISPID_UNKNOWN;
  DISPID dispid = CDispExIdManager::Get().GetID(
    bstrName, ((fdexNameEnsure & grfdex) && !m_bReadOnly));
  if (dispid)
  {
	  *pid = dispid;
    return S_OK;
  }
	return DISP_E_UNKNOWNNAME;
}

//----------------------------------------------------------------------------
//  InvokeEx
STDMETHODIMP CMagpieModuleExports::InvokeEx(
  DISPID              id,
  LCID                lcid,
  WORD                wFlags,
  DISPPARAMS        * pdp,
  VARIANT           * pvarRes,
  EXCEPINFO         * pei,
  IServiceProvider  * pspCaller)
{
  CComVariant* pvt = NULL;
  m_Data.Lookup(id, pvt);

  // put a property: this works always (except we are readonly) because
  // IDispatchEx can be extended dynamically:
  if (wFlags & DISPATCH_PROPERTYPUT)
	{
		if (m_bReadOnly)
    {
			return DISP_E_MEMBERNOTFOUND;
    }

    // need the property to be set
    if (!pdp || (pdp->cArgs < 1))
    {
			return DISP_E_BADPARAMCOUNT;
    }
    // if we don't have a value yet...
	  if (!pvt)
	  {
      // ...create one
		  pvt = new CComVariant();
		  if (!pvt)
      {
			  return E_OUTOFMEMORY;
      }
      m_Data[id] = pvt;
    }
		(*pvt) = (pdp->rgvarg[pdp->cArgs-1]);
		return S_OK;
	}

  // call a method: this works only if we have a value
	if (wFlags & DISPATCH_METHOD)
	{
		if (pvt && (VT_DISPATCH == pvt->vt) && (pvt->pdispVal))
		{
			return pvt->pdispVal->Invoke(
        0, IID_NULL, lcid, wFlags, pdp, pvarRes, pei, NULL);
		}
		return DISP_E_MEMBERNOTFOUND;
	}

  // get a property: only can do if we have a return value ptr:
  ENSURE_RETVAL(pvarRes);

  // init retval in any case
  VariantInit(pvarRes);
	if (pvt)
	{
    // have something, ok
		VariantCopy(pvarRes, pvt);
    return S_OK;
	}

	return DISP_E_MEMBERNOTFOUND;
}

//----------------------------------------------------------------------------
//  DeleteMemberByName
STDMETHODIMP CMagpieModuleExports::DeleteMemberByName(
  BSTR  bstrName,
  DWORD grfdex)
{
  DISPID dispid = CDispExIdManager::Get().GetID(bstrName);
  if (!dispid)
  {
	  return DISP_E_UNKNOWNNAME;
  }
  return DeleteMemberByDispID(dispid);
}

//----------------------------------------------------------------------------
//  DeleteMemberByDispID
STDMETHODIMP CMagpieModuleExports::DeleteMemberByDispID(
  DISPID id)
{
  CComVariant* pvt;
	if (m_Data.Lookup(id, pvt))
	{
    if (!m_bReadOnly && pvt)
    {
      // delete the data, but keep the entry in our data map.
      // IDispatchEx requires that the DISPID stays valid.
      delete pvt;
      m_Data[id] = NULL;
      return S_OK;
    }
    // Member exists but cannot be deleted.
    return S_FALSE;
	}
	return DISP_E_MEMBERNOTFOUND;
}

//----------------------------------------------------------------------------
//  GetMemberProperties
STDMETHODIMP CMagpieModuleExports::GetMemberProperties(
  DISPID  id,
  DWORD   grfdexFetch,
  DWORD * pgrfdex)
{
	return E_NOTIMPL;
}

//----------------------------------------------------------------------------
//  GetMemberName
STDMETHODIMP CMagpieModuleExports::GetMemberName(
  DISPID  id,
  BSTR  * pbstrName)
{
  return CDispExIdManager::Get().GetMemberName(id, pbstrName);
}

//----------------------------------------------------------------------------
//  GetNextDispID
STDMETHODIMP CMagpieModuleExports::GetNextDispID(
  DWORD     grfdex,
  DISPID    id,
  DISPID  * pid)
{
  ENSURE_RETVAL(pid);

  DISPID did;
  CComVariant *pvt;
  POSITION pos = NULL;
  if (id == DISPID_STARTENUM)
  {
    // get first entry
    pos = m_Data.GetStartPosition();
  }
  else
  {
    // get current entry
    pos = (POSITION)(const CAtlDispIDMap::CPair *)m_Data.Lookup(id);
    if (pos)
    {
      // have to call GetNextAssoc once to adjust pos
      // to the next entry
      m_Data.GetNextAssoc(pos, did, pvt);
    }
  }

  // as long as we have a valid pos...
  while(pos)
  {
    // ...get the entry at pos...
    m_Data.GetNextAssoc(pos, did, pvt);
    // ...and if it is not NULL...
    if (pvt)
    {
      // ...return the ID for that entry
      (*pid) = did;
      return S_OK;
    }
  }
  // sorry, no more data
  return S_FALSE;
}

//----------------------------------------------------------------------------
//  GetNameSpaceParent
STDMETHODIMP CMagpieModuleExports::GetNameSpaceParent(
  IUnknown  **  ppunk)
{
	return E_NOTIMPL;
}


#endif // def USE_MODULES_EXPORT_OBJECT
