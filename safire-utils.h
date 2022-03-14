/* Safire Utils 0.07

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
      
Note, if you want to include all the utilities but not just a couple, then add the desired macro to not include:
   SAFIRE_UTILS_NO_STRING_IMPLEMENTATION
   SAFIRE_UTILS_NO_LIST_IMPLEMENTATION
   SAFIRE_UTILS_NO_HASH_IMPLEMENTATION
   SAFIRE_UTILS_NO_CONFIG_IMPLEMENTATION


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
#  define SAFIRE_UTILS_RANDOM_IMPLEMENTATION
#  define SAFIRE_UTILS_CONFIG_IMPLEMENTATION
#  if !defined(SAFIRE_UTILS_IMPLEMENTATION_EXTERN)
#   define SAFIRE_UTILS_INLINE
#   define SAFIRE_DEF static inline
#  else 
#   define SAFIRE_DEF extern
#  endif
#endif

#ifdef SAFIRE_UTILS_STD_INCLUDES
#  ifndef SAFIRE_NO_STDIO
#   include <stdio.h>
#  elif
#   define SAFIRE_UTILS_NO_CONFIG_IMPLEMENTATION
#  endif
#  if !defined(SAFIRE_NO_ASSERT) || !defined(NDEBUG)
#   include <assert.h>
#   define SAFIRE_ASSERT(_x, _message) assert(_x && _message)
#   define SAFIRE_ASSERT2(_x, _y, _message) assert(_x && _y && _message)
#   define SAFIRE_ASSERT3(_x, _y, _z, _message) assert(_x && _y && _z && _message)
#   define SAFIRE_ASSERT4(_x, _y, _z, _w, _message) assert(_x && _y && _z && _w &&_message)
#  else
#   define SAFIRE_ASSERT(_x, _message)
#   define SAFIRE_ASSERT2(_x, _y, _message)
#   define SAFIRE_ASSERT3(_x, _y, _z, _message)
#   define SAFIRE_ASSERT4(_x, _y, _z, _w, _message)
#  endif
#  if defined(SAFIRE_UTILS_RANDOM_IMPLEMENTATION)
#   ifndef SAFIRE_UTILS_NO_RANDOM_IMPLEMENTATION
#     include <time.h>
#   endif
#  endif 
#  include <stdlib.h>
#  include <memory.h>
#  include <stdint.h>
#  include <stdbool.h>
#endif // SAFIRE_UTILS_STD_INCLUDES

#if defined(SAFIRE_UTILS_LIST_IMPLEMENTATION) 
#ifndef SAFIRE_UTILS_NO_LIST_IMPLEMENTATION
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
 * @brief resizes the buffer and pushes the element to the array without increaseing the size parameter to the correct size
 * @param[in] _Ty type of the data
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer
 * @param[in] _push (_Ty) the data that is going to be pushed to the list
*/
#define SFR_LIST_push_no_increase(_Ty, _list, _size, _push) {\
   SAFIRE_ASSERT(_push, "failed to push element to list as the element doesn't have any memory allocaed");\
   if (_list == NULL) {\
      _list = SFR_LIST_create(_Ty, _size + 1);\
   } else {\
      _list = (_Ty*)realloc(_list, sizeof(_Ty) * _size + 1);\
      SAFIRE_ASSERT(_list, "failed to re-allocate memory to list");\
   }\
   _list[_size] = _push;\
}\

/*!
 * @brief resizes the buffer with uninitalized memory
 * @param[in] _Ty type of the data
 * @param[in] _list (_Ty*) the buffer that is storing the list
 * @param[in] _size (unsigned int/int) the size of the buffer 
*/
#define SFR_LIST_resize(_Ty, _list, _size) {\
   SAFIRE_ASSERT(_list, "failed to resize buffer as the list is set to NULL");\
   SAFIRE_ASSERT(_size > 0, "failed to resize buffer as the list's size is <= 0");\
   _list = (_Ty*)realloc(_list, sizeof(_Ty) * _size);\
   SAFIRE_ASSERT(_list, "failed to resize buffer");\
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

#endif // SAFIRE_UTILS_NO_LIST_IMPLEMENTATION

#if defined(SAFIRE_UTILS_HASH_IMPLEMENTATION) 
#ifndef SAFIRE_UTILS_NO_HASH_IMPLEMENTATION

// TODO: implement hash maps 0.20

// #define SFR_HASH_create(_Tyi, _Ty, _size) {\
//    \
// }\

