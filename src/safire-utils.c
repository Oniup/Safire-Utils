#include "safire-utils.h" // change to work with your directory paths

// string functionality

char* sfr_str(const char* _str) {
    uint32_t length = sfr_strlen(_str);
    char* str = (char*)malloc(sizeof(char*) * length);
    memcpy(str, _str, length);
    str[length] = '\0';
    return str;
}

char* sfr_stralloc(uint32_t _length) {
    uint32_t length = sfr_strlen(_str);
    char* str = (char*)malloc(sizeof(char*) * length);
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

// config functionality

