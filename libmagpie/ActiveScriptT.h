/****************************************************************************
 * MagpieApplication.h : ActiveScriptT.h : Template class for script engine
 * implementation.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/
// ActiveScriptT.h : Template class for script engine implementation

#pragma once

#include "activscp.h"

// must be declared somewhere in a .cpp file
extern CLSID CLSID_JScript;
extern CLSID CLSID_JScript9;

/*============================================================================
 * template CActiveScriptT
 * Implements IActiveScriptSite.
 * Implementors can override methods of IActiveScriptSite they require to
 * handle.
 */
template<class Timpl> class CActiveScriptT : public IActiveScriptSite
{
public:
  // -------------------------------------------------------------------------
  // Initialization and cleanup methods.

  // Terminates script execution and unloads the script engine.
  void UnloadScriptEngine()
  {
	  if (m_ScriptEngine)
	  {
		  EXCEPINFO inf = {};
		  m_ScriptEngine->InterruptScriptThread(SCRIPTTHREADID_ALL, &inf, 0);
		  if (inf.bstrSource)
			  SysFreeString(inf.bstrSource);
		  if (inf.bstrDescription)
			  SysFreeString(inf.bstrDescription);
		  if (inf.bstrHelpFile)
			  SysFreeString(inf.bstrHelpFile);
	  }
	  m_ScriptGlobal.Release();
	  m_ScriptEngineParser.Release();
    if (m_ScriptEngine)
	  {
		  m_ScriptEngine->Close();
	  }
	  m_ScriptEngine.Release();
  }

  // Loads and prepares the script engine.
  //  LoadScriptEngine will leave the engine in SCRIPTSTATE_INITIALIZED.
  //  So after adding scripts put enigne (back) to state SCRIPTSTATE_CONNECTED
  //  to run the scripts.
  HRESULT LoadScriptEngine(CLSID &clsid)
  {
    if (m_ScriptEngine)
    {
      return E_UNEXPECTED;
    }
	  HRESULT hr = E_FAIL;
	  ATLASSERT(NULL == m_ScriptGlobal);

    do
    {
	    // create engine
	    hr = ::CoCreateInstance(
        clsid, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IActiveScript, (void **)&m_ScriptEngine);
      if(FAILED(hr)) break;

      // get IActiveScriptProperty-interface
      CComQIPtr<IActiveScriptProperty> propInterface(m_ScriptEngine);
      if (propInterface) {

        // and set correct version
        // *sigh*:
        // - there is no SCRIPTLANGUAGEVERSION_9
        // - SCRIPTLANGUAGEVERSION_MAX fails with E_INVALIDARG
        // - SCRIPTLANGUAGEVERSION_DEFAULT defaults to some older version
        // but we need jscript9. So we use 15, which is the highest value which is still
        // accepted by SetProperty.
        // This is totally undocumented, activscp.h does not contain any values above
        // SCRIPTLANGUAGEVERSION_5_8.
        // Also note that SetProperty will fail if vtVersion is not a VT_I4. So we cast
        // explicitly to int.
        CComVariant vtVersion((int)15);
        hr = propInterface->SetProperty(SCRIPTPROP_INVOKEVERSIONING, NULL, &vtVersion);
        ATLASSERT(SUCCEEDED(hr));
#ifdef _DEBUG

        CComVariant vt;
        CString name, verLow(_T("?")), verHigh(_T("?")), buildNo(_T("?"));
        name = (SUCCEEDED(propInterface->GetProperty(SCRIPTPROP_NAME, 0, &vt)))
          ? vt
          : _T("ERROR");
        vt.Clear();
        if (SUCCEEDED(propInterface->GetProperty(SCRIPTPROP_MAJORVERSION, 0, &vt))) {
          verHigh.Format(_T("%i"), vt.lVal);
        }
        vt.Clear();
        if (SUCCEEDED(propInterface->GetProperty(SCRIPTPROP_MINORVERSION, 0, &vt))) {
          verLow.Format(_T("%i"), vt.lVal);
        }
        vt.Clear();
        if (SUCCEEDED(propInterface->GetProperty(SCRIPTPROP_BUILDNUMBER, 0, &vt))) {
          buildNo.Format(_T("%i"), vt.lVal);
        }
        CString s;
        s.Format(_T("LOADED script engine \"%s %s.%s.%s\"\n"), name, verHigh, verLow, buildNo);
        ATLTRACE(s);
#endif
      }

      // set this as script site
      hr = m_ScriptEngine->SetScriptSite(
        static_cast<IActiveScriptSite*>(this));
      if(FAILED(hr)) break;

      // get IActiveScriptParse-interface
		  hr = m_ScriptEngine->QueryInterface(
        IID_IActiveScriptParse, (void**)&m_ScriptEngineParser);
      if(FAILED(hr)) break;

      // init IActiveScriptParse
		  hr = m_ScriptEngineParser->InitNew();
      if(FAILED(hr)) break;

      // get the script dispatch for global namespace
      CComPtr<IDispatch> pDisp;
		  hr = m_ScriptEngine->GetScriptDispatch(NULL, &pDisp);
      if(FAILED(hr)) break;

      m_ScriptGlobal = pDisp;
    } while(false);
    if (FAILED(hr))
    {
      // something failed, cleanup
      ATLASSERT(0);
	    UnloadScriptEngine();
    }
    return hr;
  }

  // -------------------------------------------------------------------------
  // Adding scripts

  // Add script as raw string. This is the lowlevel method.
  //  Before adding a script text make sure engine is in state
  //  SCRIPTSTATE_DISCONNECTED or SCRIPTSTATE_INITIALIZED.
  // lpszSource is the script encoded as UTF-16.
  // lpszModuleName is the namespace for the module, in case it is NULL
  //  the script will get added to the global namespace.
	HRESULT AddScript(LPCOLESTR lpszSource,
                    LPCOLESTR lpszModuleName = NULL,
                    LPCOLESTR lpszModuleLongName = NULL,
                    DWORD_PTR *pdwSourceContext = NULL // in/out
                    )
  {
    // dwSourceContext can contain now the parent context
	  ATLASSERT(m_ScriptEngine && m_ScriptEngineParser);
	  if(!m_ScriptEngine || !m_ScriptEngineParser)
    {
      return E_UNEXPECTED;
    }

    DWORD_PTR dwSourceContext = 0;
    __if_exists(Timpl::AddScriptForDebug)
    {
      DWORD_PTR dwParentSourceContext = (pdwSourceContext)
        ? (*pdwSourceContext)
        : 0;
      // add script to debugging support
      Timpl* pImpl = (Timpl*)this;
      IF_FAILED_RET(pImpl->AddScriptForDebug(m_ScriptEngine, lpszSource, lpszModuleName, lpszModuleLongName, dwSourceContext, dwParentSourceContext));
    }

    // parse script text
	  IF_FAILED_RET(m_ScriptEngineParser->ParseScriptText(
      lpszSource, lpszModuleName, 0, 0, dwSourceContext, 0, SCRIPTTEXT_HOSTMANAGESSOURCE|SCRIPTTEXT_ISVISIBLE, 0, 0));
    // set *pdwSourceContext to resulting source context
    if (pdwSourceContext)
    {
      (*pdwSourceContext) = dwSourceContext;
    }
    return S_OK;
  }

  // -------------------------------------------------------------------------
  // IActiveScriptSite implementation
	STDMETHOD(GetLCID)(LCID *plcid)
  {
	  *plcid = LOCALE_USER_DEFAULT;
	  return S_OK;
  }

  STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti)
  {
    return TYPE_E_ELEMENTNOTFOUND;
  }

	STDMETHOD(GetDocVersionString)(BSTR *pbstrVersion)
  {
    return E_NOTIMPL;
  }

	STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
  {
    return S_OK;
  }

	STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState)
  {
    ATLTRACE(_T("SCRIPT OnStateChange: %i\n"), ssScriptState);
    return S_OK;
  }

	STDMETHOD(OnScriptError)(IActiveScriptError *pscripterror)
  {
#ifdef _DEBUG
    CString sSrc;
    EXCEPINFO inf = {0};
    CString sErr, sDesc;

    pscripterror->GetExceptionInfo(&inf);
    if (inf.bstrDescription)
    {
	    sDesc = inf.bstrDescription;
    }
    if (inf.bstrSource)
    {
	    sErr = inf.bstrSource;
    }
    DWORD dwSourceContext;
    ULONG lLine = 0;
    long lChar = 0;
    pscripterror->GetSourcePosition(&dwSourceContext, &lLine, &lChar);

    CString sOut;
    sOut.Format(_T("================================================================================\nError 0x%08x\nat line %i pos %i:\n%s\n%s\n================================================================================\n"), inf.scode, lLine, lChar, sErr, sDesc);
    ATLTRACE(sOut);
#endif
    return S_OK;
  }

	STDMETHOD(OnEnterScript)()
  {
    return S_OK;
  }

	STDMETHOD(OnLeaveScript)()
  {
    return S_OK;
  }

protected:
  // -------------------------------------------------------------------------
  // protected members

  // the script engine itself
  CComPtr<IActiveScript>			m_ScriptEngine;

  // script parser
  CComPtr<IActiveScriptParse>	m_ScriptEngineParser;

  // IDispatchEx for global script namespace
  CComQIPtr<IDispatchEx>		  m_ScriptGlobal;
};
