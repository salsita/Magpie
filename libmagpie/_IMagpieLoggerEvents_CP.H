/****************************************************************************
 * _IMagpieLoggerEvents_CP.H : Declaration of the CProxy_IMagpieLoggerEvents
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

template<class T>
class CProxy_IMagpieLoggerEvents :
	public IConnectionPointImpl<T, &__uuidof(_IMagpieLoggerEvents)>
{
public:
  // NOTE: LogFacility maps directly to the dispatch ID of the according event!
	HRESULT Fire_ConsoleEvent(LogFacility fac, BSTR bsModuleID, SAFEARRAY* pVals)
	{
    if (!pVals->cbElements)
    {
      // Don't log when there is nothing to log. This prevents ATL from throwing
      // a bunch of exceptions for an empty SAFEARRAY.
      return E_INVALIDARG;
    }
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
        avarParams[1] = bsModuleID;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = pVals;
				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(fac, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
			}
		}
		return hr;
	}
/*
  // prototype for a log function - old style
  HRESULT Fire_OnLog( VARIANT val,  BSTR bsModuleID)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = val;
				avarParams[0] = bsModuleID;
				avarParams[0].vt = VT_BSTR;
				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
			}
		}
		return hr;
	}
*/
};

