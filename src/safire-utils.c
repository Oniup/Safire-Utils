#include "safire-utils.h" // change to work with your directory paths

// string functionality

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

// config functionality

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

// random number generators functionality

int sfr_rand_int32() {
   // https://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
   int r = rand() & 0xff;
   r |= (rand() & 0xff) << 8;
   r |= (rand() & 0xff) << 16;
   r |= (rand() & 0xff) << 24;
   return r;
}

uint32_t sfr_rand_uint32() {
   uint32_t r = (unsigned)rand() & 0xff;
   r |= ((unsigned)rand() & 0xff) << 8;
   r |= ((unsigned)rand() & 0xff) << 16;
   r |= ((unsigned)rand() & 0xff) << 24;
   return r;
}

// https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
#define IMAX_BITS(m) ((m)/((m)%255+1) / 255%255*8 + 7-86/((m)%255+12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)
assert((RAND_MAX & (RAND_MAX + 1u)) == 0, "RAND_MAX not a Mersenne number");
uint64_t sfr_rand_uint64() {
   uint64_t r = 0;
   for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
      r <<= RAND_MAX_WIDTH;
      r ^= (unsigned)rand();
   }
   return r;
}
