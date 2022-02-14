#include "base.h"

struct Options OPTIONS = {0};

enum CONVERSION_TYPE ct      = CT_NULL;
enum CONVERSION_TYPE ct_last = CT_NULL;

void show_header(void)
{
    printf("\n   [flot %s] Convert between IEEE754 hex and f32/f64\n", VERSTR);
}

void show_help(void)
{
    show_header();

    printf(
        "\n   Hex values are automatically padded to 8/16 characters (appended zeroes) before processing."
        "\n"
        "\n   Options:"
        "\n      -i :   Use interactive console."
        "\n"
        "\n      -a :   Autodetect and show one result."
        "\n               - If detected as hex:"
        "\n                   numbers of length <= 8 will be treated as 32b, else 64b."
        "\n               - If detected as float:"
        "\n                   use the 'f' suffix to treat as single, else treated as double."
        "\n"
        "\n      -p :   Print float values with guaranteed precision"
        "\n               - 9  sig figs printed for float"
        "\n               - 17 sig figs printed for double" 
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
