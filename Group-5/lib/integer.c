#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool my_isdigit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isnumber(const char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    // Handle optional sign
    if (*str == '+' || *str == '-') {
        ++str;
    }

    bool has_digit = false;
    while (*str != '\0') {
        if (!my_isdigit(*str)) {
            return false;
        }
        has_digit = true;
        ++str;
    }

    return has_digit;
}
