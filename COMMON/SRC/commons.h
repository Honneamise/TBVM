#ifndef COMMONS_H
#define COMMONS_H

//generic defines
#define LEN_32 32
#define LEN_64 64
#define LEN_128 128
#define MAX_LEN 0xFF

//generic types
typedef unsigned char Uint8;
typedef unsigned short Uint16;

//debug
#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRT_SECURE_NO_WARNINGS
#include "crtdbg.h"
#endif

//std
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "ctype.h"

//core
#include "funcs.h"
#include "ins.h"

#endif