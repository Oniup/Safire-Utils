/* Safire Utils 0.03

To use the library, define this before including the file or 
as a preprocessor:
    #define SAFIRE_UTILS_IMPLEMENTATION

Note: if you want to use the external functions definitions, then
    #define SAFIRE_UTILS_IMPLEMENTATION_EXTERN

your source code should look something like this:
    #include ".../"
    #include ".../"
    #include ".../"
    #include ".../"
    #define SAFIRE_UTILS_IMPLEMENTATION
    #include ".../safire-utils.h"

Note, if you don't want to include everything, you can customize 
what to include through the following macros:

    safire kind of needs these, so if not included you'll have to edit this file:
        SAFIRE_UTILS_STD_INCLUDES
            - includes all the std libraries that this uses

    safire isn't dependent on these so it won't break the library to not include
        SAFIRE_UTILS_STRING_IMPLEMENTATION
            - custom c string functions
        SAFIRE_UTILS_LIST_IMPLEMENTATION
        SAFIRE_UTILS_HASH_IMPLEMENTATION
        
        SAFIRE_NO_ASSERT
            - removes the assert functionality when compiling in debug mode

        SAFIRE_UTILS_INLINE
            - TODO: uses static inline functions, if not included, then use external function definition


*/

#ifndef __SFR_UTILS_H__
#define __SFR_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SAFIRE_UTILS_IMPLEMENTATION) || defined(SAFIRE_UTILS_IMPLEMENTATION_EXTERN)
#  define SAFIRE_UTILS_STD_INCLUDES
#  define SAFIRE_UTILS_STRING_IMPLEMENTATION
#  define SAFIRE_UTILS_LIST_IMPLEMENTATION
#  define SAFIRE_UTILS_HASH_IMPLEMENTATION
#  if !defined(SAFIRE_UTILS_IMPLEMENTATION_EXTERN)
#    define SAFIRE_UTILS_INLINE
#    define SAFIRE_DEF static inline
#  else 
#    define SAFIRE_DEF extern
#  endif
#endif

#ifdef SAFIRE_UTILS_STD_INCLUDES
#  ifndef SAFIRE_NO_STDIO
#    include <stdio.h>
#  endif
#  ifndef SAFIRE_NO_ASSERT
#    ifndef NDEBUG
#      include <assert.h>
#      define SAFIRE_ASSERT(_x, _message) assert(_x && _message)
#    endif
#  else
#    define SAFIRE_ASSERT(_x, _message)
#  endif
#  include <stdlib.h>
#  include <memory.h>
#  include <stdint.h>
#  include <stdbool.h>
#endif // SAFIRE_UTILS_STD_INCLUDES

#if defined(SAFIRE_UTILS_STRING_IMPLEMENTATION) 
#  if !defined(SAFIRE_UTILS_IMPLEMENTATION) || !defined(SAFIRE_UTILS_IMPLEMENTATION_EXTERN)
#    if defined(SAFIRE_UTILS_INLINE)
#      define SAFIRE_DEF static inline 
#    else 
#      define SAFIRE_DEF extern
#    endif
#  endif

