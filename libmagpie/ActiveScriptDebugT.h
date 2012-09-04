#pragma once

#include <activscp.h>

template<class Timpl> class CActiveScriptDebugT
  : public IActiveScriptSiteDebug
{
public:

  CActiveScriptDebugT() :
    m_appCookie(0),
    m_appAdded(FALSE)
  {
  }

  ~CActiveScriptDebugT()
  {
    UninitializeDebugInterface();
  }

  HRESULT InitializeDebugInterface(LPCOLESTR appId)
  {
    if (FAILED(CoCreateInstance(CLSID_ProcessDebugManager, NULL, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER,
      IID_IProcessDebugManager, (void**)&m_debugManager)))
    {
      return S_FALSE; ///< on some PCs it is not available
    }

    IF_FAILED_RET(m_debugManager->CreateApplication(&m_debugApp));
    IF_FAILED_RET(m_debugApp->SetName(appId));
    IF_FAILED_RET(m_debugManager->AddApplication(m_debugApp, &m_appCookie));
    m_appAdded = TRUE;

    return S_OK;
  }

  void UninitializeDebugInterface()
  {
    if (!m_debugManager)
    {
      return; ///< init failed
    }
    m_debugDocHelpers.RemoveAll();
    if (m_appAdded)
    {
      m_debugManager->RemoveApplication(m_appCookie);
      m_appAdded = FALSE;
    }
    if (m_debugApp)
    {
      m_debugApp->Close();
      m_debugApp = NULL;
    }
    m_debugManager = NULL;
  }

  /**
   * Registers a script file.
   */
  HRESULT AddScriptFile(IActiveScript* scriptEngine, LPCWSTR filePath, LPCWSTR moduleName, LPCWSTR scriptText, DWORD_PTR &sourceContext)
  {
    if (!m_debugManager)
    {
      return S_FALSE; ///< init failed
    }

    CComPtr<IDebugDocumentHelper> debugDocHelper;
    IF_FAILED_RET(m_debugManager->CreateDebugDocumentHelper(NULL, &debugDocHelper));
    IF_FAILED_RET(debugDocHelper->Init(m_debugApp, moduleName, filePath, TEXT_DOC_ATTR_READONLY));

    IF_FAILED_RET(debugDocHelper->AddUnicodeText(scriptText));
    IF_FAILED_RET(debugDocHelper->DefineScriptBlock(0, (ULONG)wcslen(scriptText), scriptEngine, FALSE, &sourceContext));
    IF_FAILED_RET(debugDocHelper->Attach(NULL));

    m_debugDocHelpers.Add(sourceContext, debugDocHelper);

    return S_OK;
  }

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

  STDMETHODIMP GetApplication(IDebugApplication **ppda)
  {
    if (!ppda)
    {
      return E_INVALIDARG;
    }
    if (m_debugApp)
    {
      return m_debugApp.CopyTo(ppda);
    } else {
      return E_UNEXPECTED;
    }
  }

  STDMETHODIMP GetRootApplicationNode(IDebugApplicationNode **ppdanRoot)
  {
    if (!ppdanRoot)
    {
      return E_INVALIDARG;
    }
    if (m_debugApp)
    {
      return m_debugApp->GetRootNode(ppdanRoot);
    } else {
      return E_FAIL;
    }
  }

  // Allows a smart host to control the handling of runtime errors
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
  DWORD   m_appCookie;
  BOOL    m_appAdded;
  CComPtr<IDebugApplication>    m_debugApp;
  CComPtr<IProcessDebugManager> m_debugManager;
  CSimpleMap<DWORD_PTR, CComPtr<IDebugDocumentHelper> > m_debugDocHelpers; ///< IDebugDocumentHelper instance for each module

};
