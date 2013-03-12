/*! @file unit_test.c
 *  @copyright Copyright (c) 2013 Kyle Weicht. All rights reserved.
 */
#include "unit_test.h"

#include <stdlib.h>

/* Constants
 */
enum {MAX_TESTS = 4096};

/* Variables
 */
static test_func_t* _test_funcs[MAX_TESTS] = {NULL};
static int _num_tests = 0;

/* Internal functions
 */

/* External functions
 */
void register_test(test_func_t* func)
{
	_test_funcs[_num_tests] = func;
	_num_tests++;
}

int run_all_tests(int argc, const char* argv[])
{
	return 0;
	(void)sizeof(argc);
	(void)sizeof(*argv);
}
