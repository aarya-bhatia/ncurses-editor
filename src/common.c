#include "common.h"

int char_type(char c)
{
    if (isalnum(c)) {
        return C_ALPHANUM;
    } else if (isspace(c)) {
        return C_WHITESPACE;
    } else if (c > 0 && c < 32) {
        return C_CONTROL;
    } else if ((c > ' ' && c < '0') || (c > 'Z' && c < 'a') || (c > 'z')) {
        return C_SPECIAL;
    }

    return C_UNDEFINED;
}
