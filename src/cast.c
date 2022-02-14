#include "base.h"
#include "cast.h"

char RESULT_F32[BUFLEN] = {0};
char RESULT_F64[BUFLEN] = {0};
char RESULT_H32[BUFLEN] = {0};
char RESULT_H64[BUFLEN] = {0};


static bool validate_str_hex(char *str, u32 len)
{
    for (u32 i = 0; i < len; i++)
    {
        char c = str[i];
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')))
            // invalid range
            return FALSE;
    }

    return TRUE;
}

static bool validate_str_flt(char *str, u32 len)
{
    u32 dotCount = 0;

    for (u32 i = 0; i < len; i++)
    {
        char c = str[i];

        if (c == '.' && ++dotCount > 1)
            // too many dots
            return FALSE;

        if (!((c >= '0' && c <= '9') || (c == '.') || (c == '-')))
            // invalid range
            return FALSE;
    }

    return TRUE;
}


bool CAST_h32_to_f32(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_hex(input, strlen(input)))
    {
        printf("FATAL: Could not parse: %s", input);
        return FALSE;
    }

    //- pad to 8
    char *p = input;

    // Go to end of string
    while (*++p != '\0');

    // Append zeroes to end
    while (strlen(input) < 8)
    {
        p[0] = '0';
        p[1] = '\0';
        p++;
    }

    u32 numh = strtoul(input, NULL, 16);
    f32 numf = *(f32 *)&numh;

    sprintf(RESULT_F32, OPTIONS.PRECISE_OUTPUT ? "%.9g" : "%lg", numf);

    return TRUE;
}

bool CAST_h64_to_f64(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_hex(input, strlen(input)))
    {
        printf("FATAL: Could not parse: %s", input);
        return FALSE;
    }

    //- pad to 16
    char *p = input;

    // Go to end of string
    while (*++p != '\0');

    // Append zeroes to end
    while (strlen(input) < 16)
    {
        p[0] = '0';
        p[1] = '\0';
        p++;
    }

    u64 numh = strtoull(input, NULL, 16);
    f64 numd = *(f64 *)&numh;

    sprintf(RESULT_F64, OPTIONS.PRECISE_OUTPUT ? "%.17lg" : "%lg", numd);

    return TRUE;
}

bool CAST_f32_to_h32(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_flt(input, strlen(input)))
    {
        printf("FATAL: Could not parse: %s", input);
        return FALSE;
    }

    f32 numf = strtof(input, NULL);
    u32 numh = *(u32 *)&numf;

    sprintf(RESULT_H32, "%lX", numh);
    return TRUE;
}

bool CAST_f64_to_h64(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_flt(input, strlen(input)))
    {
        printf("FATAL: Could not parse: %s", input);
        return FALSE;
    }

    f64 numd = strtod(input, NULL);
    u64 numh = *(u64 *)&numd;

    sprintf(RESULT_H64, "%llX", numh);
    return TRUE;
}

