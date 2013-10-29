/** @file unit_test.c
 *  @copyright Copyright (c) 2013 Kyle Weicht. All rights reserved.
 */
#include "unit_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <dirent.h>
#include <math.h>
#if LUA_TESTS
    #ifdef __cplusplus
        #include <lua.hpp>
    #else
        #include <lua.h>
        #include <lualib.h>
        #include <lauxlib.h>
    #endif
#endif /* LUA_TESTS */
#ifndef _WIN32
    #include <unistd.h>
    #include <inttypes.h>
#else
    #include <direct.h>
    #define snprintf sprintf_s
    #define getcwd _getcwd
#endif

#ifndef PRId64
    #define PRId64 "ld"
#endif

#if defined(__APPLE__) || defined(__GNUG__)
    #define ERROR_FORMAT "%s:%d: error: "
#else
    #define ERROR_FORMAT "%s(%d): error: "
#endif

/* Constants
 */
enum {MAX_TESTS = 4096};
static const float EPSILON = 0.00001f;

typedef enum {
    kResultPass,
    kResultFail,
    kResultIgnore
} test_result_t;

/* Variables
 */
static test_func_t* _test_funcs[MAX_TESTS] = {NULL};
static int  _num_tests = 0;
static int  _num_tests_passed = 0;
static int  _num_tests_failed = 0;
static int  _num_tests_ignored = 0;
static test_result_t _current_result = kResultPass;

/* Internal functions
 */
#if LUA_TESTS
static struct lua_State*    _L = NULL;
static char _current_lua_test_file[1024];
static const char* _get_ext(const char* filename)
{
    const char* end = filename + strlen(filename);
    while(end && *end != '.' && end > filename)
        end--;
    if(end == filename)
        return NULL;
    if(*end == '.')
        end++;
    return end;
}
static int _lua_line(lua_State* L)
{
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "nSl", &ar);
    return ar.currentline;
}
static int _FAIL_l(lua_State* L)
{
    _fail(_current_lua_test_file, _lua_line(L), lua_tostring(L, 1));
    return 0;
}

static int _check_true_l(lua_State* L)
{
    _check_true(_current_lua_test_file, _lua_line(L), lua_toboolean(L, 1));
    return 0;
}
static int _check_false_l(lua_State* L)
{
    _check_false(_current_lua_test_file, _lua_line(L), lua_toboolean(L, 1));
    return 0;
}

