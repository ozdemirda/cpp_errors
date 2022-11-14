CC = g++

INCLUDE_DIR = ../../include
OBJECT_DIR = objects

_create_object_dir := $(shell mkdir -p $(OBJECT_DIR))

CFLAGS = -I$(INCLUDE_DIR) -Wall -O3
LFLAGS = 

HEADER_FILES = $(INCLUDE_DIR)/code_location.h \
	$(INCLUDE_DIR)/error.h \
	$(INCLUDE_DIR)/error_types/predefined_errors.h \
	$(INCLUDE_DIR)/error_types/user_defined_errors.h

default: all

example: $(OBJECT_DIR)/example.o
	$(CC) -o example $(OBJECT_DIR)/example.o $(LFLAGS)

all: example

$(OBJECT_DIR)/example.o:  example.cpp $(HEADER_FILES)
	$(CC) $(CFLAGS) -c example.cpp -o $(OBJECT_DIR)/example.o

clean:
	rm -rf example $(OBJECT_DIR)
