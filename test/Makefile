CC = g++

INCLUDE_DIR = ../include
OBJECT_DIR = objects

_create_object_dir := $(shell mkdir -p $(OBJECT_DIR))

CFLAGS = -I$(INCLUDE_DIR) -Wall -O3
LFLAGS = -lgtest -lgtest_main

HEADER_FILES = $(INCLUDE_DIR)/code_location.h \
	$(INCLUDE_DIR)/error.h \
	$(INCLUDE_DIR)/error_types/predefined_errors.h \
	$(INCLUDE_DIR)/error_types/user_defined_errors.h

default: all

tests: $(OBJECT_DIR)/tests.o
	$(CC) -o tests $(OBJECT_DIR)/tests.o $(LFLAGS)

all: tests

$(OBJECT_DIR)/tests.o:  tests.cpp $(HEADER_FILES)
	$(CC) $(CFLAGS) -c tests.cpp -o $(OBJECT_DIR)/tests.o

clean:
	rm -rf tests $(OBJECT_DIR)
