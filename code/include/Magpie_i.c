

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IMagpieApplication,0xEB490C81,0x7F44,0x42a7,0x8D,0x45,0x02,0xEF,0xA8,0xB8,0x27,0x7E);


MIDL_DEFINE_GUID(IID, LIBID_MagpieLib,0x927FEA3E,0x303E,0x44d3,0x96,0x5F,0x95,0x36,0x0C,0x16,0xBA,0x45);


MIDL_DEFINE_GUID(IID, DIID__IMagpieLoggerEvents,0xC9FC6B1C,0x26FD,0x4af7,0x8F,0x5F,0x49,0x7A,0x7E,0xA4,0xFD,0x89);


MIDL_DEFINE_GUID(IID, IID_IMagpieObjectCreator,0x04E73040,0x64B5,0x4624,0xBA,0x53,0xF0,0x66,0x31,0x37,0xD3,0x2B);


MIDL_DEFINE_GUID(IID, IID_IMagpieConsole,0x5FCD00DD,0xF021,0x496a,0x9F,0xD1,0x24,0xB7,0x19,0x5F,0xF5,0xD9);


MIDL_DEFINE_GUID(IID, IID_IMagpieModuleRestricted,0x77CA8F88,0x66B2,0x4bdf,0x87,0x3F,0x8C,0x33,0xBD,0x81,0x4E,0xF5);


MIDL_DEFINE_GUID(IID, IID_IMagpieModule,0xCE0BBC10,0xB7B6,0x476f,0xA7,0x99,0xA5,0x9A,0x41,0x05,0xC3,0xBC);


MIDL_DEFINE_GUID(IID, IID_IMagpieRequire,0x82440242,0x4B02,0x4690,0xB2,0xA0,0x17,0x1C,0xA2,0x06,0x13,0x6A);


MIDL_DEFINE_GUID(IID, IID_IMagpieScriptLoader,0x25CDB11F,0xCA03,0x4f28,0x99,0xCC,0x88,0xCC,0xF6,0x94,0xA5,0x07);


MIDL_DEFINE_GUID(IID, IID_IMagpieScriptLoader2,0xEC310E49,0xEF0C,0x4c91,0x8B,0x27,0xFE,0xA2,0x54,0x80,0x30,0x09);


MIDL_DEFINE_GUID(CLSID, CLSID_MagpieApplication,0x0C9C9632,0xF0F9,0x40c2,0x8A,0xA5,0xE4,0x91,0x12,0xC7,0x41,0xA6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



