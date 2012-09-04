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
// IMagpieApplication implementation

//----------------------------------------------------------------------------
//  log
STDMETHODIMP CMagpieConsole::log(
  VARIANT val)
{
  Fire_OnLog(val, m_sCurrentModuleID);
  return S_OK;
}

//----------------------------------------------------------------------------
//  debug
STDMETHODIMP CMagpieConsole::debug(
  VARIANT val)
{
  Fire_OnDebug(val, m_sCurrentModuleID);
  return S_OK;
}

//----------------------------------------------------------------------------
//  info
STDMETHODIMP CMagpieConsole::info(
  VARIANT val)
{
  Fire_OnInfo(val, m_sCurrentModuleID);
  return S_OK;
}

//----------------------------------------------------------------------------
//  warn
STDMETHODIMP CMagpieConsole::warn(
  VARIANT val)
{
  Fire_OnWarn(val, m_sCurrentModuleID);
  return S_OK;
}

//----------------------------------------------------------------------------
//  error
STDMETHODIMP CMagpieConsole::error(
  VARIANT val)
{
  Fire_OnError(val, m_sCurrentModuleID);
  return S_OK;
}