#endif // SAFIRE_UTILS_NO_HASH_IMPLEMENTATION
#endif // SAFIRE_UTILS_HASH_IMPLEMENTATION

#if defined(SAFIRE_UTILS_STRING_IMPLEMENTATION) 
#ifndef SAFIRE_UTILS_NO_STRING_IMPLEMENTATION

#define SFR_MAX_STACK_LINE_LENGTH 1024
#define SFR_MAX_STACK_CHAR_BUFFER_LENGTH 100000
/*!
 * @brief allocates memory and copies over the _str to the new string
 * @param[in] _str desired string to be copied into new string
 * @returns (char*) a new c style string
*/
SAFIRE_DEF char* sfr_str(const char* _str);
/*!
 * @brief allocates uninitialised memory to the char* buffer
 * @param[in] _length the desired length that will be allocated
 * @returns (char*) a new c style string of uninitialised memory
*/
SAFIRE_DEF char* sfr_str_alloc(uint32_t _length);
/*!
 * @brief allocates and copies the desired string to the destination string
 * @param[in] _src string to be copied into the destired string
 * @param[in] _dest the desired string that will have _src message copied over to
 * @returns (uint32_t) the length of the string that was copied over to the _dest
*/
SAFIRE_DEF uint32_t sfr_str_set(char* _dest, const char* _src);
/*!
 * @brief allocates and copies the desired string to the destination string
 * @param[in] _src string to be copied into the destired string
 * @param[in] _dest the desired string that will have _src message copied over to
 * @param[in] _length the length of the _src string, optimisation over 'sfr_str_set' if known length
*/
SAFIRE_DEF void sfr_str_set_length(char* _dest, const char* _src, uint32_t _length);
/*!
 * @brief returns the length of a string
 * @param[in] _src desired string to find the length of
 * @returns (uint32_t ) the length of the string
*/
SAFIRE_DEF uint32_t sfr_str_length(const char* _src);
/*!
 * @brief returns true if the two strings are the same
 * @param[in] _str1 the string that will check if same as _str2
 * @param[in] _str2 the string that will check if same as _str1
 * @returns (bool) true if the two strings are the same
*/
SAFIRE_DEF bool sfr_str_cmp(const char* _str1, const char* _str2);
/*!
 * @brief returns true if the two strings are the same
 * @param[in] _str1 the string that will check if same as _str2
 * @param[in] _str2 the string that will check if same as _str1
 * @param[in] _length the length of both strings, optimisation over 'str_strcmp' if known lengths are the same
 * @returns (bool) true if the two strings are the same
*/
SAFIRE_DEF bool sfr_str_cmp_length(const char* _str1, const char* _str2, uint32_t _length);
/*!
 * @brief frees the strings memory and sets the strings pointer to NULL
 * @param[in] _src desired string to be freed
*/
SAFIRE_DEF void sfr_str_free(char** _str);

#if defined(SAFIRE_UTILS_INLINE)

char* sfr_str(const char* _str) {
   uint32_t length = sfr_str_length(_str);
   char* str = (char*)malloc(sizeof(char*) * length);
   memcpy(str, _str, length);
   str[length] = '\0';
   return str;
}

char* sfr_str_alloc(uint32_t _length) {
   char* str = (char*)malloc(sizeof(char*) * _length);
   str[_length] = '\0';
   return str;
}

uint32_t sfr_str_set(char* _dest, const char* _src) {
   if (_dest != NULL) {
      sfr_str_free(&_dest);
   }
   uint32_t length = sfr_str_length(_src);
   _dest = (char*)malloc(sizeof(char*) * length);
   memcpy(_dest, _src, length);
   _dest[length] = '\0';
   return length;
}

void sfr_str_set_length(char* _dest, const char* _src, uint32_t _length) {
   if (_dest != NULL) {
      sfr_str_free(&_dest);
   }
   _dest = (char*)malloc(sizeof(char*) * _length);
   memcpy(_dest, _src, _length);
   _dest[_length] = '\0';
}

uint32_t sfr_str_length(const char* _src) {
   uint32_t length = 0;
   while (_src[length] != '\0') { 
      length++;
   }
   return length;
}

bool sfr_str_cmp(const char* _str1, const char* _str2) {
   uint32_t i = 0;
   while (i < SFR_MAX_STACK_CHAR_BUFFER_LENGTH) {
      if (_str1[i] == '\0' && _str2[i] == '\0') {
         return true;
      } else if(_str1[i] != _str2[i]) {
         return false;
      }
      i++;
   }
   return false;
}