/*!
 * @brief allocates memory and copies over the _str to the new string
 * @param[in] _str desired string to be copied into new string
 * @returns (char*) a new c style string
*/
SAFIRE_DEF char* sfr_str(const char* _str);
/*!
 * @brief allocates and copies the desired string to the destination string
 * @param[in] _src string to be copied into the destired string
 * @param[in] _dest the desired string that will have _src message copied over to
 * @returns (uint32_t) the length of the string that was copied over to the _dest
*/
SAFIRE_DEF uint32_t sfr_strset(char* _dest, const char* _src);
/*!
 * @brief allocates and copies the desired string to the destination string
 * @param[in] _src string to be copied into the destired string
 * @param[in] _dest the desired string that will have _src message copied over to
 * @param[in] _length the length of the _src string, optimisation over 'sfr_strset' if known length
*/
SAFIRE_DEF void sfr_strsetlen(char* _dest, const char* _src, uint32_t _length);
/*!
 * @brief returns the length of a string
 * @param[in] _src desired string to find the length of
 * @returns (uint32_t ) the length of the string
*/
SAFIRE_DEF uint32_t sfr_strlen(const char* _src);
/*!
 * @brief returns true if the two strings are the same
 * @param[in] _str1 the string that will check if same as _str2
 * @param[in] _str2 the string that will check if same as _str1
 * @returns (bool) true if the two strings are the same
*/
SAFIRE_DEF bool sfr_strcmp(const char* _str1, const char* _str2);
/*!
 * @brief returns true if the two strings are the same
 * @param[in] _str1 the string that will check if same as _str2
 * @param[in] _str2 the string that will check if same as _str1
 * @param[in] _length the length of both strings, optimisation over 'str_strcmp' if known lengths are the same
 * @returns (bool) true if the two strings are the same
*/
SAFIRE_DEF bool sfr_strcmplen(const char* _str1, const char* _str2, uint32_t _length);
/*!
 * @brief frees the strings memory and sets the strings pointer to NULL
 * @param[in] _src desired string to be freed
*/
SAFIRE_DEF void sfr_str_free(char** _str);

#  if defined(SAFIRE_UTILS_INLINE)

char* sfr_str(const char* _str) {
    uint32_t length = sfr_strlen(_str);
    char* str = (char*)malloc(sizeof(char*) * length);
    memcpy(str, _str, length);
    str[length] = '\0';
    return str;
}

uint32_t sfr_strset(char* _dest, const char* _src) {
    if (_dest != NULL) {
        sfr_str_free(&_dest);
    }
    uint32_t length = sfr_strlen(_src);
    _dest = (char*)malloc(sizeof(char*) * length);
    memcpy(_dest, _src, length);
    _dest[length] = '\0';
    return length;
}

void sfr_strsetlen(char* _dest, const char* _src, uint32_t _length) {
    if (_dest != NULL) {
        sfr_str_free(&_dest);
    }
    _dest = (char*)malloc(sizeof(char*) * _length);
    memcpy(_dest, _src, _length);
    _dest[_length] = '\0';
}

uint32_t sfr_strlen(const char* _src) {
    uint32_t length = 0;
    while (_src[length] != '\0') { 
        length++;
    }
    return length;
}

bool sfr_strcmp(const char* _str1, const char* _str2) {
    uint32_t i = 0;
    while (i < 100000) {
        if (_str1[i] == '\0' && _str2[i] == '\0') {
            return true;
        } else if(_str1[i] != _str2[i]) {
            return false;
        }
        i++;
    }
    return false;
}

bool sfr_strcmplen(const char* _str1, const char* _str2, uint32_t _length) {
    if (!(_str1[_length] == '\0' && _str2[_length] == '\0')) {
        return false;
    }
    for (uint32_t i = 0; i < _length; i++) {
        if (_str1[i] != _str2[i]) {
            return false;
        }
    }
    return true;
}

void sfr_str_free(char** _str) {
    SAFIRE_ASSERT(*_str, "failed to terminate string as the _str doesn't have any memory assigned to it");
    free(*_str);
    *_str = NULL;
}

#  endif //SAFIRE_UTILS_INLINE
#endif

#if defined(SAFIRE_UTILS_LIST_IMPLEMENTATION) 
/*!
 * @brief allocates un-initialised memory to a buffer (list)
 * @param[in] _Ty type of the data
 * @param[in] _size (unsigned int/int) the size of the buffer
*/
#define SFR_LIST_create(_Ty, _size) ({\
    _Ty* list = (_Ty*)malloc(sizeof(_Ty) * _size);\
    SAFIRE_ASSERT(list, "failed to allocate memory to list");\
    list;\
})\

/*!
 * @brief resizes the buffer and pushes the element to the array
 * @param[in] _Ty type of the data
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer
 * @param[in] _push (_Ty) the data that is going to be pushed to the list
*/
#define SFR_LIST_copy(_Ty, _list_dest, _size_dest, _list_src, _size_src) {\
    SAFIRE_ASSERT(_list_src, "failed to erase element from list as the list doesn't have any memory allocated");\
    SAFIRE_ASSERT(_size_src > 0,"failed to erase element from list as the size is set to 0");\
    if (_list_dest != NULL) {\
        free(_list_dest);\
    }\
    _list_dest = (_Ty*)malloc(sizeof(_Ty) * _size_src);\
    memcpy(_list_dest, _list_src, sizeof(_Ty) * _size_src);\
    _size_dest = _size_src;\
}\

