

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jan 03 17:11:50 2014
 */
/* Compiler settings for src\Magpie.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Magpie_i_h__
#define __Magpie_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMagpieApplication_FWD_DEFINED__
#define __IMagpieApplication_FWD_DEFINED__
typedef interface IMagpieApplication IMagpieApplication;
#endif 	/* __IMagpieApplication_FWD_DEFINED__ */


#ifndef ___IMagpieLoggerEvents_FWD_DEFINED__
#define ___IMagpieLoggerEvents_FWD_DEFINED__
typedef interface _IMagpieLoggerEvents _IMagpieLoggerEvents;
#endif 	/* ___IMagpieLoggerEvents_FWD_DEFINED__ */


#ifndef __IMagpieObjectCreator_FWD_DEFINED__
#define __IMagpieObjectCreator_FWD_DEFINED__
typedef interface IMagpieObjectCreator IMagpieObjectCreator;
#endif 	/* __IMagpieObjectCreator_FWD_DEFINED__ */


#ifndef __IMagpieConsole_FWD_DEFINED__
#define __IMagpieConsole_FWD_DEFINED__
typedef interface IMagpieConsole IMagpieConsole;
#endif 	/* __IMagpieConsole_FWD_DEFINED__ */


#ifndef __IMagpieModuleRestricted_FWD_DEFINED__
#define __IMagpieModuleRestricted_FWD_DEFINED__
typedef interface IMagpieModuleRestricted IMagpieModuleRestricted;
#endif 	/* __IMagpieModuleRestricted_FWD_DEFINED__ */


#ifndef __IMagpieModule_FWD_DEFINED__
#define __IMagpieModule_FWD_DEFINED__
typedef interface IMagpieModule IMagpieModule;
#endif 	/* __IMagpieModule_FWD_DEFINED__ */


#ifndef __IMagpieRequire_FWD_DEFINED__
#define __IMagpieRequire_FWD_DEFINED__
typedef interface IMagpieRequire IMagpieRequire;
#endif 	/* __IMagpieRequire_FWD_DEFINED__ */


#ifndef __IMagpieScriptLoader_FWD_DEFINED__
#define __IMagpieScriptLoader_FWD_DEFINED__
typedef interface IMagpieScriptLoader IMagpieScriptLoader;
#endif 	/* __IMagpieScriptLoader_FWD_DEFINED__ */


#ifndef __IMagpieScriptLoader2_FWD_DEFINED__
#define __IMagpieScriptLoader2_FWD_DEFINED__
typedef interface IMagpieScriptLoader2 IMagpieScriptLoader2;
#endif 	/* __IMagpieScriptLoader2_FWD_DEFINED__ */


#ifndef __MagpieApplication_FWD_DEFINED__
#define __MagpieApplication_FWD_DEFINED__

#ifdef __cplusplus
typedef class MagpieApplication MagpieApplication;
#else
typedef struct MagpieApplication MagpieApplication;
#endif /* __cplusplus */

#endif 	/* __MagpieApplication_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Magpie_0000_0000 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_Magpie_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Magpie_0000_0000_v0_0_s_ifspec;

#ifndef __IMagpieApplication_INTERFACE_DEFINED__
#define __IMagpieApplication_INTERFACE_DEFINED__