bool sfr_str_cmp_length(const char* _str1, const char* _str2, uint32_t _length) {
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

#endif //SAFIRE_UTILS_INLINE
#endif // SAFIRE_UTILS_NO_STRING_IMPLEMENTATION
#endif // SAFIRE_UTILS_STRING_IMPLEMENTATION

#if defined(SAFIRE_UTILS_CONFIG_IMPLEMENTATION) 
#if defined(SAFIRE_UTILS_STRING_IMPLEMENTATION)
#ifndef SAFIRE_UTILS_NO_CONFIG_IMPLEMENTATION

typedef struct SFR_config_settings SFR_config_settings_t;
typedef struct SFR_config_data SFR_config_data_t;
typedef struct SFR_config SFR_config_t;

struct SFR_config_settings {
   bool use_names;
};

struct SFR_config_data {
   char* name;
   char* value;
};

struct SFR_config {
   SFR_config_settings_t* settings;
   char* path;

   SFR_config_data_t* data;
   uint32_t size;
};

/*!
 * @brief reads the config file and pushes the data collected to an array of strings in the config data structure
 * @param[in] _config the data structure that will store the data collected from the config file
 * @param[in] _path path to the config file
*/
SAFIRE_DEF void sfr_config_read(SFR_config_t* _config, const char* _path);
/*!
 * @brief converts the config (char* format) at the desire index to a float
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _index index that you want to convert
 * @returns (float) the value that is being converted
*/
SAFIRE_DEF float sfr_config_convert_float(SFR_config_t* _config, uint32_t _index);\
/*!
 * @brief converts the config (char* format) at the desire index to an int
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _index index that you want to convert
 * @returns (int) the value that is being converted
*/
SAFIRE_DEF int sfr_config_convert_int32(SFR_config_t* _config, uint32_t _index);
/*!
 * @brief converts the config (char* format) at the desire index to a 64 bit int
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _index index that you want to convert
 * @returns (long long) the value that is being converted
*/
SAFIRE_DEF long long sfr_config_convert_int64(SFR_config_t* _config, uint32_t _index);
/*!
 * @brief converts the config (char* format) at the desire index to an unsigned int 
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _index index that you want to convert
 * @returns (unsigned int) the value that is being converted
*/
SAFIRE_DEF uint32_t sfr_config_convert_uint32(SFR_config_t* _config, uint32_t _index);
/*!
 * @brief converts the config (char* format) at the desire index to an unsigned long long
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _index index that you want to convert
 * @returns (unsigned long long) the value that is being converted
*/
SAFIRE_DEF uint64_t sfr_config_convert_uint64(SFR_config_t* _config, uint32_t _index);
/*!
 * @brief converts the config (char* format) at the desire index to a float
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _element name of the element that is going to be converted
 * @returns (float) the value that is being converted
*/
SAFIRE_DEF float sfr_config_convert_name_float(SFR_config_t* _config, const char* _element);
/*!
 * @brief converts the config (char* format) at the desire index to an int
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _element name of the element that is going to be converted
 * @returns (int) the value that is being converted
*/
SAFIRE_DEF int sfr_config_convert_name_int32(SFR_config_t* _config, const char* _element);
/*!
 * @brief converts the config (char* format) at the desire index to a long long
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _element name of the element that is going to be converted
 * @returns (long long) the value that is being converted
*/
SAFIRE_DEF long long sfr_config_convert_name_int64(SFR_config_t* _config, const char* _element);
/*!
 * @brief converts the config (char* format) at the desire index to an unsigned long long
 * @param[in] _config the config data collected through the str_config_read() function
 * @param[in] _element name of the element that is going to be converted
 * @returns (unsigned long long) the value that is being converted
*/
SAFIRE_DEF uint32_t sfr_config_convert_name_uint32(SFR_config_t* _config, const char* _element);
/*!
 * @brief converts the config (char* format) at the desire index to an unsigned int
 * @param[in] _config the config data's collected through the str_config_read() function
 * @param[in] _element name of the element that is going to be converted
 * @returns (unsigned int) the value that is being converted
*/
SAFIRE_DEF uint64_t sfr_config_convert_name_uint64(SFR_config_t* _config, const char* _element);
/*!
 * @brief frees the string data stored in the config data structure
 * @param[in] _config desired config data structure to be freed
*/
SAFIRE_DEF void sfr_config_free(SFR_config_t* _config);
/*!
 * @brief prints the data to the console
 * @param[in] _config desired config data structure to be freed
*/
SAFIRE_DEF void sfr_config_print(SFR_config_t* _config);

#if defined(SAFIRE_UTILS_INLINE)

void sfr_config_read(SFR_config_t* _config, const char* _path) {
   SAFIRE_ASSERT(_config, "failed to read source from the config file as the config data structure is set to NULL");
   SAFIRE_ASSERT(_config, "failed to read source from the config file as the path given is set to NULL");

   // config settings
   SFR_config_settings_t settings = {};
   settings.use_names = true;

   // freeing the data if needed
   if (_config->path != NULL) {
      sfr_str_free(&_config->path);
   }
   if (_config->data != NULL) {
      if (_config->settings != NULL) {
         settings.use_names = _config->settings->use_names;
      } 
      for (uint32_t i = 0; i < _config->size; i++) {
         if (settings.use_names) {
            sfr_str_free(&_config->data[i].name);
         }
         sfr_str_free(&_config->data[i].value);
      }
      free(_config->data);
      _config->data = NULL;
      _config->size = 0;
   }

   // getting the source
   FILE* file = fopen(_path, "r");
   char line[SFR_MAX_STACK_LINE_LENGTH];
   while (fgets(line, SFR_MAX_STACK_LINE_LENGTH, file)) {
      uint32_t line_length = sfr_str_length(line);
      bool next_line = false;
      char push[SFR_MAX_STACK_LINE_LENGTH];
      uint32_t j = 0;
      
      // setting location storage
      uint32_t k = _config->size;
      uint32_t dest = settings.use_names ? 0 : 1;
      _config->size++;
      if (k > 0) {
         SFR_LIST_resize(SFR_config_data_t, _config->data, _config->size);
      } else {
         _config->data = SFR_LIST_create(SFR_config_data_t, _config->size);
      }
      _config->data[k].name = NULL;
      _config->data[k].value = NULL;

      // parse line
      for (uint32_t i = 0; i < line_length; i++) {         
         switch (line[i]) {
         // pushing the line's data
         case '\0': {
            if (j > 0) {
               push[j] = '\0';
               _config->data[k].value = sfr_str(push);
            } else {
               if (dest > 0) {
                  sfr_str_free(&_config->data[k].name);
               }
               SFR_LIST_pop(SFR_config_data_t, _config->data, _config->size);
            }
            next_line = true;
            break;
         }
         case '\n': {
            if (j > 0) {
               push[j] = '\0';
               _config->data[k].value = sfr_str(push);
            } else {
               if (dest > 0) {
                  sfr_str_free(&_config->data[k].name);
               }
               SFR_LIST_pop(SFR_config_data_t, _config->data, _config->size);
            }
            next_line = true;
            break;
         }
         // pushing the data's 'name' and setting dest to 'value' 
         case ':': {
            if (settings.use_names) {
               SAFIRE_ASSERT(dest == 0, "failed to parse line for some reason");
            }
            if (j > 0) {
               push[j] = '\0';
               _config->data[k].name = sfr_str(push);
            }
            dest++;
            j = 0;
         }
         // skips
         case '\r': {
            continue;
         }
         case ' ': {
            continue;
         }
         // comments
         case '/': {
            SAFIRE_ASSERT(line[i + 1] == '/', "failed to parse line - invalid syntax");
            // checking if the name has already been pushed
            if (dest > 0) {
               if (j > 0) {
                  push[j] = '\0';
                  _config->data[k].value = sfr_str(push);
               }
            } else {
               // don't add this data to the config if name has not been pushed
               if (_config->data[k].name != NULL) {
                  sfr_str_free(&_config->data[k].name);
               }
               if (_config->data[k].value != NULL) {
                  sfr_str_free(&_config->data[k].value);
               }
               SFR_LIST_pop(SFR_config_data_t, _config->data, _config->size);
            }
            next_line = true;
            break;
         }
         }

         if (next_line) {
            break;
         } else {
            push[j] = line[i];
            j++;
         }
      }

      memset(line, '\0', SFR_MAX_STACK_LINE_LENGTH);
   }
   
}

float sfr_config_convert_float(SFR_config_t* _config, uint32_t _index) {
   SAFIRE_ASSERT(_config, "failed to convert data to float as the config data structure is set to null");
   SAFIRE_ASSERT(_config->data, "failed to convert data to float as the config data is set to null");
   SAFIRE_ASSERT(_index < _config->size, "failed to convert data to float as the index provided is greater than the config size");
   return (float)atof(_config->data[_index].value);
}

int sfr_config_convert_int32(SFR_config_t* _config, uint32_t _index) {
   SAFIRE_ASSERT(_config, "failed to convert data to int32 as the config data structure is set to null");
   SAFIRE_ASSERT(_config->data, "failed to convert data to int32 as the config data is set to null");
   SAFIRE_ASSERT(_index < _config->size, "failed to convert data to int32 as the index provided is greater than the config size");
   return (int)atoi(_config->data[_index].value);
}

long long sfr_config_convert_int64(SFR_config_t* _config, uint32_t _index) {
   SAFIRE_ASSERT(_config, "failed to convert data to int64 as the config data structure is set to null");
   SAFIRE_ASSERT(_config->data, "failed to convert data to int64 as the config data is set to null");
   SAFIRE_ASSERT(_index < _config->size, "failed to convert data to int64 as the index provided is greater than the config size");
   return (long long)atoi(_config->data[_index].value);
}

uint32_t sfr_config_convert_uint32(SFR_config_t* _config, uint32_t _index) {
   SAFIRE_ASSERT(_config, "failed to convert data to unsigned int32 as the config data structure is set to null");
   SAFIRE_ASSERT(_config->data, "failed to convert data to unsigned int32 as the config data is set to null");
   SAFIRE_ASSERT(_index < _config->size, "failed to convert data to unsigned int32 as the index provided is greater than the config size");
   return (uint32_t)atoi(_config->data[_index].value);
}

uint64_t sfr_config_convert_uint64(SFR_config_t* _config, uint32_t _index) {
   SAFIRE_ASSERT(_config, "failed to convert data to unsigned int64 as the config data structure is set to null");
   SAFIRE_ASSERT(_config->data, "failed to convert data to unsigned int64 as the config data is set to null");
   SAFIRE_ASSERT(_index < _config->size, "failed to convert data to unsigned int64 as the index provided is greater than the config size");
   return (uint64_t)atoi(_config->data[_index].value);
}

float sfr_config_convert_name_float(SFR_config_t* _config, const char* _element) {
   SAFIRE_ASSERT(_config, "failed to convert data to float as the config data structure is set to null");
   for (uint32_t i = 0; i < _config->size; i++) {
      if (sfr_str_cmp(_config->data[i].name, _element)) {
         return sfr_config_convert_float(_config, i);
      }
   }
   SAFIRE_ASSERT(!_element, "failed to find element in the config stack");
   return 0.0f;
}

int sfr_config_convert_name_int32(SFR_config_t* _config, const char* _element) {
   SAFIRE_ASSERT(_config, "failed to convert data to int32 as the config data structure is set to null");
   for (uint32_t i = 0; i < _config->size; i++) {
      if (sfr_str_cmp(_config->data[i].name, _element)) {
         return sfr_config_convert_int32(_config, i);
      }
   }
   SAFIRE_ASSERT(!_element, "failed to find element in the config stack");
   return 0;
}

long long sfr_config_convert_name_int64(SFR_config_t* _config, const char* _element) {
   SAFIRE_ASSERT(_config, "failed to convert data to int64 as the config data structure is set to null");
   for (uint32_t i = 0; i < _config->size; i++) {
      if (sfr_str_cmp(_config->data[i].name, _element)) {
         return sfr_config_convert_int64(_config, i);
      }
   }
   SAFIRE_ASSERT(!_element, "failed to find element in the config stack");
   return 0;
}

uint32_t sfr_config_convert_name_uint32(SFR_config_t* _config, const char* _element) {
   SAFIRE_ASSERT(_config, "failed to convert data to unsigned int32 as the config data structure is set to null");
   for (uint32_t i = 0; i < _config->size; i++) {
      if (sfr_str_cmp(_config->data[i].name, _element)) {
         return sfr_config_convert_uint32(_config, i);
      }
   }
   SAFIRE_ASSERT(!_element, "failed to find element in the config stack");
   return 0;
}

uint64_t sfr_config_convert_name_uint64(SFR_config_t* _config, const char* _element) {
   SAFIRE_ASSERT(_config, "failed to convert data to unsigned int64 as the config data structure is set to null");
   for (uint32_t i = 0; i < _config->size; i++) {
      if (sfr_str_cmp(_config->data[i].name, _element)) {
         return sfr_config_convert_uint64(_config, i);
      }
   }
   SAFIRE_ASSERT(!_element, "failed to find element in the config stack");
   return 0;
}

void sfr_config_free(SFR_config_t* _config) {
   SAFIRE_ASSERT(_config, "failed to free config data structure");
   if (_config->path != NULL) {
      sfr_str_free(&_config->path);
   }
   if (_config->data != NULL) {
      bool free_names = true;
      if (_config->settings != NULL) {
         free_names = _config->settings->use_names;
      } 
      for (uint32_t i = 0; i < _config->size; i++) {
         if (free_names) {
            sfr_str_free(&_config->data[i].name);
         }
         sfr_str_free(&_config->data[i].value);
      }
      free(_config->data);
      _config->data = NULL;
      _config->size = 0;
   }
}

void sfr_config_print(SFR_config_t* _config) {
   for (uint32_t i = 0; i < _config->size; i++) {
      printf("name: %s, value: %s\n", _config->data[i].name, _config->data[i].value);
   }
}

#endif // SAFIRE_UTILS_INLINE
#endif // SAFIRE_UTILS_NO_CONFIG_IMPLEMENTATION
#endif // SAFIRE_UTILS_STRING_IMPLEMENTATION
#endif // SAFIRE_UTILS_CONFIG_IMPLEMENTATION

#if defined(SAFIRE_UTILS_RANDOM_IMPLEMENTATION)
#ifndef SAFIRE_UTILS_NO_RANDOM_IMPLEMENTATION

SAFIRE_DEF int sfr_rand_int32();
SAFIRE_DEF long long sfr_rand_int64();
SAFIRE_DEF uint32_t sfr_rand_uint32();
SAFIRE_DEF uint64_t sfr_rand_uint64();

#if defined(SAFIRE_UTILS_INLINE)

int sfr_rand_int32() {
   time_t t1; 
   srand ((unsigned)time(&t1));
   // https://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
   int r = rand() & 0xff;
   r |= (rand() & 0xff) << 8;
   r |= (rand() & 0xff) << 16;
   r |= (rand() & 0xff) << 24;
   return r;
}

uint32_t sfr_rand_uint32() {
   time_t t1; 
   srand ((unsigned)time(&t1));
   uint32_t r = (unsigned)rand() & 0xff;
   r |= ((unsigned)rand() & 0xff) << 8;
   r |= ((unsigned)rand() & 0xff) << 16;
   r |= ((unsigned)rand() & 0xff) << 24;
   return r;
}

// https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
#define IMAX_BITS(m) ((m)/((m)%255+1) / 255%255*8 + 7-86/((m)%255+12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)
uint64_t sfr_rand_uint64() {
   time_t t1; 
   srand ((unsigned)time(&t1));
   uint64_t r = 0;
   for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
      r <<= RAND_MAX_WIDTH;
      r ^= (unsigned)rand();
   }
   return r;
}

