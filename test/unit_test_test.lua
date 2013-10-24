function TestUnit_Test()
    local x = 60
    local y = 60
    CHECK_EQUAL_FLOAT(x,y)
end

function Ignore_NotATest_Test()
    local x = 60
    local y = 60
    CHECK_EQUAL(x,y)
end

function BasicCheck_Test()
	CHECK_TRUE(true)
	CHECK_FALSE(false)
	CHECK_NOT_EQUAL(8, -9)
	CHECK_EQUAL(990234, 990234)
	CHECK_LESS_THAN(32, 43)
	CHECK_GREATER_THAN(43, 32)
	CHECK_NULL(nil)
	CHECK_NOT_EQUAL_FLOAT(-23.4, 23.4)
	CHECK_EQUAL_FLOAT(23.4, 23.4)
	CHECK_EQUAL_STRING("This is a string", "This is a string")
	CHECK_NOT_EQUAL_STRING("This is a string", "This is a string too")
end

