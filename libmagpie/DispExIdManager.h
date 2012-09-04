/****************************************************************************
 * DispExIdManager.h : A singleton class for managing DISPIDs for
 *  IDispatchEx. These DISPIDs should be unique and valid even after deleting
 *  properties from an IDispatchEx object.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

/*============================================================================
 * class CDispExIdManager
 * By default a reverse lookup (DISPID given, name requested) is done by
 *  iterating the m_IDs map. Since this method will not get called so often
 *  in normal usage this is enough. However, if heavy use of
 *  GetMemberName() is expected define CDispExIdManager_USE_REVERSELOOKUPMAP
 *  to add a reverse lookup map.
 */
class CDispExIdManager
{
public:
  // -------------------------------------------------------------------------
  // static singleton getter
  static CDispExIdManager & Get();

public:
  // -------------------------------------------------------------------------
  // public methods

  // Get an ID for a given name.
  // If bCreate is true the name is created if it does not exist.
  DISPID GetID(LPCOLESTR  lpsName,
               BOOL       bCreate = FALSE);

  // Reverse resolution of a name. Get the name for a given DISPID.
  HRESULT GetMemberName(DISPID  id,
                        BSTR  * pbstrName);

  // Clear, remove all name / DISPIDs pairs from the map.
  void Clear();

private:
  // -------------------------------------------------------------------------
  // ctor
  CDispExIdManager(void);

private:
  // -------------------------------------------------------------------------
  // private members

  // The next DISPID to be given to a client
  DISPID                    m_NextID;

  // Map of names / IDs
  CAtlMap<CString, DISPID>  m_IDs;

#ifdef CDispExIdManager_USE_REVERSELOOKUPMAP
  // Map of IDs / names
  CAtlMap<DISPID, CString>  m_Names;
#endif
};
