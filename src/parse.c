#include "base.h"
#include "parse.h"

#include "cast.h"
#include "util.h"

#define INPUT(string) (strstr(input, string))

s32 PARSE_one(char *buf)
{
    char input[BUFLEN] = {0};

    strcpy(input, buf);
    input[BUFLEN - 1] = '\0';

    bool newLine = !OPTIONS.SHOW_RAW_RESULT;

    char *delim = newLine ? "\n" : " ";

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
        {
            // hex 32, convert to f32
            ct = CONVERT_TO_F32;
        }
        else if (strlen(input) <= 16)
        {
            // hex 64, convert to f64
            ct = CONVERT_TO_F64;
        }
        else
        {
            // Invalid input
            printf("ERR: Invalid input (too long)!");
            return 1;
        }
    }

    // We add a prefix if the type is new/changed, and if printing on the same line
    bool addPrfx = !newLine && ct != ct_last;

    //- Run conversions

    if (OPTIONS.SHOW_RAW_RESULT)
    {
        switch (ct)
        {
            case CONVERT_TO_F32:
            {
                toupper_str(input);

                if (!CAST_h32_to_f32(input))
                    return 1;

                if (addPrfx)
                    printf("[f32] ");

                if (!strstr(RESULT_F32, "NAN"))
                {
                    printf("%s%s", RESULT_F32, delim);
                }
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

                if (!CAST_h64_to_f64(input))
                    return 1;

                if (addPrfx)
                    printf("[f64] ");

                if (!strstr(RESULT_F64, "NAN"))
                {
                    printf("%s%s", RESULT_F64, delim);
                }
                else
                {
                    printf("ERR: Could not convert!");
                    return 1;
                }

                break;
            }
            case CONVERT_TO_H32:
            {
                if (!CAST_f32_to_h32(input))
                    return 1;

                if (addPrfx)
                    printf("[h32] ");

                if (!strstr(RESULT_H32, "NAN"))
                {
                    printf("%s%s", RESULT_H32, delim);
                }
                else
                {
                    printf("ERR: Could not convert!");
                    return 1;
                }

                break;
            }
            case CONVERT_TO_H64:
            {
                if (!CAST_f64_to_h64(input))
                    return 1;

                if (addPrfx)
                    printf("[h64] ");

                if (!strstr(RESULT_H64, "NAN"))
                {
                    printf("%s%s", RESULT_H64, delim);
                }
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

                if (!CAST_h32_to_f32(input))
                    return 1;
                if (!CAST_h64_to_f64(input))
                    return 1;

                if (!strstr(RESULT_F32, "NAN"))
                    printf("[f32] %s%s", RESULT_F32, delim);
                if (!strstr(RESULT_F64, "NAN"))
                    printf("[f64] %s%s", RESULT_F64, delim);

                break;
            }
            case CONVERT_TO_H32:
            case CONVERT_TO_H64:
                if (!CAST_f32_to_h32(input))
                    return 1;
                if (!CAST_f64_to_h64(input))
                    return 1;

                if (!strstr(RESULT_H32, "NAN"))
                    printf("[h32] %s%s", RESULT_H32, delim);
                if (!strstr(RESULT_H64, "NAN"))
                    printf("[h64] %s%s", RESULT_H64, delim);

                break;

            case CT_NULL:
                printf("ERR: Invalid input!");
                return 1;
        }
    }

    ct_last = ct;

    return 0;
}

s32 PARSE_all(char **vals, s32 valc)
{
    ct_last = CT_NULL;

    for (s32 i = 0; i < valc; i++)
    {
        char *str = vals[i];

        s32 res;
        if (res = PARSE_one(str))
            return res;
    }

    return 0;
}

s32 PARSE_interactive(void)
{
    show_header();

    while (TRUE)
    {
        printf("\n>>> ");

        char   *input = NULL;
        size_t  len   = 0;
        size_t  _     = getline(&input, &len, stdin);

        struct TokenInfo *tokenInfo = split_tokens(input);

        if (tokenInfo == NULL)
            //!
            exit(EXIT_FAILURE);

        if (!sanitise_strings(tokenInfo->ptrBuf, tokenInfo->ptrCnt));
            PARSE_all(tokenInfo->ptrBuf, tokenInfo->ptrCnt);

        //- Cleanup
        free_tokens(tokenInfo);
        free(input);
    }
}
