/** @file unit_test.h
 *  @brief Unit Testing Framework
 *  @copyright Copyright (c) 2013 Kyle Weicht. All rights reserved.
 */
#ifndef __unit_test_h__
#define __unit_test_h__

#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#define LUA_TESTS 1

#ifdef _MSC_VER
    #pragma warning(disable:4127) /* Conditional expression is constant (the do-while) */
#endif

typedef void (test_func_t)(void);

/** @brief Test creation macros
 */
#ifdef __cplusplus
    #define BEGIN_TESTS(name)
    #define END_TESTS

    #define TEST(test_name) \
        static void TEST_##test_name(void); \
        static int _##test_name##_register = _register_test(&TEST_##test_name); \
        static void TEST_##test_name(void)

    #define IGNORE_TEST(test_name) \
        void TEST_##test_name(void);    \
        static int _##test_name##_register = _register_test(&_ignore_test); \
        void TEST_##test_name(void)

    #define TEST_FIXTURE(fixture, test_name)                                                           \
        struct TEST_##test_name : public fixture {                                                     \
            void test(void);                                                                           \
        };                                                                                             \
        static void TEST_##fixture##_##test_name(void) {                                               \
            TEST_##test_name test;                                                                     \
            test.test();                                                                               \
        }                                                                                              \
        static int _##fixture##_##test_name##_register = _register_test(&TEST_##fixture##_##test_name); \
        void TEST_##test_name::test(void )

    #define IGNORE_TEST_FIXTURE(fixture, test_name)                                                    \
        struct TEST_##test_name : public fixture {                                                     \
            void test(void);                                                                           \
        };                                                                                             \
        static void TEST_##fixture##_##test_name(void) {                                               \
            TEST_##test_name test;                                                                     \
            test.test();                                                                               \
        }                                                                                              \
        static int _##fixture##_##test_name##_register = __ignore_test(&TEST_##fixture##_##test_name); \
        void TEST_##test_name::test(void )

    extern "C" { // Use C linkage
#else
    #define BEGIN_TESTS(name)
    #define END_TESTS

    #define TEST(test_name) \
        static void TEST_##test_name(void); \
        static test_func_t* _##test_name##_register = &TEST_##test_name; \
        static void TEST_##test_name(void)

    #define IGNORE_TEST(test_name) \
        void TEST_##test_name(void);    \
        static test_func_t* _##test_name##_register = &_ignore_test; \
        void TEST_##test_name(void)

    #define REGISTER_TEST(test_name) \
        _register_test(_##test_name##_register)

    #define TEST_MODULE(module_name)    \
        void MODULE_##module_name(void);  \
        void MODULE_##module_name(void)

    #define DECLARE_MODULE(module_name) \
        extern void MODULE_##module_name(void)

    #define REGISTER_MODULE(module_name) \
        MODULE_##module_name();
#endif

int _register_test(test_func_t* func);
void _ignore_test(void);
int __ignore_test(test_func_t* func);

/** Checking functions
 */
#define FAIL(message) \
    _fail(__FILE__, __LINE__, message)
void _fail(const char* file, int line, const char* format, ...);

/* bool */
#define CHECK_TRUE(value) \
    _check_true(__FILE__, __LINE__, (int)(value))
#define CHECK_FALSE(value) \
    _check_false(__FILE__, __LINE__, (int)(value))

void _check_true(const char* file, int line, int value);
void _check_false(const char* file, int line, int value);

/* integer */
#define CHECK_EQUAL(expected, actual) \
    _check_equal(__FILE__, __LINE__, (int64_t)expected, (int64_t)actual)
#define CHECK_NOT_EQUAL(expected, actual) \
    _check_not_equal(__FILE__, __LINE__, (int64_t)expected, (int64_t)actual)
#define CHECK_LESS_THAN(left, right) \
    _check_less_than(__FILE__, __LINE__, (int64_t)left, (int64_t)right)
#define CHECK_GREATER_THAN(left, right) \
    _check_greater_than(__FILE__, __LINE__, (int64_t)left, (int64_t)right)
#define CHECK_LESS_THAN_EQUAL(left, right) \
    _check_less_than_equal(__FILE__, __LINE__, (int64_t)left, (int64_t)right)
#define CHECK_GREATER_THAN_EQUAL(left, right) \
    _check_greater_than_equal(__FILE__, __LINE__, (int64_t)left, (int64_t)right)

void _check_equal(const char* file, int line, int64_t expected, int64_t actual);
void _check_not_equal(const char* file, int line, int64_t expected, int64_t actual);
void _check_less_than(const char* file, int line, int64_t left, int64_t right);
void _check_greater_than(const char* file, int line, int64_t left, int64_t right);
void _check_less_than_equal(const char* file, int line, int64_t left, int64_t right);
void _check_greater_than_equal(const char* file, int line, int64_t left, int64_t right);

/* pointer */
#define CHECK_EQUAL_POINTER(expected, actual) \
    _check_equal_pointer(__FILE__, __LINE__, (const void*)expected, (const void*)actual)
#define CHECK_NOT_EQUAL_POINTER(expected, actual) \
    _check_not_equal_pointer(__FILE__, __LINE__, (const void*)expected, (const void*)actual)
#define CHECK_NULL(ptr) \
    _check_null(__FILE__, __LINE__, (const void*)ptr)
#define CHECK_NOT_NULL(ptr) \
    _check_not_null(__FILE__, __LINE__, (const void*)ptr)

void _check_equal_pointer(const char* file, int line, const void* expected, const void* actual);
void _check_not_equal_pointer(const char* file, int line, const void* expected, const void* actual);
void _check_null(const char* file, int line, const void* pointer);
void _check_not_null(const char* file, int line, const void* pointer);

/* float */
#define CHECK_EQUAL_FLOAT(expected, actual) \
    _check_equal_float(__FILE__, __LINE__, (double)expected, (double)actual)
#define CHECK_EQUAL_FLOAT_EPSILON(expected, actual, epsilon) \
    _check_equal_float_epsilon(__FILE__, __LINE__, (double)expected, (double)actual, (double)epsilon)
#define CHECK_NOT_EQUAL_FLOAT(expected, actual) \
    _check_not_equal_float(__FILE__, __LINE__, (double)expected, (double)actual)
#define CHECK_LESS_THAN_FLOAT(left, right) \
    _check_less_than_float(__FILE__, __LINE__, (double)left, (double)right)
#define CHECK_GREATER_THAN_FLOAT(left, right) \
    _check_greater_than_float(__FILE__, __LINE__, (double)left, (double)right)
#define CHECK_LESS_THAN_EQUAL_FLOAT(left, right) \
    _check_less_than_equal_float(__FILE__, __LINE__, (double)left, (double)right)
#define CHECK_GREATER_THAN_EQUAL_FLOAT(left, right) \
    _check_greater_than_equal_float(__FILE__, __LINE__, (double)left, (double)right)

void _check_equal_float(const char* file, int line, double expected, double actual);
void _check_equal_float_epsilon(const char* file, int line, double expected, double actual, double epsilon);
void _check_not_equal_float(const char* file, int line, double expected, double actual);
void _check_less_than_float(const char* file, int line, double left, double right);
void _check_greater_than_float(const char* file, int line, double left, double right);
void _check_less_than_equal_float(const char* file, int line, double left, double right);
void _check_greater_than_equal_float(const char* file, int line, double left, double right);

/* string */
#define CHECK_EQUAL_STRING(expected, actual) \
    _check_equal_string(__FILE__, __LINE__, expected, actual)
#define CHECK_NOT_EQUAL_STRING(expected, actual) \
    _check_not_equal_string(__FILE__, __LINE__, expected, actual)

void _check_equal_string(const char* file, int line, const char* expected, const char* actual);
void _check_not_equal_string(const char* file, int line, const char* expected, const char* actual);


#if defined(__OBJC__) && defined(__cplusplus)
    #import <XCTest/XCTest.h>
    #undef BEGIN_TESTS
    #undef END_TESTS
    #define BEGIN_TESTS(name) \
        @interface test_##name : XCTestCase \
        @end    \
        @implementation test_##name
    #define END_TESTS \
        @end

    #undef TEST
    #undef IGNORE_TEST
    #undef TEST_FIXTURE
    #undef IGNORE_TEST_FIXTURE

    #define TEST(x) \
        - (void)test##x
    #define IGNORE_TEST(x) \
        - (void)ignoreTest##x

    #define TEST_FIXTURE(fixture, test_name)       \
        struct TEST_##test_name : public fixture { \
            id self;                               \
            void test(void);                       \
        };                                         \
        -(void) test_##fixture##_##test_name {     \
            TEST_##test_name test;                 \
            test.self = self;                      \
            test.test();                           \
        }                                          \
        void TEST_##test_name::test(void )

    #define IGNORE_TEST_FIXTURE(fixture, test_name)\
        struct TEST_##test_name : public fixture { \
            id self;                               \
            void test(void);                       \
        };                                         \
        -(void) ignore_##fixture##_##test_name {   \
            TEST_##test_name test;                 \
            test.self = self;                      \
            test.test();                           \
        }                                          \
        void TEST_##test_name::test(void )

    #undef FAIL
    #define FAIL(m) \
        XCTFail(m)

    
    #define TEST_MODULE(module_name)    \
        void MODULE_##module_name(void);  \
        void MODULE_##module_name(void)

    #define DECLARE_MODULE(module_name) \
        extern void MODULE_##module_name(void)

    #define REGISTER_MODULE(module_name) \
        test##module_name();
    
    #define REGISTER_TEST(test_name)

    /* bool */
    #undef CHECK_TRUE
    #undef CHECK_FALSE

    #define CHECK_TRUE(v) \
        XCTAssertTrue(v, @"")
    #define CHECK_FALSE(v) \
        XCTAssertFalse(v, @"")

    /* integer */
    #undef CHECK_EQUAL
    #undef CHECK_NOT_EQUAL
    #undef CHECK_LESS_THAN
    #undef CHECK_GREATER_THAN
    #undef CHECK_LESS_THAN_EQUAL
    #undef CHECK_GREATER_THAN_EQUAL

    #define CHECK_EQUAL(expected, actual) \
        XCTAssertEqual(expected, actual, @"")
    #define CHECK_NOT_EQUAL(expected, actual) \
        XCTAssertNotEqual(expected, actual, @"")
    #define CHECK_LESS_THAN(left, right) \
        CHECK_TRUE((int64_t)left < (int64_t)right)
    #define CHECK_GREATER_THAN(left, right) \
        CHECK_TRUE((int64_t)left > (int64_t)right)
    #define CHECK_LESS_THAN_EQUAL(left, right) \
        CHECK_TRUE((int64_t)left <= (int64_t)right)
    #define CHECK_GREATER_THAN_EQUAL(left, right) \
        CHECK_TRUE((int64_t)left >= (int64_t)right)

    /* pointer */
    #undef CHECK_EQUAL_POINTER
    #undef CHECK_NOT_EQUAL_POINTER
    #undef CHECK_NULL
    #undef CHECK_NOT_NULL

    #define CHECK_NOT_NULL(v) \
        XCTAssertTrue(v != NULL, @"")
    #define CHECK_NULL(v) \
        XCTAssertTrue(v == NULL, @"")
    #define CHECK_EQUAL_POINTER(expected, actual) \
        XCTAssertEqual(expected, actual, @"")
    #define CHECK_NOT_EQUAL_POINTER(expected, actual) \
        XCTAssertNotEqual(expected, actual, @"")


    /* float */
    #undef CHECK_EQUAL_FLOAT
    #undef CHECK_EQUAL_FLOAT_EPSILON
    #undef CHECK_NOT_EQUAL_FLOAT
    #undef CHECK_LESS_THAN_FLOAT
    #undef CHECK_GREATER_THAN_FLOAT
    #undef CHECK_LESS_THAN_EQUAL_FLOAT
    #undef CHECK_GREATER_THAN_EQUAL_FLOAT

    #define CHECK_LESS_THAN_FLOAT(left, right) \
        CHECK_TRUE((double)left < (double)right)
    #define CHECK_GREATER_THAN_FLOAT(left, right) \
        CHECK_TRUE((double)left > (double)right)
    #define CHECK_LESS_THAN_EQUAL_FLOAT(left, right) \
        CHECK_TRUE((double)left <= (double)right)
    #define CHECK_GREATER_THAN_EQUAL_FLOAT(left, right) \
        CHECK_TRUE((double)left >= (double)right)
    #define CHECK_EQUAL_FLOAT(expected, actual) \
        XCTAssertEqualWithAccuracy(expected, actual, 0.001, @"")
    #define CHECK_EQUAL_FLOAT_EPSILON(expected, actual, epsilon) \
        XCTAssertEqualWithAccuracy(expected, actual, epsilon, @"")
    #define CHECK_NOT_EQUAL_FLOAT(expected, actual) \
        XCTAssertNotEqualWithAccuracy(expected, actual, 0.001, @"")


    /* string */
    #undef CHECK_EQUAL_STRING
    #undef CHECK_NOT_EQUAL_STRING

    #define CHECK_EQUAL_STRING(expected, actual) \
        CHECK_TRUE(strcmp(expected, actual) == 0)
    #define CHECK_NOT_EQUAL_STRING(expected, actual) \
        CHECK_FALSE(strcmp(expected, actual) == 0)


#endif /* __OBJC2__ */

/** @brief Runs all tests. Returns number of failed tests (0 for success).
 *  @return Number of failed tests. 0 for success
 */
int run_all_tests(int argc, const char* argv[]);

#define RUN_ALL_TESTS(argc, argv, test_arg, register_func) \
    do {                                            \
        int _ii;                                    \
        for(_ii=0;_ii<argc;++_ii)                   \
            if(strcmp(argv[_ii], test_arg) == 0) {  \
                test_func_t* _reg = register_func;  \
                if(_reg)                            \
                    _reg();                         \
                return run_all_tests(argc, argv);   \
            }                                       \
    } while(__LINE__ == -1)

#ifdef __cplusplus
    } // extern "C" {
#endif

#endif /* include guard */
