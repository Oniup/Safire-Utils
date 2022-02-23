#ifndef __SFR_UTILS_H__
#define __SFR_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* =========== Mandatory includes ============ */

#include <stdarg.h>

/* ================== Types ================== */

#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#endif

typedef unsigned long long  uint64_t;
typedef unsigned int        uint32_t;
typedef unsigned short      uint16_t;

typedef long long           int64_t;
typedef int                 int32_t;
typedef short               int16_t;

typedef struct              SFR_List SFR_List_t;
typedef struct              SFR_Hash SFR_Hash_t;

/* ================== String ================= */

extern char*                sfr_str(const char* _str);
extern uint32_t             sfr_strset(char* _dest, const char* _src);
extern void                 sfr_strsetlen(char* _dest, const char* _src, uint32_t _length);
extern uint32_t             sfr_strlen(const char* _src);
extern bool                 sfr_strcmp(const char* _str1, const char* _str2);
extern bool                 sfr_strcmplen(const char* _str1, const char* _str2, uint32_t _length);
extern void                 sfr_str_free(char** _str);  

/* ================== List =================== */

struct SFR_List {
    uint32_t index;
    void** data;
};

extern SFR_List_t*          sfr_list(uint32_t _count, ...);
extern void                 sfr_list_push(SFR_List_t* _list, void* _data);
extern void                 sfr_list_push_list(SFR_List_t* _list, uint32_t _count, ...);
extern void                 sfr_list_free();

/* ================== Hash =================== */


#ifdef __cplusplus
}
#endif
#endif