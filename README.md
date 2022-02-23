# Safire Utils (C) 

## Contents

* [About the Project](#about)
* [Project Status](#project-status)
* [Known Issues](#known-issues)
* [Setup](#setup)

## 🔥 About

Safire Utils is a small, lightweight, header only (options for using src) utils library written in C that implements functionality for useful data structures such as lists (basically std::vector), hash maps, better string functionality and more. The Goal is to keep the library as lightweight, have no dependencies, and be as fast as possible while also making your life easier when programming when dealing with dynamic arrays.

## 📚 Documentation 

*for more information, check out the html docs: (coming soon...)*

To use the library, define this before including the file or as a preprocessor:
```c
#define SAFIRE_UTILS_IMPLEMENTATION
```
*Note:* if you want to use the external functions definitions, then
```c
#define SAFIRE_UTILS_IMPLEMENTATION_EXTERN
```
your source code should look something like this:
```c
#include ".../"
#include ".../"
#include ".../"
#include ".../"
#define SAFIRE_UTILS_IMPLEMENTATION
#include ".../safire-utils.h"
```

*Note*, if you don't want to include everything, you can customize what to include through the following macros:

safire kind of needs these, so if not included you'll have to edit this file:
```c
#define SAFIRE_UTILS_STD_INCLUDES 
// includes all the std libraries that this uses
#define SAFIRE_UTILS_TYPES_IMPLEMENTATION 
// includes all the custom types
```
safire isn't dependent on these so it won't break the library to not include
```c
#define SAFIRE_UTILS_STRING_IMPLEMENTATION
#define SAFIRE_UTILS_LIST_IMPLEMENTATION
#define SAFIRE_UTILS_HASH_IMPLEMENTATION
#define SAFIRE_NO_ASSERT
// uses static inline functions, if not included, then use external function definition
#define SAFIRE_UTILS_INLINE
```

## 🔨 Project Status

**safire utils is currently in a really early stage...**

## 🐜 Known Issues

*currently not aware of any*

## 📦 Build and Setup

There is no dependencies needed, just slap it in your code and your good to go :)

## 📃 License 

I haven't been bothered to write a license yet, but in short, you can do whatever you want with it, however if you do use it, make sure to reference me by mentioning this repo somewhere. Thats all for now :D