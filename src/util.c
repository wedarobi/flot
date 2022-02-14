#include "base.h"
#include "util.h"


struct TokenInfo *split_tokens(char *input)
{
    struct TokenInfo *tokenInfo = malloc(sizeof(struct TokenInfo));

    char **ptrBuf = NULL;
    s32    ptrCnt = 0;

    char *delim = " ";

    // We use strtok twice, so make a copy
    char *inputCpy = malloc(strlen(input) + 1);

    if (tokenInfo == NULL || inputCpy == NULL)
        //! bad malloc, get out
        return NULL;

    strcpy(inputCpy, input);

    /**
     * Count number of values to parse
     * 
     * Run strtok over the input first to know how many
     * string pointers to malloc
     */
    {
        char *tok = strtok(input, delim);

        while (tok != NULL)
        {
            ptrCnt++;

            tok = strtok(NULL, delim);
        }
    }

    /**
     * Alloc and copy words
     * 
     * Store all token pointers in the buffer
     */
    {
        ptrBuf = malloc(ptrCnt * sizeof(char *));

        // Track curr idx through the buffer
        s32 currPtrIdx = 0;

        // Get initial token
        char *tok = strtok(inputCpy, delim);

        while (tok != NULL)
        {
            // Alloc and store copy of token to ptr buffer

            ptrBuf[currPtrIdx] = malloc(strlen(tok) + 1);

            if (ptrBuf[currPtrIdx] == NULL)
                //! bad malloc, get out
                return NULL;

            strcpy(ptrBuf[currPtrIdx], tok);

            currPtrIdx++;

            // Try get next token
            tok = strtok(NULL, delim);
        }
    }

    tokenInfo->ptrBuf = ptrBuf;
    tokenInfo->ptrCnt = ptrCnt;
    tokenInfo->err    = FALSE;

    free(inputCpy);

    return tokenInfo;
}


void free_tokens(struct TokenInfo *tokenInfo)
{
    if (tokenInfo->err)
        // Bad status, don't free
        return;

    for (s32 i = 0; i < tokenInfo->ptrCnt; i++)
        if (tokenInfo->ptrBuf[i] != NULL)
            free(tokenInfo->ptrBuf[i]);

    free(tokenInfo->ptrBuf);
    tokenInfo->ptrBuf = NULL;
}

s32 sanitise_strings(char **vals, s32 valc)
{
    for (s32 i = 0; i < valc; i++)
    {
        char **str = &vals[i];

        // Check for hex prefix
        if (strstr(*str, "0x") != NULL)
        {
            // Has hex prefix
            if (strlen(*str) > 2)
            {
                // skip the prefix
                *str += 2;
            }
            else
            {
                // Error: only prefix "0x" is present, no number
                printf("ERR: Invalid number passed (run without args for usage): %s\n", *str);

                return 1;
            }
        }

        // Trim
        {
            char *s = *str;

            while (*s)
            {
                if (*s == '\r' || *s == '\n')
                {
                    *s = '\0';
                    break;
                }

                s++;
            }
        }
    }

    return 0;
}

void toupper_str(char *str)
{
    while (*str)
        *str++ = toupper(*str);
}

