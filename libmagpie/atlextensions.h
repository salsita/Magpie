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

template <class Base, class CTORARG> //Base must be derived from CComObjectRoot
class CComContainedObjectRefCtorArg : 
	public Base
{
public:
	typedef Base _BaseClass;
	CComContainedObjectRefCtorArg(_In_opt_ void* pv, CTORARG & arg) : _BaseClass(arg)
	{
		m_pOuterUnknown = (IUnknown*)pv;
	}
#ifdef _ATL_DEBUG_INTERFACES
	virtual ~CComContainedObjectRefCtorArg()
	{
		_AtlDebugInterfacesModule.DeleteNonAddRefThunk(_GetRawUnknown());
		_AtlDebugInterfacesModule.DeleteNonAddRefThunk(m_pOuterUnknown);
	}
#endif

	STDMETHOD_(ULONG, AddRef)() throw() 
	{
		return OuterAddRef();
	}
	STDMETHOD_(ULONG, Release)() throw() 
	{
		return OuterRelease();
	}
	STDMETHOD(QueryInterface)(
		_In_ REFIID iid, 
		_Deref_out_ void** ppvObject) throw()
	{
		return OuterQueryInterface(iid, ppvObject);
	}
	template <class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(
		_Deref_out_ Q** pp)
	{
		return QueryInterface(__uuidof(Q), (void**)pp);
	}
	//GetControllingUnknown may be virtual if the Base class has declared
	//DECLARE_GET_CONTROLLING_UNKNOWN()
	IUnknown* GetControllingUnknown() throw()
	{
#ifdef _ATL_DEBUG_INTERFACES
		IUnknown* p;
		_AtlDebugInterfacesModule.AddNonAddRefThunk(m_pOuterUnknown, _T("CComContainedObjectRefCtorArg"), &p);
		return p;
#else
		return m_pOuterUnknown;
#endif
	}
};


//contained is the user's class that derives from CComObjectRoot and whatever
//interfaces the user wants to support on the object
template <class contained, class CTORARG>
class CComAggObjectRefCtorArg :
	public IUnknown,
	public CComObjectRootEx< typename contained::_ThreadModel::ThreadModelNoCS >
{
public:
	typedef contained _BaseClass;
	CComAggObjectRefCtorArg(_In_opt_ void* pv, CTORARG & arg) : 
		m_contained(pv, arg)
	{
		_pAtlModule->Lock();
	}
	HRESULT _AtlInitialConstruct()
	{
		HRESULT hr = m_contained._AtlInitialConstruct();
		if (SUCCEEDED(hr))
		{
			hr = CComObjectRootEx< typename contained::_ThreadModel::ThreadModelNoCS >::_AtlInitialConstruct();
		}
		return hr;
	}
	//If you get a message that this call is ambiguous then you need to
	// override it in your class and call each base class' version of this
	HRESULT FinalConstruct()
	{
		CComObjectRootEx<contained::_ThreadModel::ThreadModelNoCS>::FinalConstruct();
		return m_contained.FinalConstruct();
	}
	void FinalRelease()
	{
		CComObjectRootEx<contained::_ThreadModel::ThreadModelNoCS>::FinalRelease();
		m_contained.FinalRelease();
	}
	// Set refcount to -(LONG_MAX/2) to protect destruction and 
	// also catch mismatched Release in debug builds
	virtual ~CComAggObjectRefCtorArg()
	{
		m_dwRef = -(LONG_MAX/2);
		FinalRelease();
#ifdef _ATL_DEBUG_INTERFACES
		_AtlDebugInterfacesModule.DeleteNonAddRefThunk(this);
#endif
		_pAtlModule->Unlock();
	}

	STDMETHOD_(ULONG, AddRef)() 
	{
		return InternalAddRef();
	}
	STDMETHOD_(ULONG, Release)()
	{
		ULONG l = InternalRelease();
		if (l == 0)
			delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(
		_In_ REFIID iid, 
		_Deref_out_ void** ppvObject)
	{
		ATLASSERT(ppvObject != NULL);
		if (ppvObject == NULL)
			return E_POINTER;
		*ppvObject = NULL;

		HRESULT hRes = S_OK;
		if (InlineIsEqualUnknown(iid))
		{
			*ppvObject = (void*)(IUnknown*)this;
			AddRef();
#ifdef _ATL_DEBUG_INTERFACES
			_AtlDebugInterfacesModule.AddThunk((IUnknown**)ppvObject, (LPCTSTR)contained::_GetEntries()[-1].dw, iid);
#endif // _ATL_DEBUG_INTERFACES
		}
		else
			hRes = m_contained._InternalQueryInterface(iid, ppvObject);
		return hRes;
	}
	template <class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(_Deref_out_ Q** pp)
	{
		return QueryInterface(__uuidof(Q), (void**)pp);
	}
	static HRESULT WINAPI CreateInstance(
		_Inout_opt_ LPUNKNOWN pUnkOuter, 
		_Deref_out_ CComAggObjectRefCtorArg<contained, CTORARG>** pp)
	{
		ATLASSERT(pp != NULL);
		if (pp == NULL)
			return E_POINTER;
		*pp = NULL;

		HRESULT hRes = E_OUTOFMEMORY;
		CComAggObjectRefCtorArg<contained>* p = NULL;
		ATLTRY((p = new CComAggObjectRefCtorArg<contained, CTORARG>(pUnkOuter)))
		if (p != NULL)
		{
			p->SetVoid(NULL);
			p->InternalFinalConstructAddRef();
			hRes = p->_AtlInitialConstruct();
			if (SUCCEEDED(hRes))
				hRes = p->FinalConstruct();
			if (SUCCEEDED(hRes))
				hRes = p->_AtlFinalConstruct();
			p->InternalFinalConstructRelease();
			if (hRes != S_OK)
			{
				delete p;
				p = NULL;
			}
		}
		*pp = p;
		return hRes;
	}

	CComContainedObjectRefCtorArg<contained, CTORARG> m_contained;
};

