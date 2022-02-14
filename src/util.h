#pragma once

#include "types.h"

struct TokenInfo {
    char **ptrBuf;
    s32    ptrCnt;

    bool   err;
};


struct TokenInfo *split_tokens        (char *input);
void              free_tokens         (struct TokenInfo *tokenInfo);

s32               sanitise_strings    (char **vals, s32 valc);
void              toupper_str         (char *str);