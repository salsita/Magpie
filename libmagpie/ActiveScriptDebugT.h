#pragma once

#include <activscp.h>

// IProcessDebugManager::CreateApplication / IProcessDebugManager::AddApplication
// seems to be buggy according to http://grownsoftware.com/activescripting-faq/hostdebug.htm#HD3.
// So far we didn't have problems, but if so we should use
// IProcessDebugManager::GetDefaultApplication instead.
// Disable this define to use GetDefaultApplication.
#define SCRIPTDEBUG_CREATE_APPLICATION

template<class Timpl> class CActiveScriptDebugT
  : public IActiveScriptSiteDebug
{
public:

  CActiveScriptDebugT()
#ifdef SCRIPTDEBUG_CREATE_APPLICATION
    : m_appCookie(0)
#endif
  {
  }

  ~CActiveScriptDebugT()
  {
    UninitializeDebugInterface();
  }

  //----------------------------------------------------------------------------
  //  InitializeDebugInterface
  HRESULT InitializeDebugInterface(LPCOLESTR appId)
  {
    if (FAILED(CoCreateInstance(CLSID_ProcessDebugManager, NULL, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER,
      IID_IProcessDebugManager, (void**)&m_debugManager)))
    {
      return S_FALSE; ///< on some PCs it is not available
    }

#ifdef SCRIPTDEBUG_CREATE_APPLICATION
    IF_FAILED_RET(m_debugManager->CreateApplication(&m_debugApp));
    IF_FAILED_RET(m_debugApp->SetName(appId));
    IF_FAILED_RET(m_debugManager->AddApplication(m_debugApp, &m_appCookie));
#else
    IF_FAILED_RET(m_debugManager->GetDefaultApplication(&m_debugApp));
#endif

    return S_OK;
  }

  //----------------------------------------------------------------------------
  //  UninitializeDebugInterface
  void UninitializeDebugInterface()
  {
    if (!m_debugManager)
    {
      return; ///< init failed
    }
    m_debugDocHelpers.RemoveAll();
#ifdef SCRIPTDEBUG_CREATE_APPLICATION
    if (m_appCookie)
    {
      m_debugManager->RemoveApplication(m_appCookie);
      m_appCookie = 0;
    }
#endif
    if (m_debugApp)
    {
      m_debugApp->Close();
      m_debugApp = NULL;
    }
    m_debugManager = NULL;
  }

  //----------------------------------------------------------------------------
  //  AddScriptForDebug
  //  Register a script for debugging support
  HRESULT AddScriptForDebug(
    IActiveScript*  scriptEngine, // ptr to the script engine
    LPCOLESTR       lpszSource,   // the actual script source
    LPCOLESTR       lpszDocName,  // a short name for debugging
    LPCOLESTR       lpszDocNameLong,  // a long name for debugging such as a filename
    DWORD_PTR     & dwSourceContext,  // OUT: the debbuging context
    DWORD_PTR       dwParentSourceContext = 0 // OPTIONAL: the debugging context of the logical parent
    )
  {
    if (!m_debugManager)
    {
      return E_UNEXPECTED; ///< init failed
    }
    // see if we have a parent. it is safe to look the parent up even if dwParentSourceContext
    // is 0 because we will not find a parent in this case. this safes one additional if-clause
    // since we have to check debugDocHelperParent for NULL anyway.
    CComPtr<IDebugDocumentHelper>
      debugDocHelperParent(m_debugDocHelpers.Lookup(dwParentSourceContext));

    CComPtr<IDebugDocumentHelper> debugDocHelper;
    if (debugDocHelperParent)
    {
      // if we have a parent add script text to that instance
      // add source code
      IF_FAILED_RET(debugDocHelperParent->AddUnicodeText(lpszSource));
      // adjust out value to the found context
      dwSourceContext = dwParentSourceContext;
      // now the executed script appears nicely at the bottom of the according module
      // script - the parent. This is nice for executing free scripts in a module, they
      // don't appear in the debugger as new blocks, they get added to the parent script.
      // But pls see the comments in CMagpieModule::Init
    }
    else
    {
      // create a new IDebugDocumentHelper
      IF_FAILED_RET(m_debugManager->CreateDebugDocumentHelper(NULL, &debugDocHelper));
      IF_FAILED_RET(debugDocHelper->Init(
        m_debugApp,
        lpszDocName,
        (lpszDocNameLong) ? lpszDocNameLong : lpszDocName,
        TEXT_DOC_ATTR_READONLY));
      // Modules don't have a parent
      IF_FAILED_RET(debugDocHelper->Attach(NULL));
      // add source code
      IF_FAILED_RET(debugDocHelper->AddUnicodeText(lpszSource));
      // define a block for this code
      IF_FAILED_RET(debugDocHelper->DefineScriptBlock(0, (ULONG)wcslen(lpszSource),
        scriptEngine, FALSE, &dwSourceContext));
      m_debugDocHelpers.Add(dwSourceContext, debugDocHelper);
    }

    // dwSourceContext contains now the debugging source context
    return S_OK;
  }

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // IActiveScriptSiteDebug implementation

  //----------------------------------------------------------------------------
  //  GetDocumentContextFromPosition
  STDMETHODIMP GetDocumentContextFromPosition(
    DWORD_PTR dwSourceContext,
    ULONG uCharacterOffset,
    ULONG uNumChars,
    IDebugDocumentContext **ppsc)
  {
    CComPtr<IDebugDocumentHelper> debugDocHelper = m_debugDocHelpers.Lookup(dwSourceContext);
    if (!debugDocHelper)
    {
      return E_UNEXPECTED;
    }

    ULONG ulStartPos = 0;
    IF_FAILED_RET(debugDocHelper->GetScriptBlockInfo(dwSourceContext, NULL, &ulStartPos, NULL));

    return debugDocHelper->CreateDebugDocumentContext(ulStartPos + uCharacterOffset, uNumChars, ppsc);
  }

  //----------------------------------------------------------------------------
  //  GetApplication
  STDMETHODIMP GetApplication(IDebugApplication **ppda)
  {
    if (!ppda)
    {
      return E_POINTER;
    }
    *ppda = NULL;
    if (m_debugApp)
    {
      return m_debugApp.CopyTo(ppda);
    }
    else
    {
      return E_FAIL;
    }
  }

  //----------------------------------------------------------------------------
  //  GetRootApplicationNode
  STDMETHODIMP GetRootApplicationNode(IDebugApplicationNode **ppdanRoot)
  {
    if (!ppdanRoot)
    {
      return E_POINTER;
    }
    if (m_debugApp)
    {
      return m_debugApp->GetRootNode(ppdanRoot);
    }
    else
    {
      return E_FAIL;
    }
  }

  //----------------------------------------------------------------------------
  //  OnScriptErrorDebug
  //  Allows a smart host to control the handling of runtime errors
  STDMETHODIMP OnScriptErrorDebug(IActiveScriptErrorDebug* pErrorDebug, BOOL* pfEnterDebugger, BOOL* pfCallOnScriptErrorWhenContinuing)
  {
    if (pfEnterDebugger)
    {
      *pfEnterDebugger = TRUE;
    }
    if (pfCallOnScriptErrorWhenContinuing)
    {
      *pfCallOnScriptErrorWhenContinuing = TRUE;
    }
    return S_OK;
  }

private:
#ifdef SCRIPTDEBUG_CREATE_APPLICATION
  DWORD                         m_appCookie;
#endif
  CComPtr<IDebugApplication>    m_debugApp;
  CComPtr<IProcessDebugManager> m_debugManager;
  CSimpleMap<DWORD_PTR, CComPtr<IDebugDocumentHelper> >
                                m_debugDocHelpers; ///< IDebugDocumentHelper instance for each module

};
