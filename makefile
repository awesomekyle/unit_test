LIBRARY = ./libunittest.a
TARGET = ./test_unit_test

SRCS = src/unit_test.c

TEST_SRCS = test/main.c

OBJECTS = $(SRCS:.c=.o)
TEST_OBJECTS = $(TEST_SRCS:.c=.o)

CFLAGS += -MMD -MP -g -Isrc
CXXFLAGS += -MMD -MP -g -Isrc

_DEPS := $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

.PHONY: clean test

all: $(TARGET) test

$(LIBRARY) : $(OBJECTS)
	@echo "Creating library..."
	ar rcs $(LIBRARY) $(OBJECTS)

$(TARGET) : $(LIBRARY) $(TEST_OBJECTS)
	@echo "Creating tests..."
	$(CXX) $(LDFLAGS) $(TEST_OBJECTS) $(LIBRARY) -o $(TARGET)

test: $(TARGET)
	$(TARGET) -t

%.o : %.c
	@echo "Compiling $<..."
	$(SILENT) $(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	@echo "Compiling $<..."
	$(SILENT) $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	$(RM) -f -r $(OBJECTS) $(TEST_OBJECTS) $(_DEPS)
	$(RM) $(LIBRARY) $(TARGET)

-include $(_DEPS)