static int _check_equal_l(lua_State* L)
{
    _check_equal(_current_lua_test_file, _lua_line(L), lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}
static int _check_not_equal_l(lua_State* L)
{
    _check_not_equal(_current_lua_test_file, _lua_line(L), lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}
static int _check_less_than_l(lua_State* L)
{
    _check_less_than(_current_lua_test_file, _lua_line(L), lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}
static int _check_greater_than_l(lua_State* L)
{
    _check_greater_than(_current_lua_test_file, _lua_line(L), lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}
static int _check_less_than_equal_l(lua_State* L)
{
    _check_less_than_equal(_current_lua_test_file, _lua_line(L), lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}
static int _check_greater_than_equal_l(lua_State* L)
{
    _check_greater_than_equal(_current_lua_test_file, _lua_line(L), lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}

static int _check_null_l(lua_State* L)
{
    _check_null(_current_lua_test_file, _lua_line(L), lua_topointer(L, 1));
    return 0;
}
static int _check_not_null_l(lua_State* L)
{
    _check_not_null(_current_lua_test_file, _lua_line(L), lua_topointer(L, 1));
    return 0;
}
static int _check_equal_pointer_l(lua_State* L)
{
    _check_equal_pointer(_current_lua_test_file, _lua_line(L), lua_topointer(L, 1), lua_topointer(L, 2));
    return 0;
}
static int _check_not_equal_pointer_l(lua_State* L)
{
    _check_not_equal_pointer(_current_lua_test_file, _lua_line(L), lua_topointer(L, 1), lua_topointer(L, 2));
    return 0;
}

static int _check_equal_float_l(lua_State* L)
{
    _check_equal_float(_current_lua_test_file, _lua_line(L), lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
}
static int _check_not_equal_float_l(lua_State* L)
{
    _check_not_equal_float(_current_lua_test_file, _lua_line(L), lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
}
static int _check_less_than_float_l(lua_State* L)
{
    _check_less_than_float(_current_lua_test_file, _lua_line(L), lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
}
static int _check_greater_than_float_l(lua_State* L)
{
    _check_greater_than_float(_current_lua_test_file, _lua_line(L), lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
}
static int _check_less_than_equal_float_l(lua_State* L)
{
    _check_less_than_equal_float(_current_lua_test_file, _lua_line(L), lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
}
static int _check_greater_than_equal_float_l(lua_State* L)
{
    _check_greater_than_equal_float(_current_lua_test_file, _lua_line(L), lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
}

static int _check_equal_string_l(lua_State* L)
{
    _check_equal_string(_current_lua_test_file, _lua_line(L), lua_tolstring(L, (1), ((void*)0)), lua_tolstring(L, (2), ((void*)0)));
    return 0;
}
static int _check_not_equal_string_l(lua_State* L)
{
    _check_not_equal_string(_current_lua_test_file, _lua_line(L), lua_tolstring(L, (1), ((void*)0)), lua_tolstring(L, (2), ((void*)0)));
    return 0;
}
static luaL_Reg _lua_test_methods[] = {
    { "FAIL", _FAIL_l },
    { "CHECK_TRUE", _check_true_l },
    { "CHECK_FALSE", _check_false_l },

    { "CHECK_EQUAL", _check_equal_l },
    { "CHECK_NOT_EQUAL", _check_not_equal_l },
    { "CHECK_LESS_THAN", _check_less_than_l },
    { "CHECK_GREATER_THAN", _check_greater_than_l },
    { "CHECK_LESS_THAN_EQUAL", _check_less_than_equal_l },
    { "CHECK_GREATER_THAN_EQUAL", _check_greater_than_equal_l },

    { "CHECK_NULL", _check_null_l },
    { "CHECK_NOT_NULL", _check_not_null_l },
    { "CHECK_EQUAL_POINTER", _check_equal_pointer_l },
    { "CHECK_NOT_EQUAL_POINTER", _check_not_equal_pointer_l },

    { "CHECK_EQUAL_FLOAT", _check_equal_float_l },
    { "CHECK_NOT_EQUAL_FLOAT", _check_not_equal_float_l },
    { "CHECK_LESS_THAN_FLOAT", _check_less_than_float_l },
    { "CHECK_GREATER_THAN_FLOAT", _check_greater_than_float_l },
    { "CHECK_LESS_THAN_EQUAL_FLOAT", _check_less_than_equal_float_l },
    { "CHECK_GREATER_THAN_EQUAL_FLOAT", _check_greater_than_equal_float_l },

    { "CHECK_EQUAL_STRING", _check_equal_string_l },
    { "CHECK_NOT_EQUAL_STRING", _check_not_equal_string_l },
    { NULL, NULL },
};
static int _count_lua_test(lua_State* L)
{
    _num_tests++;
    switch(_current_result)
    {
    case kResultPass: _num_tests_passed++; printf("."); break;
    case kResultFail: _num_tests_failed++; break;
    case kResultIgnore: _num_tests_ignored++; printf("!"); break;
    }
    _current_result = kResultPass;
    return 0;
    (void)sizeof(L);
}
static int _ignore_lua_test(lua_State* L)
{
    _ignore_test();
    return 0;
    (void)sizeof(L);
}
#endif /* LUA_TESTS */

/* External functions
 */
void _fail(const char* file, int line, const char* format, ...)
{
    va_list args;
    char buffer[1024];
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    printf("\n"ERROR_FORMAT"%s\n", file, line, buffer);
    _current_result = kResultFail;
}

/* bool checks */
void _check_true(const char* file, int line, int value)
{
    if(!value)
        _fail(file, line, "False");
}
void _check_false(const char* file, int line, int value)
{
    if(value)
        _fail(file, line, "True");
}

/* integer checks */
void _check_equal(const char* file, int line, int64_t expected, int64_t actual)
{
    if(expected != actual)
        _fail(file,line, "Expected: %"PRId64"  Actual: %"PRId64"", expected, actual);
}
void _check_not_equal(const char* file, int line, int64_t expected, int64_t actual)
{
    if(expected == actual)
        _fail(file,line, "Actual value equals expected: %"PRId64"", actual);
}
void _check_less_than(const char* file, int line, int64_t left, int64_t right)
{
    if(left >= right)
        _fail(file,line, "%"PRId64" is not less than %"PRId64"", left, right);
}
void _check_greater_than(const char* file, int line, int64_t left, int64_t right)
{
    if(left <= right)
        _fail(file,line, "%"PRId64" is not greater than %"PRId64"", left, right);
}
void _check_less_than_equal(const char* file, int line, int64_t left, int64_t right)
{
    if(left > right)
        _fail(file,line, "%"PRId64" is not less than %"PRId64"", left, right);
}
void _check_greater_than_equal(const char* file, int line, int64_t left, int64_t right)
{
    if(left < right)
        _fail(file,line,"%"PRId64" is not greater than %"PRId64"", left, right);
}

/* pointer checks */
void _check_equal_pointer(const char* file, int line, const void* expected, const void* actual)
{
    if(expected != actual)
        _fail(file,line, "Expected: %p  Actual: %p", expected, actual);
}
void _check_not_equal_pointer(const char* file, int line, const void* expected, const void* actual)
{
    if(expected == actual)
        _fail(file,line, "Actual value equals expected: %p", actual);
}
void _check_null(const char* file, int line, const void* pointer)
{
    if(pointer != NULL)
        _fail(file,line, "Pointer is not NULL: %p", pointer);
}
void _check_not_null(const char* file, int line, const void* pointer)
{
    if(pointer == NULL)
        _fail(file,line, "Pointer is NULL");
}

/* float checks */
void _check_equal_float(const char* file, int line, double expected, double actual)
{
    if(fabs(expected - actual) > EPSILON)
        _fail(file,line, "Expected: %f  Actual: %f", expected, actual);
}
void _check_equal_float_epsilon(const char* file, int line, double expected, double actual, double epsilon)
{
    if(fabs(expected - actual) > epsilon)
        _fail(file,line, "Expected: %f  Actual: %f", expected, actual);
}
void _check_not_equal_float(const char* file, int line, double expected, double actual)
{
    if(fabs(expected - actual) < EPSILON)
        _fail(file,line, "Actual value equals expected: %f", actual);
}
void _check_less_than_float(const char* file, int line, double left, double right)
{
    if(left >= right)
        _fail(file,line, "%f is not less than %f", left, right);
}
void _check_greater_than_float(const char* file, int line, double left, double right)
{
    if(left <= right)
        _fail(file,line, "%f is not greater than %f", left, right);
}
void _check_less_than_equal_float(const char* file, int line, double left, double right)
{
    if(left > right)
        _fail(file,line, "%f is not less than %f", left, right);
}
void _check_greater_than_equal_float(const char* file, int line, double left, double right)
{
    if(left < right)
        _fail(file,line, "%f is not greater than %f", left, right);
}

/* string checks */
void _check_equal_string(const char* file, int line, const char* expected, const char* actual)
{
    if(strcmp(expected, actual) != 0)
        _fail(file,line, "Expected: %s  Actual: %s", expected, actual);
}
void _check_not_equal_string(const char* file, int line, const char* expected, const char* actual)
{
    if(strcmp(expected, actual) == 0)
        _fail(file,line, "Strings are equal: %s", actual);
}


int _register_test(test_func_t* func)
{
    _test_funcs[_num_tests] = func;
    return _num_tests++;
}
int __ignore_test(test_func_t* func)
{
    _test_funcs[_num_tests] = _ignore_test;
    return _num_tests++;
    (void)sizeof(func);
}
void _ignore_test(void)
{
    _current_result = kResultIgnore;
}

int run_all_tests(int argc, const char* argv[])
{
    int ii;
    #if LUA_TESTS
    const char script[] =
    "function run_tests()\n"\
    "    for key, val in pairs(_G) do\n"\
    "        if \"function\" == type(val) then\n"\
    "            if string.find(key, \"_Test\") then\n"\
    "                if string.find(key, \"Ignore_\") then\n"\
    "                    _ignore_lua_test()\n"\
    "                else\n"\
    "                    _G[key]()\n"\
    "                end\n"\
    "                    _count_lua_test()\n"\
    "                _G[key] = nil\n"\
    "            end\n"\
    "        end\n"\
    "    end\n"\
    "end";
    char cwd[1024] = {0};
    int result;
    DIR *dir = NULL;
    struct dirent *ent = NULL;

    /* Seed a random number */
    srand((uint32_t)time(NULL));

    /* Create Lua state */
    _L = luaL_newstate();
    luaL_openlibs(_L);
    for(ii=0; ii<(int)sizeof(_lua_test_methods)/(int)sizeof(_lua_test_methods[0])-1; ++ii) {
        lua_pushcfunction(_L, _lua_test_methods[ii].func);
        lua_setglobal(_L, _lua_test_methods[ii].name);
    }
    lua_pushcfunction(_L, _count_lua_test);
    lua_setglobal(_L, "_count_lua_test");
    lua_pushcfunction(_L, _ignore_lua_test);
    lua_setglobal(_L, "_ignore_lua_test");
    luaL_dostring(_L, script);
    #endif /* LUA_TESTS */

    printf("------------------------------------------------------------");


    /* C++ tests */
    for(ii=0;ii<_num_tests;++ii) {
        if(ii % 60 == 0)
            printf("\n");
        _current_result = kResultPass;
        _test_funcs[ii]();
        switch(_current_result)
        {
        case kResultPass: _num_tests_passed++; printf("."); break;
        case kResultFail: _num_tests_failed++; break;
        case kResultIgnore: _num_tests_ignored++; printf("!"); break;
        }
    }

    #if LUA_TESTS
    /* Lua tests */
    getcwd(cwd, sizeof(cwd));
    if ((dir = opendir (".")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            const char* str = ent->d_name;
            const char* ext = _get_ext(ent->d_name);
            if(ext == NULL)
                continue;
            if(strcmp(ext, "lua") == 0) {
                snprintf(_current_lua_test_file,  sizeof(_current_lua_test_file), "%s/%s", cwd, str);
                _current_result = kResultPass;

                result = luaL_loadfile(_L, str);
                if(result) {
                    printf("\n%s\n", lua_tostring(_L, -1));
                } else {
                    luaL_dofile(_L, str);
                    lua_getglobal(_L, "run_tests");
                    lua_pcall(_L, 0, 0, 0);
                }
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
    lua_close(_L);

    #endif /* LUA_TESTS */

    printf("\n------------------------------------------------------------\n");
    printf("%d failed, %d passed, %d ignored, %d total\n",
            _num_tests_failed, _num_tests_passed, _num_tests_ignored, _num_tests);


    return _num_tests_failed;
    (void)sizeof(argc);
    (void)sizeof(*argv);
}
