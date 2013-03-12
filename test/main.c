#include <stdio.h>
#include "unit_test.h"



int main(int argc, const char* argv[])
{
    extern void REGISTER_unit_test(void);
    REGISTER_unit_test();
    RUN_ALL_TESTS(argc, argv, "-t");

    return 0;
}