/* interface IMagpieApplication */
/* [unique][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EB490C81-7F44-42a7-8D45-02EFA8B8277E")
    IMagpieApplication : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [string][in] */ const OLECHAR *lpszAppName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddScriptLoader( 
            IMagpieScriptLoader *pLoader) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddFilesystemScriptLoader( 
            /* [string][in] */ const OLECHAR *lpszRootPath) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddResourceScriptLoader( 
            /* [in] */ HANDLE_PTR hModule) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [string][in] */ const OLECHAR *lpszModuleID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RunScript( 
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [string][in] */ const OLECHAR *lpszScript) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ExecuteScript( 
            /* [string][in] */ const OLECHAR *lpszScript,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ VARIANT *result) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ExecuteScriptGlobal( 
            /* [string][in] */ const OLECHAR *lpszScript,
            /* [retval][out] */ VARIANT *result) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ExecuteGlobal( 
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ VARIANT *result) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddExtension( 
            /* [string][in] */ const OLECHAR *lpszExtensionName,
            /* [in] */ IDispatch *pDispExtension) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetModuleObject( 
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ IMagpieModuleRestricted **ppRet) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddNamedItem( 
            /* [string][in] */ const OLECHAR *lpszName,
            /* [in] */ IDispatch *pDisp,
            /* [in] */ ULONG ulFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetScriptDispatch( 
            LPCOLESTR pstrItemName,
            IDispatch **ppdisp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieApplication * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieApplication * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszAppName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddScriptLoader )( 
            IMagpieApplication * This,
            IMagpieScriptLoader *pLoader);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddFilesystemScriptLoader )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszRootPath);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddResourceScriptLoader )( 
            IMagpieApplication * This,
            /* [in] */ HANDLE_PTR hModule);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszModuleID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RunScript )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [string][in] */ const OLECHAR *lpszScript);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ExecuteScript )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszScript,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ VARIANT *result);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ExecuteScriptGlobal )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszScript,
            /* [retval][out] */ VARIANT *result);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ExecuteGlobal )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ VARIANT *result);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddExtension )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszExtensionName,
            /* [in] */ IDispatch *pDispExtension);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetModuleObject )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ IMagpieModuleRestricted **ppRet);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddNamedItem )( 
            IMagpieApplication * This,
            /* [string][in] */ const OLECHAR *lpszName,
            /* [in] */ IDispatch *pDisp,
            /* [in] */ ULONG ulFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMagpieApplication * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetScriptDispatch )( 
            IMagpieApplication * This,
            LPCOLESTR pstrItemName,
            IDispatch **ppdisp);
        
        END_INTERFACE
    } IMagpieApplicationVtbl;

    interface IMagpieApplication
    {
        CONST_VTBL struct IMagpieApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieApplication_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieApplication_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieApplication_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieApplication_Init(This,lpszAppName)	\
    ( (This)->lpVtbl -> Init(This,lpszAppName) ) 

#define IMagpieApplication_AddScriptLoader(This,pLoader)	\
    ( (This)->lpVtbl -> AddScriptLoader(This,pLoader) ) 

#define IMagpieApplication_AddFilesystemScriptLoader(This,lpszRootPath)	\
    ( (This)->lpVtbl -> AddFilesystemScriptLoader(This,lpszRootPath) ) 

#define IMagpieApplication_AddResourceScriptLoader(This,hModule)	\
    ( (This)->lpVtbl -> AddResourceScriptLoader(This,hModule) ) 

#define IMagpieApplication_Run(This,lpszModuleID)	\
    ( (This)->lpVtbl -> Run(This,lpszModuleID) ) 

#define IMagpieApplication_RunScript(This,lpszModuleID,lpszScript)	\
    ( (This)->lpVtbl -> RunScript(This,lpszModuleID,lpszScript) ) 

#define IMagpieApplication_ExecuteScript(This,lpszScript,lpszModuleID,result)	\
    ( (This)->lpVtbl -> ExecuteScript(This,lpszScript,lpszModuleID,result) ) 

#define IMagpieApplication_ExecuteScriptGlobal(This,lpszScript,result)	\
    ( (This)->lpVtbl -> ExecuteScriptGlobal(This,lpszScript,result) ) 

#define IMagpieApplication_ExecuteGlobal(This,lpszModuleID,result)	\
    ( (This)->lpVtbl -> ExecuteGlobal(This,lpszModuleID,result) ) 

#define IMagpieApplication_AddExtension(This,lpszExtensionName,pDispExtension)	\
    ( (This)->lpVtbl -> AddExtension(This,lpszExtensionName,pDispExtension) ) 

#define IMagpieApplication_GetModuleObject(This,lpszModuleID,ppRet)	\
    ( (This)->lpVtbl -> GetModuleObject(This,lpszModuleID,ppRet) ) 

#define IMagpieApplication_AddNamedItem(This,lpszName,pDisp,ulFlags)	\
    ( (This)->lpVtbl -> AddNamedItem(This,lpszName,pDisp,ulFlags) ) 

#define IMagpieApplication_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IMagpieApplication_GetScriptDispatch(This,pstrItemName,ppdisp)	\
    ( (This)->lpVtbl -> GetScriptDispatch(This,pstrItemName,ppdisp) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieApplication_INTERFACE_DEFINED__ */



#ifndef __MagpieLib_LIBRARY_DEFINED__
#define __MagpieLib_LIBRARY_DEFINED__

/* library MagpieLib */
/* [version][uuid][helpstring] */ 

/* [uuid][helpstring] */ 
enum  DECLSPEC_UUID("C1425F75-242A-47db-AAC1-97ED8CF88AD9") LogFacility
    {	LT_LOG	= 1,
	LT_DEBUG	= 2,
	LT_INFO	= 3,
	LT_WARN	= 4,
	LT_ERROR	= 5
    } ;

EXTERN_C const IID LIBID_MagpieLib;

#ifndef ___IMagpieLoggerEvents_DISPINTERFACE_DEFINED__
#define ___IMagpieLoggerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IMagpieLoggerEvents */
/* [uuid][helpstring] */ 


EXTERN_C const IID DIID__IMagpieLoggerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C9FC6B1C-26FD-4af7-8F5F-497A7EA4FD89")
    _IMagpieLoggerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IMagpieLoggerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IMagpieLoggerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IMagpieLoggerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IMagpieLoggerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IMagpieLoggerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IMagpieLoggerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IMagpieLoggerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IMagpieLoggerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IMagpieLoggerEventsVtbl;

    interface _IMagpieLoggerEvents
    {
        CONST_VTBL struct _IMagpieLoggerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IMagpieLoggerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IMagpieLoggerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IMagpieLoggerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IMagpieLoggerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IMagpieLoggerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IMagpieLoggerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IMagpieLoggerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IMagpieLoggerEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IMagpieObjectCreator_INTERFACE_DEFINED__
#define __IMagpieObjectCreator_INTERFACE_DEFINED__

/* interface IMagpieObjectCreator */
/* [unique][oleautomation][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieObjectCreator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("04E73040-64B5-4624-BA53-F0663137D32B")
    IMagpieObjectCreator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE createObject( 
            /* [in] */ BSTR aObjectName,
            /* [retval][out] */ LPDISPATCH *aRetObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieObjectCreatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieObjectCreator * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieObjectCreator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieObjectCreator * This);
        
        HRESULT ( STDMETHODCALLTYPE *createObject )( 
            IMagpieObjectCreator * This,
            /* [in] */ BSTR aObjectName,
            /* [retval][out] */ LPDISPATCH *aRetObject);
        
        END_INTERFACE
    } IMagpieObjectCreatorVtbl;

    interface IMagpieObjectCreator
    {
        CONST_VTBL struct IMagpieObjectCreatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieObjectCreator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieObjectCreator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieObjectCreator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieObjectCreator_createObject(This,aObjectName,aRetObject)	\
    ( (This)->lpVtbl -> createObject(This,aObjectName,aRetObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieObjectCreator_INTERFACE_DEFINED__ */


#ifndef __IMagpieConsole_INTERFACE_DEFINED__
#define __IMagpieConsole_INTERFACE_DEFINED__

/* interface IMagpieConsole */
/* [unique][nonextensible][dual][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieConsole;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5FCD00DD-F021-496a-9FD1-24B7195FF5D9")
    IMagpieConsole : public IDispatch
    {
    public:
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE log( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE debug( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE info( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE warn( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE error( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieConsoleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieConsole * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieConsole * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieConsole * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMagpieConsole * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMagpieConsole * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMagpieConsole * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMagpieConsole * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *log )( 
            IMagpieConsole * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *debug )( 
            IMagpieConsole * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *info )( 
            IMagpieConsole * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *warn )( 
            IMagpieConsole * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *error )( 
            IMagpieConsole * This,
            /* [in] */ SAFEARRAY * pVals);
        
        END_INTERFACE
    } IMagpieConsoleVtbl;

    interface IMagpieConsole
    {
        CONST_VTBL struct IMagpieConsoleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieConsole_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieConsole_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieConsole_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieConsole_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMagpieConsole_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMagpieConsole_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMagpieConsole_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMagpieConsole_log(This,pVals)	\
    ( (This)->lpVtbl -> log(This,pVals) ) 

#define IMagpieConsole_debug(This,pVals)	\
    ( (This)->lpVtbl -> debug(This,pVals) ) 

#define IMagpieConsole_info(This,pVals)	\
    ( (This)->lpVtbl -> info(This,pVals) ) 

#define IMagpieConsole_warn(This,pVals)	\
    ( (This)->lpVtbl -> warn(This,pVals) ) 

#define IMagpieConsole_error(This,pVals)	\
    ( (This)->lpVtbl -> error(This,pVals) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieConsole_INTERFACE_DEFINED__ */


#ifndef __IMagpieModuleRestricted_INTERFACE_DEFINED__
#define __IMagpieModuleRestricted_INTERFACE_DEFINED__

/* interface IMagpieModuleRestricted */
/* [unique][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieModuleRestricted;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77CA8F88-66B2-4bdf-873F-8C33BD814EF5")
    IMagpieModuleRestricted : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetExportsObject( 
            /* [retval][out] */ LPDISPATCH *ppRet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieModuleRestrictedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieModuleRestricted * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieModuleRestricted * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieModuleRestricted * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExportsObject )( 
            IMagpieModuleRestricted * This,
            /* [retval][out] */ LPDISPATCH *ppRet);
        
        END_INTERFACE
    } IMagpieModuleRestrictedVtbl;

    interface IMagpieModuleRestricted
    {
        CONST_VTBL struct IMagpieModuleRestrictedVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieModuleRestricted_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieModuleRestricted_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieModuleRestricted_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieModuleRestricted_GetExportsObject(This,ppRet)	\
    ( (This)->lpVtbl -> GetExportsObject(This,ppRet) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieModuleRestricted_INTERFACE_DEFINED__ */


#ifndef __IMagpieModule_INTERFACE_DEFINED__
#define __IMagpieModule_INTERFACE_DEFINED__

/* interface IMagpieModule */
/* [unique][nonextensible][dual][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CE0BBC10-B7B6-476f-A799-A59A4105C3BC")
    IMagpieModule : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_id( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieModuleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieModule * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieModule * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieModule * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMagpieModule * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMagpieModule * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMagpieModule * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMagpieModule * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_id )( 
            IMagpieModule * This,
            /* [retval][out] */ VARIANT *pVal);
        
        END_INTERFACE
    } IMagpieModuleVtbl;

    interface IMagpieModule
    {
        CONST_VTBL struct IMagpieModuleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieModule_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieModule_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieModule_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieModule_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMagpieModule_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMagpieModule_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMagpieModule_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMagpieModule_get_id(This,pVal)	\
    ( (This)->lpVtbl -> get_id(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieModule_INTERFACE_DEFINED__ */


#ifndef __IMagpieRequire_INTERFACE_DEFINED__
#define __IMagpieRequire_INTERFACE_DEFINED__

/* interface IMagpieRequire */
/* [unique][nonextensible][dual][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieRequire;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("82440242-4B02-4690-B2A0-171CA206136A")
    IMagpieRequire : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE require( 
            /* [in] */ BSTR moduleID,
            /* [retval][out] */ VARIANT *pvtExports) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_main( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieRequireVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieRequire * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieRequire * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieRequire * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMagpieRequire * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMagpieRequire * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMagpieRequire * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMagpieRequire * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *require )( 
            IMagpieRequire * This,
            /* [in] */ BSTR moduleID,
            /* [retval][out] */ VARIANT *pvtExports);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_main )( 
            IMagpieRequire * This,
            /* [retval][out] */ VARIANT *pVal);
        
        END_INTERFACE
    } IMagpieRequireVtbl;

    interface IMagpieRequire
    {
        CONST_VTBL struct IMagpieRequireVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieRequire_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieRequire_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieRequire_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieRequire_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMagpieRequire_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMagpieRequire_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMagpieRequire_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMagpieRequire_require(This,moduleID,pvtExports)	\
    ( (This)->lpVtbl -> require(This,moduleID,pvtExports) ) 

#define IMagpieRequire_get_main(This,pVal)	\
    ( (This)->lpVtbl -> get_main(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieRequire_INTERFACE_DEFINED__ */


#ifndef __IMagpieScriptLoader_INTERFACE_DEFINED__
#define __IMagpieScriptLoader_INTERFACE_DEFINED__

/* interface IMagpieScriptLoader */
/* [unique][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieScriptLoader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("25CDB11F-CA03-4f28-99CC-88CCF694A507")
    IMagpieScriptLoader : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE HasModuleScript( 
            /* [string][in] */ const OLECHAR *lpszModuleID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetModuleScript( 
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ BSTR *pbsScript) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetModuleScriptDecorated( 
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [string][in] */ const OLECHAR *lpszScriptIntro,
            /* [string][in] */ const OLECHAR *lpszScriptExtro,
            /* [retval][out] */ BSTR *pbsScript) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieScriptLoaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieScriptLoader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieScriptLoader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieScriptLoader * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *HasModuleScript )( 
            IMagpieScriptLoader * This,
            /* [string][in] */ const OLECHAR *lpszModuleID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetModuleScript )( 
            IMagpieScriptLoader * This,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [retval][out] */ BSTR *pbsScript);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetModuleScriptDecorated )( 
            IMagpieScriptLoader * This,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [string][in] */ const OLECHAR *lpszScriptIntro,
            /* [string][in] */ const OLECHAR *lpszScriptExtro,
            /* [retval][out] */ BSTR *pbsScript);
        
        END_INTERFACE
    } IMagpieScriptLoaderVtbl;

    interface IMagpieScriptLoader
    {
        CONST_VTBL struct IMagpieScriptLoaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieScriptLoader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieScriptLoader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieScriptLoader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieScriptLoader_HasModuleScript(This,lpszModuleID)	\
    ( (This)->lpVtbl -> HasModuleScript(This,lpszModuleID) ) 

#define IMagpieScriptLoader_GetModuleScript(This,lpszModuleID,pbsScript)	\
    ( (This)->lpVtbl -> GetModuleScript(This,lpszModuleID,pbsScript) ) 

#define IMagpieScriptLoader_GetModuleScriptDecorated(This,lpszModuleID,lpszScriptIntro,lpszScriptExtro,pbsScript)	\
    ( (This)->lpVtbl -> GetModuleScriptDecorated(This,lpszModuleID,lpszScriptIntro,lpszScriptExtro,pbsScript) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieScriptLoader_INTERFACE_DEFINED__ */


#ifndef __IMagpieScriptLoader2_INTERFACE_DEFINED__
#define __IMagpieScriptLoader2_INTERFACE_DEFINED__

/* interface IMagpieScriptLoader2 */
/* [unique][uuid][object][helpstring] */ 


EXTERN_C const IID IID_IMagpieScriptLoader2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EC310E49-EF0C-4c91-8B27-FEA254803009")
    IMagpieScriptLoader2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [string][in] */ const OLECHAR *lpszPropID,
            /* [retval][out] */ BSTR *pbsRet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMagpieScriptLoader2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMagpieScriptLoader2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMagpieScriptLoader2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMagpieScriptLoader2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            IMagpieScriptLoader2 * This,
            /* [string][in] */ const OLECHAR *lpszModuleID,
            /* [string][in] */ const OLECHAR *lpszPropID,
            /* [retval][out] */ BSTR *pbsRet);
        
        END_INTERFACE
    } IMagpieScriptLoader2Vtbl;

    interface IMagpieScriptLoader2
    {
        CONST_VTBL struct IMagpieScriptLoader2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMagpieScriptLoader2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMagpieScriptLoader2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMagpieScriptLoader2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMagpieScriptLoader2_GetProperty(This,lpszModuleID,lpszPropID,pbsRet)	\
    ( (This)->lpVtbl -> GetProperty(This,lpszModuleID,lpszPropID,pbsRet) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMagpieScriptLoader2_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_MagpieApplication;

#ifdef __cplusplus

class DECLSPEC_UUID("0C9C9632-F0F9-40c2-8AA5-E49112C741A6")
MagpieApplication;
#endif
#endif /* __MagpieLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


