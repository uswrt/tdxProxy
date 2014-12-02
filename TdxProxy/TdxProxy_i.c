

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Dec 02 12:31:29 2014
 */
/* Compiler settings for TdxProxy.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
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

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_ITdxClient,0x7ADC239B,0x6BAB,0x4FC0,0x9D,0x17,0x65,0xE2,0x04,0x55,0xB5,0x55);


MIDL_DEFINE_GUID(IID, LIBID_TdxProxyLib,0xCCEF9E64,0x0CAB,0x45D2,0xA6,0x95,0xDB,0x40,0x95,0xC2,0x66,0x66);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0xF33259B0,0x55E2,0x43F3,0x86,0x32,0x44,0x0E,0x98,0xA3,0x50,0x20);


MIDL_DEFINE_GUID(CLSID, CLSID_TdxClient,0x5281C9E8,0xEA61,0x4EAB,0xAB,0xF2,0x9B,0xB2,0x5C,0x9B,0x69,0x98);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



