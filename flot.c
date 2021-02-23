#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "types.h"

static void toupper_str(char *str)
{
    while (*str)
        *str++ = toupper(*str);
}

typedef enum CONVERSION_TYPE {
    CT_NULL,
    CONVERT_TO_F32,
    CONVERT_TO_F64,
    CONVERT_TO_H32,
    CONVERT_TO_H64,
} CONVERSION_TYPE;

CONVERSION_TYPE ct = CT_NULL;

static struct {
    u32 SHOW_RAW_RESULT : 1;
} OPTIONS = {0};

char RESULT_F32[0x50] = {0};
char RESULT_F64[0x50] = {0};
char RESULT_H32[0x50] = {0};
char RESULT_H64[0x50] = {0};

#define BUFLEN (0x50)

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
        if (c == '.')
        {
            if (++dotCount > 1)
            {
                // too many dots
                return FALSE;
            }
        }

        if (!((c >= '0' && c <= '9') || (c == '.')))
        {
            // invalid range
            return FALSE;
        }
    }
    return TRUE;
}

static bool do_cast_for_h32_to_f32(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_hex(input, strlen(input)))
    {
        printf("FATAL: Could not parse!");
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

    u32 numh = strtol(input, NULL, 16);
    f32 numf = *(f32 *)&numh;

    sprintf(RESULT_F32, "%g", numf);
    return TRUE;
}

static bool do_cast_for_h64_to_f64(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_hex(input, strlen(input)))
    {
        printf("FATAL: Could not parse!");
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

    u64 numh = strtoll(input, NULL, 16);
    f64 numd = *(f64 *)&numh;

    sprintf(RESULT_F64, "%lg", numd);
    return TRUE;
}

static bool do_cast_for_f32_to_h32(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_flt(input, strlen(input)))
    {
        printf("FATAL: Could not parse!");
        return FALSE;
    }

    f32 numf = strtof(input, NULL);
    u32 numh = *(u32 *)&numf;

    sprintf(RESULT_H32, "%lX", numh);
    return TRUE;
}

static bool do_cast_for_f64_to_h64(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (!validate_str_flt(input, strlen(input)))
    {
        printf("FATAL: Could not parse!");
        return FALSE;
    }

    f64 numd = strtod(input, NULL);
    u64 numh = *(u64 *)&numd;

    sprintf(RESULT_H64, "%llX", numh);
    return TRUE;
}

#define INPUT(string) (strstr(input, string))

