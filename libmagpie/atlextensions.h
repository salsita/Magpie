/****************************************************************************
 * atlextensions.h : Some stuff that ATL does not have, but we need
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

#pragma once

/*============================================================================
 * template CComObjectStackRefCtorArg
 * This is a copy of CComObjectStackEx that takes a reference of type CTORARG
 * and passes it to the constructor of the implementing class. Usually you
 * would pass the aggregating class here.
 * Allows to to use references with the constructor instead of pointers with
 * an Init() method. Less error prone than the pointer method.
 * Otherwise it is identical to CComObjectStackEx.
 */

// Base must be derived from CComObjectRoot
template <class Base, class CTORARG>
class CComObjectStackRefCtorArg : public Base
{
public:
	typedef Base _BaseClass;

  CComObjectStackRefCtorArg(CTORARG & arg) : Base(arg)
	{ 
#ifdef _DEBUG
		m_dwRef = 0;
#endif
		m_hResFinalConstruct = _AtlInitialConstruct();
		if (SUCCEEDED(m_hResFinalConstruct))
			m_hResFinalConstruct = FinalConstruct(); 
	}

	virtual ~CComObjectStackRefCtorArg()
	{
		// This assert indicates mismatched ref counts.
		//
		// The ref count has no control over the
		// lifetime of this object, so you must ensure
		// by some other means that the object remains 
		// alive while clients have references to its interfaces.
		ATLASSUME(m_dwRef == 0);
		FinalRelease();
#ifdef _ATL_DEBUG_INTERFACES
		_AtlDebugInterfacesModule.DeleteNonAddRefThunk(_GetRawUnknown());
#endif
	}

	STDMETHOD_(ULONG, AddRef)() throw()
	{
#ifdef _DEBUG
		return InternalAddRef();
#else
		return 0;
#endif
	}

	STDMETHOD_(ULONG, Release)() throw()
	{
#ifdef _DEBUG
		return InternalRelease();
#else
		return 0;
#endif
	}

	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) throw()
	{
		return _InternalQueryInterface(iid, ppvObject);
	}

	HRESULT m_hResFinalConstruct;

};
