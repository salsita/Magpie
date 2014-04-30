/****************************************************************************
 * Misc.cpp : Implementation of some helper functions
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "StdAfx.h"
#include "Misc.h"

size_t Misc::SplitString(const CString & str, LPCTSTR tokens, CAtlList<CString> & list)
{
  list.RemoveAll();
  CAtlString token;
  int curPos = 0;

  token = str.Tokenize(tokens, curPos);
  while (-1 != curPos)
  {
    if (!token.IsEmpty())
    {
      list.AddTail(token);
    }
    token = str.Tokenize(tokens, curPos);
  }
  return list.GetCount();
}