static s32 run(char *buf)
{
    char input[BUFLEN] = {0};
    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    if (FALSE);

    //- Determine conversion type
    else if (INPUT("."))
    {
        if (INPUT("f"))
        {
            // float convert to hex 32
            ct = CONVERT_TO_H32;
            // remove "f", set to end of string
            *INPUT("f") = '\0';
        }
        else
        {
            // double, convert to hex 64
            ct = CONVERT_TO_H64;
        }
    }
    else
    {
        // Treat as hex
        if (strlen(input) <= 8)
            // hex 32, convert to f32
            ct = CONVERT_TO_F32;
        else if (strlen(input) <= 16)
            // hex 64, convert to f64
            ct = CONVERT_TO_F64;
        else
        {
            // Invalid input
            printf("ERR: Invalid input (too long)!");
            return 1;
        }
        
    }

    //- Run conversions

    if (OPTIONS.SHOW_RAW_RESULT)
    {
        switch (ct)
        {
            case CONVERT_TO_F32:
            {
                toupper_str(input);
                if (!do_cast_for_h32_to_f32(input))
                    return 1;

                if (!strstr(RESULT_F32, "NAN"))
                    printf("%s\n", RESULT_F32);
                else
                {
                    printf("ERR: Could not convert!");
                    return 1;
                }
                break;
            }
            case CONVERT_TO_F64:
            {
                toupper_str(input);
                if (!do_cast_for_h64_to_f64(input))
                    return 1;

                if (!strstr(RESULT_F64, "NAN"))
                    printf("%s\n", RESULT_F64);
                else
                {
                    printf("ERR: Could not convert!");
                    return 1;
                }

                break;
            }
            case CONVERT_TO_H32:
            {
                if (!do_cast_for_f32_to_h32(input))
                    return 1;

                if (!strstr(RESULT_H32, "NAN"))
                    printf("%s\n", RESULT_H32);
                else
                {
                    printf("ERR: Could not convert!");
                    return 1;
                }
                break;
            }
            case CONVERT_TO_H64:
            {
                if (!do_cast_for_f64_to_h64(input))
                    return 1;

                if (!strstr(RESULT_H64, "NAN"))
                    printf("%s\n", RESULT_H64);
                else
                {
                    printf("ERR: Could not convert!");
                    return 1;
                }

                break;
            }

            case CT_NULL:
                printf("ERR: Invalid input!");
                return 1;
        }
    }
    else
    {
        switch (ct)
        {
            case CONVERT_TO_F32:
            case CONVERT_TO_F64:
            {
                toupper_str(input);
                if (!do_cast_for_h32_to_f32(input))
                    return 1;
                if (!do_cast_for_h64_to_f64(input))
                    return 1;

                if (!strstr(RESULT_F32, "NAN"))
                    printf("[f32] %s\n", RESULT_F32);
                if (!strstr(RESULT_F64, "NAN"))
                    printf("[f64] %s\n", RESULT_F64);

                break;
            }
            case CONVERT_TO_H32:
            case CONVERT_TO_H64:
                if (!do_cast_for_f32_to_h32(input))
                    return 1;
                if (!do_cast_for_f64_to_h64(input))
                    return 1;

                if (!strstr(RESULT_H32, "NAN"))
                    printf("[h32] %s\n", RESULT_H32);
                if (!strstr(RESULT_H64, "NAN"))
                    printf("[h64] %s\n", RESULT_H64);

                break;

            case CT_NULL:
                printf("ERR: Invalid input!");
                return 1;
        }
    }

    return 0;
}

static void show_help(void)
{
    printf(
        "\n   - Convert between IEEE754 hex and f32/f64 -"
        "\n   - wed"
        "\n"
        "\n   Hex values are automatically padded to 8/16 characters (appended zeroes) before processing."
        "\n"
        "\n   Options:"
        "\n      -a :   Autodetect and show one result."
        "\n               - If detected as hex:"
        "\n                   numbers of length <= 8 will be treated as 32b, else 64b."
        "\n               - If detected as float:"
        "\n                   use the 'f' suffix to treat as single, else treated as double."
        "\n"
        "\n   Examples:  flot 3f8"
        "\n              flot 0x4200000000000000"
        "\n              flot 1.2"
        "\n              flot -a 3f8         (treated as 32b)"
        "\n              flot -a 4014000000  (treated as 64b)"
        "\n              flot -a 3.5f        (treated as single)"
        "\n              flot -a 1.2         (treated as double)"
        "\n"
    );
}

#define ARG(string) (!strcmp(arg, string))

int main(int argc, char *argv[])
{
    u32 n = argc;

    //! takes the last passed argument only
    char *arg = argv[argc - 1];

    //- Checks

    if (argc <= 1)
    {
        show_help();
        return 0;
    }

    //# Check for hex prefix
    if (strstr(arg, "0x") != NULL)
    {
        // Has hex prefix
        if (strlen(arg) > 2)
        {
            // skip the prefix
            arg += 2;
        }
        else
        {
            // Error: only prefix "0x" is present, no number
            printf("ERR: Invalid number passed.\n");
            show_help();
            return 1;
        }
    }

    //- Process flags

    while (--n)
    {
        char *arg = argv[n];

        if (FALSE) ;

        else if (ARG("-a") || ARG("/a"))
            OPTIONS.SHOW_RAW_RESULT = TRUE;
    }

    return run(arg);
}


