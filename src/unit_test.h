/*! @file unit_test.h
 *  @brief Unit Testing Framework
 *  @copyright Copyright (c) 2013 Kyle Weicht. All rights reserved.
 */
#ifndef __unit_test_h__
#define __unit_test_h__

typedef void (test_func_t)(void);

#define TEST(test_name) \
 	static void TEST_##test_name(void);	\
 	static test_func_t* _##test_name##_register = &TEST_##test_name; \
 	static void TEST_##test_name(void)

#define REGISTER_TEST(test_name) \
 	register_test(_##test_name##_register)

void register_test(test_func_t* func);

/*! @brief Runs all tests. Returns number of failed tests (0 for success).
 *	@return Number of failed tests. 0 for success
 */
int run_all_tests(int argc, const char* argv[]);

#endif /* include guard */
