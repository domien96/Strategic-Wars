#ifndef MIN_UNIT
#define MIN_UNIT

/* based on min_unit */
/* original source: http://www.jera.com/techinfo/jtns/jtn002.html */

#include <stdlib.h>
#include <stdio.h> 

#ifndef _WIN32
#define mu_assert(test) do { if (!(test)) return mu_assert_message(__func__, __FILE__, __LINE__, #test, NULL); } while (0)
#else
#define mu_assert(test) do { if (!(test)) return mu_assert_message(__FUNCTION__, __FILE__, __LINE__, #test, NULL); } while (0)
#endif
#define mu_assert_msg(message, test) do { if (!(test)) return mu_assert_message(__func__, __FILE__, __LINE__, #test, message); } while (0)
#define mu_run_test(test) do { char *message = test(); mu_tests_run++; \
                                     if (message) return message; else printf("   Test \"%s\" PASS\n", #test); } while (0)
extern int mu_tests_run;

char* mu_assert_message(const char* func, const char* file, int line, const char* code, const char* message);

#endif /*MIN_UNIT*/
