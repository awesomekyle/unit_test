#include <stdio.h>
#include "unit_test.h"

int main(int argc, const char* argv[])
{
    int ii;
    for(ii=0;ii<argc;++ii) {
        printf("%s\n", argv[ii]);
    }
    return run_all_tests(argc, argv);
}