/*!
 * @brief resizes the buffer and pushes the element to the array
 * @param[in] _Ty type of the data
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer
 * @param[in] _push (_Ty) the data that is going to be pushed to the list
*/
#define SFR_LIST_push(_Ty, _list, _size, _push) {\
    SAFIRE_ASSERT(_push, "failed to push element to list as the element doesn't have any memory allocaed");\
    if (_list == NULL) {\
        _size++;\
        _list = SFR_LIST_create(_Ty, _size);\
    } else {\
        _size++;\
        _list = (_Ty*)realloc(_list, sizeof(_Ty) * _size);\
        SAFIRE_ASSERT(_list, "failed to re-allocate memory to list");\
    }\
    _list[_size - 1] = _push;\
}\

/*!
 * @brief removes the last element from the list
 * @param[in] _Ty type of the data
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer
*/
#define SFR_LIST_pop(_Ty, _list, _size) {\
    SAFIRE_ASSERT(_list, "failed to erase element from list as the list doesn't have any memory allocated");\
    SAFIRE_ASSERT(_size > 0, "failed to erase element from list as the size is set to 0");\
    _size = _size - 1;\
    _Ty* temp = (_Ty*)malloc(sizeof(_Ty) * _size);\
    memcpy(temp, _list, sizeof(_Ty) * _size);\
    free(_list);\
    _list = (_Ty*)malloc(sizeof(_Ty) * _size);\
    memcpy(_list, temp, sizeof(_Ty) * _size);\
}\

/*!
 * @brief erases (removes) an element at a desired position from the buffer
 * @param[in] _Ty type of the data
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer
 * @param[in] _target (_Ty) the index of the target element to be erased from the buffer
*/
#define SFR_LIST_erase(_Ty, _list, _size, _target) {\
    SAFIRE_ASSERT(_target < _size, "failed to erase element from list as the target is greater than the count");\
    SAFIRE_ASSERT(_list, "failed to erase element from list as the list doesn't have any memory allocated");\
    SAFIRE_ASSERT(_size > 0, "failed to erase element from list as the size is set to 0");\
    _size = _size - 1;\
    uint32_t temp1_size = _target;\
    uint32_t temp2_size = _size - _target;\
    _Ty* temp1 = (_Ty*)malloc(sizeof(_Ty) * temp1_size);\
    _Ty* temp2 = (_Ty*)malloc(sizeof(_Ty) * temp2_size);\
    memcpy(temp1, _list, sizeof(_Ty) * temp1_size);\
    memcpy(temp2, _list + (_target + 1), sizeof(_Ty) * temp2_size);\
    free(_list);\
    _list = (_Ty*)malloc(sizeof(_Ty) * _size);\
    memcpy(_list, temp1, sizeof(_Ty) * temp1_size);\
    memcpy(_list + temp1_size, temp2, sizeof(_Ty) * temp2_size);\
}\

/*!
 * @brief frees the buffer
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer
*/
#define SFR_LIST_free(_list, _size) {\
    free(_list);\
    _list = NULL;\
    _size = 0;\
}\

#endif // SAFIRE_UTILS_HAS_IMPLEMENTATION

#if defined(SAFIRE_UTILS_HASH_IMPLEMENTATION) 

// TODO: implement hash maps

// #define SFR_HASH_create(_Tyi, _Ty, _size) {\
//     \
// }\

#endif // SAFIRE_UTILS_HASH_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#endif // __SFR_UTILS_H__

/*
    version history
        0.03    (24-02-2022) removed implementation of uint32_t and replaced with the proper c includes
                             added internal documentation for the string functions
        0.02    (23-02-2022) added inline string functionality
                             added functions for lists
                             added config through macros
                             added some internal documentation at the beggining of the file
        0.01    (23-02-2022) git repo init
*/
