TOP_DIR := $(shell pwd)
SRC_DIR := $(TOP_DIR)/src
BUILD_DIR ?= $(TOP_DIR)/build
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
TARGET_EXEC := compiler


CXXFLAGS := -std=c++17 -g -I/usr/local/include/antlr4-runtime -I./frontend -L/usr/local/lib/ -Wl,-rpath=/usr/local/lib/ -lantlr4-runtime
CXX := clang++

$(BUILD_DIR)/$(TARGET_EXEC): antlr 
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@


compile:
	clang++  $(SRCS)  -o build/main

run: compile
	$(BUILD_DIR)/$(TARGET_EXEC) -i test/test.sysy -o out

test:
	$(BUILD_DIR)/$(TARGET_EXEC) -i test/test.sysy -o out

debug:
	gdb $(BUILD_DIR)/$(TARGET_EXEC) -x test/init.gdb
antlr:
	java -jar /usr/local/lib/antlr-4.13.1-complete.jar -Dlanguage=Cpp $(SRC_DIR)/frontend/SysYParser.g4 $(SRC_DIR)/frontend/SysYLexer.g4 -visitor

.PHONY: test 

MODE = koopa
LV = 1

autotest:
	autotest -${MODE} -s lv$(LV) .