#endif // SAFIRE_UTILS_INLINE
#endif // SAFIRE_UTILS_NO_RANDOM_IMPLEMENTATION
#endif // SAFIRE_UTILS_RANDOM_IMPLEMENTATION

#endif // SAFIRE_UTILS_HAS_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#endif // __SFR_UTILS_H__

/*
   version history
      0.07   (13-03-2022) added random number generator functions
                          added the number generator functions to the source code
                          formated the tabs spaces to be 3 spaces instead of 4 so there is more code on the screen
      0.06   (02-03-2022) fixed bug when the config parsed a line with only a \n where it would create an empty element and not remove it
                      updated README.md
                      added the config's optional extern functions instead of using the static inline functions (in .c)
      0.05   (01-03-2022) finished the config data structure
                      added the config string to number functions (char* -> float, int, uint32_t, ...)
                      moved the static generic functions to be above all the static inline/extern functions
      0.04   (26-02-2022) added more assert macros
                      improved the config macros a little by adding functionality to include everything but remove the ones you don't want
                      added and started config file reader utility (this is a early design, and is likely to be reworked later) ...
                      added a source version for all the static inline functions, (this doesn't include the static generic functions such as the list and hash map)
      0.03   (24-02-2022) removed implementation of uint32_t and replaced with the proper c includes
                      added internal documentation for the string functions
      0.02   (23-02-2022) added inline string functionality
                      added functions for lists
                      added config through macros
                      added some internal documentation at the beggining of the file
      0.01   (23-02-2022) git repo init
*/
