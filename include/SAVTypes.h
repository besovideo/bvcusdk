#ifndef _SAV_TYPE_DEFINED
#define _SAV_TYPE_DEFINED

typedef char SAV_TYPE_INT8;
typedef unsigned char SAV_TYPE_UINT8;
typedef short SAV_TYPE_INT16;
typedef unsigned short SAV_TYPE_UINT16;
typedef int SAV_TYPE_INT32;
typedef unsigned int SAV_TYPE_UINT32;
#ifdef _MSC_VER
typedef __int64 SAV_TYPE_INT64;
typedef unsigned __int64 SAV_TYPE_UINT64;
#else
typedef long long SAV_TYPE_INT64;
typedef unsigned long long SAV_TYPE_UINT64;
#endif
typedef SAV_TYPE_UINT32 SAV_TYPE_BOOL;
#define SAV_BOOL_TRUE  1
#define SAV_BOOL_FALSE 0

typedef struct _SAV_Bit_Context {
    SAV_TYPE_UINT8* pData;
    SAV_TYPE_UINT32 iDataSize;
} SAV_Bit_Context;

#endif/*_SAV_TYPE_DEFINED*/