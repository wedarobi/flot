#include "base.h"
#include "flot.h"

#include "util.h"
#include "parse.h"

#define ARG(string)   (!strcmp(arg, string))

int main(int argc, char *argv[])
{
    //- Alloc

    /* Store all pointers for values to convert */
    char **vals = malloc(argc * sizeof(char *));
    /* Number of values to convert */
    s32    valc = 0;


    //- Checks #1

    if (argc <= 1)
    {
        show_help();

        return 0;
    }


    //- Process args

    for (u32 n = 1; n < argc; n++)
    {
        char *arg = argv[n];

        if (FALSE);

        else if (ARG("-a") || ARG("/a"))
            OPTIONS.SHOW_RAW_RESULT = TRUE;

        else if (ARG("-p") || ARG("/p"))
            OPTIONS.PRECISE_OUTPUT = TRUE;

        else if (ARG("-i") || ARG("/i"))
            OPTIONS.INTERACTIVE = TRUE;

        else
            //- Store str ptr
            vals[valc++] = arg;
    }

    if (OPTIONS.INTERACTIVE)
    {
        /* Interactive console */
        return PARSE_interactive();
    }
    else
    {
        /* Non-interactive */

        //- Checks #2

        int res;

        if (res = sanitise_strings(vals, valc))
            return res;


        //- Run

        res = PARSE_all(vals, valc);

        free(vals);

        return res;
    }
}


