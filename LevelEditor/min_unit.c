#include "min_unit.h"

#define MIN_UNIT_MAX_MSG 10240

int mu_tests_run = 0;

static char message_string[MIN_UNIT_MAX_MSG+1];

char* mu_assert_message(const char* func, const char* file, int line, const char* code, const char* message)
{
    if (message == NULL)
        snprintf(message_string, MIN_UNIT_MAX_MSG, "   Test \"%s\" FAIL\n       function: \"%s\"\n           code: \"%s\"\n           file: %s\n           line: %d\n", func, func, code, file, line);
    else
        snprintf(message_string, MIN_UNIT_MAX_MSG, "   Test \"%s\" FAIL\n       function: \"%s\"\n           code: \"%s\"\n           file: %s\n           line: %d\n            msg: \"%s\"", func, func, code, file, line, message);

    message_string[MIN_UNIT_MAX_MSG] = '\0';

    return message_string;
}

