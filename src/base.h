#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "types.h"

#define VERSTR "v0.21"

#define BUFLEN (0x50)

struct Options {
    u32 SHOW_RAW_RESULT : 1;
    u32 PRECISE_OUTPUT  : 1;
    u32 INTERACTIVE     : 1;
};

enum CONVERSION_TYPE {
    CT_NULL,
    CONVERT_TO_F32,
    CONVERT_TO_F64,
    CONVERT_TO_H32,
    CONVERT_TO_H64,
};


struct Options OPTIONS;

enum CONVERSION_TYPE ct;
enum CONVERSION_TYPE ct_last;


void show_header(void);
void show_help(void);
