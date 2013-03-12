/** @file unit_test_test.cpp
 *  @copyright Copyright (c) 2013 Kyle Weicht. All rights reserved.
 */
#include "unit_test.h"

#include <stdio.h>

TEST(MakeTest)
{
}
IGNORE_TEST(FailTest)
{
    FAIL("Failure!");
}
TEST(BoolChecks)
{
    CHECK_TRUE(1);
    CHECK_FALSE(0);
}
TEST(CheckIntEqual)
{
    CHECK_EQUAL(33, 33);
    CHECK_NOT_EQUAL(-56, 56);
}
TEST(CheckIntLTGT)
{
    CHECK_LESS_THAN(458, 558);
    CHECK_GREATER_THAN(-564, -664);
    CHECK_LESS_THAN_EQUAL(458, 558);
    CHECK_LESS_THAN_EQUAL(458, 458);
    CHECK_GREATER_THAN_EQUAL(-564, -564);
    CHECK_GREATER_THAN_EQUAL(-564, -664);
}
TEST(CheckPointerEqual)
{
    int* a = (int*)0xDEADBEEF;
    int* b = (int*)0xDEADBEEF;
    int* c = (int*)0xBEEFDEAD;
    int* d = NULL;
    CHECK_EQUAL_POINTER(a, b);
    CHECK_NOT_EQUAL_POINTER(a, c);
    CHECK_NULL(d);
    CHECK_NOT_NULL(a);
}

TEST_MODULE(unit_test)
{
    REGISTER_TEST(MakeTest);
    REGISTER_TEST(FailTest);
    REGISTER_TEST(BoolChecks);
    REGISTER_TEST(CheckIntEqual);
    REGISTER_TEST(CheckIntLTGT);
    REGISTER_TEST(CheckPointerEqual);
}
