/****************************************************************************
 * DispExIdManager.cpp : Implementation of CDispExIdManager.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "StdAfx.h"
#include "DispExIdManager.h"

/*============================================================================
 * class CDispExIdManager
 */

//----------------------------------------------------------------------------
//  CTOR
CDispExIdManager::CDispExIdManager(void)
  : m_NextID(1)
{
}

//----------------------------------------------------------------------------
//  static Get
CDispExIdManager & CDispExIdManager::Get()
{
  static CDispExIdManager instance;
  return instance;
}

//----------------------------------------------------------------------------
//  GetID
DISPID CDispExIdManager::GetID(
  LPCOLESTR lpsName,
  BOOL      bCreate)
{
  CString s(lpsName);
  DISPID id;
  if (m_IDs.Lookup(lpsName, id))
  {
    return id;
  }
  if (!bCreate)
  {
    return 0;
  }
  m_IDs[lpsName] = id = m_NextID++;
#ifdef CDispExIdManager_USE_REVERSELOOKUPMAP
  m_Names[id] = lpsName;
#endif
  return id;
}

//----------------------------------------------------------------------------
//  GetMemberName
HRESULT CDispExIdManager::GetMemberName(
  DISPID  id,
  BSTR  * pbstrName)
{
  ENSURE_RETVAL(pbstrName);
  CString sName;
#ifdef CDispExIdManager_USE_REVERSELOOKUPMAP
  if (m_Names.Lookup(id, sName))
  {
    (*pbstrName) = sName.AllocSysString();
    return S_OK;
  }
#else
  DISPID dispid;

  POSITION pos = m_IDs.GetStartPosition();
  while(pos)
  {
    m_IDs.GetNextAssoc(pos, sName, dispid);
    if (dispid == id)
    {
      (*pbstrName) = sName.AllocSysString();
      return S_OK;
    }
  }
#endif
  return DISP_E_UNKNOWNNAME;
}

//----------------------------------------------------------------------------
//  GetMemberName
void CDispExIdManager::Clear()
{
  m_IDs.RemoveAll();
#ifdef CDispExIdManager_USE_REVERSELOOKUPMAP
  m_Names.RemoveAll();
#endif
  m_NextID = 1;
}
