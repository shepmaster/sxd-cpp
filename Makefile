GLIB_CFLAGS := $(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS := $(shell pkg-config --libs glib-2.0)

CC := gcc
CFLAGS := -Werror -Wall -ansi -pedantic -g -m32 $(GLIB_CFLAGS)
LDFLAGS := $(GLIB_LDFLAGS)

ALL_C_FILES := $(wildcard *.c)

TESTS := $(filter %-test.c,$(ALL_C_FILES))
TESTS := $(patsubst %.c,%,$(TESTS))
TEST_RESULTS := $(addsuffix .results,$(TESTS))

LIB_OBJECTS := $(filter-out %-test.c,$(ALL_C_FILES))
LIB_OBJECTS := $(patsubst %.c,%.o,$(LIB_OBJECTS))

default: $(TEST_RESULTS) $(TESTS)

%.results: %
	./$^ > $@

%-test: $(LIB_OBJECTS) %-test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TEST_RESULTS) $(TESTS) $(LIB_OBJECTS)

.PRECIOUS: %.o