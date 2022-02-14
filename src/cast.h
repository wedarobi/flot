#pragma once

#include "base.h"

char RESULT_F32[BUFLEN];
char RESULT_F64[BUFLEN];
char RESULT_H32[BUFLEN];
char RESULT_H64[BUFLEN];

bool CAST_h32_to_f32(char *buf);
bool CAST_h64_to_f64(char *buf);
bool CAST_f32_to_h32(char *buf);
bool CAST_f64_to_h64(char *buf);

