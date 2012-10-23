/****************************************************************************
 * MagpieConsole.cpp : Implementation of CMagpieConsole
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#include "stdafx.h"
#include "MagpieApplication.h"

/*============================================================================
 * class CMagpieApplication
 */

LPCOLESTR CMagpieConsole::EMPTY_MODULE_ID = _T("???");

//----------------------------------------------------------------------------
//  CTOR
CMagpieConsole::CMagpieConsole(
  CMagpieApplication & application) :
         m_Application(application)
{
}

//----------------------------------------------------------------------------
//  Init
HRESULT CMagpieConsole::Init()
{
  m_sCurrentModuleID = EMPTY_MODULE_ID;
  return S_OK;
}

//----------------------------------------------------------------------------
//  Shutdown
HRESULT CMagpieConsole::Shutdown()
{
  return S_OK;
}

//----------------------------------------------------------------------------
//  EnterModule
void CMagpieConsole::EnterModule(
  LPCOLESTR lpszModuleID)
{
  m_ModuleStack.AddTail(m_sCurrentModuleID);
  m_sCurrentModuleID = lpszModuleID;
}

//----------------------------------------------------------------------------
//  ExitModule
void CMagpieConsole::ExitModule()
{
  if (m_ModuleStack.IsEmpty())
  {
    m_sCurrentModuleID = EMPTY_MODULE_ID;
  }
  else
  {
    m_sCurrentModuleID = m_ModuleStack.RemoveTail();
  }
}

//----------------------------------------------------------------------------
//  FinalConstruct
HRESULT CMagpieConsole::FinalConstruct()
{
	return S_OK;
}

//----------------------------------------------------------------------------
//  FinalRelease
void CMagpieConsole::FinalRelease()
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// IMagpieConsole implementation

//----------------------------------------------------------------------------
//  log
STDMETHODIMP CMagpieConsole::log(
  SAFEARRAY* pVals)
{
  Fire_ConsoleEvent(LT_LOG, m_sCurrentModuleID, pVals);
  return S_OK;
}

//----------------------------------------------------------------------------
//  debug
STDMETHODIMP CMagpieConsole::debug(
  SAFEARRAY* pVals)
{
  Fire_ConsoleEvent(LT_DEBUG, m_sCurrentModuleID, pVals);
  return S_OK;
}

//----------------------------------------------------------------------------
//  info
STDMETHODIMP CMagpieConsole::info(
  SAFEARRAY* pVals)
{
  Fire_ConsoleEvent(LT_INFO, m_sCurrentModuleID, pVals);
  return S_OK;
}

//----------------------------------------------------------------------------
//  warn
STDMETHODIMP CMagpieConsole::warn(
  SAFEARRAY* pVals)
{
  Fire_ConsoleEvent(LT_WARN, m_sCurrentModuleID, pVals);
  return S_OK;
}

//----------------------------------------------------------------------------
//  error
STDMETHODIMP CMagpieConsole::error(
  SAFEARRAY* pVals)
{
  Fire_ConsoleEvent(LT_ERROR, m_sCurrentModuleID, pVals);
  return S_OK;
}
