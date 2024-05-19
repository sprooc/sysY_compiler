MAKEFLAGS += -j8
TOP_DIR := $(shell pwd)
SRC_DIR := $(TOP_DIR)/src
BUILD_DIR ?= $(TOP_DIR)/build
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
TARGET_EXEC := compiler

CPPFLAGS := -std=c++17 -g -I/usr/local/include/antlr4-runtime -I./frontend 
CXXFLAGS := -std=c++17 -g -I/usr/local/include/antlr4-runtime -I./frontend -L/usr/local/lib/ -Wl,-rpath=/usr/local/lib/ -lantlr4-runtime
CXX := clang++
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.cpp.o, $(SRCS))
OBJS := $(patsubst $(BUILD_DIR)/%.cpp, $(BUILD_DIR)/%.cpp.o, $(OBJS))

$(BUILD_DIR)/$(TARGET_EXEC): 
	cp /home/sprooc/sysY-compiler/build/compiler $(BUILD_DIR)/$(TARGET_EXEC)

	# $(CXX) $(CXXFLAGS) $(SRCS) -o $@
	
define cxx_recipe
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@
endef
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp; $(cxx_recipe)

antlr:
	java -jar /usr/local/lib/antlr-4.13.1-complete.jar -Dlanguage=Cpp $(SRC_DIR)/frontend/SysYParser.g4 $(SRC_DIR)/frontend/SysYLexer.g4 -visitor -no-listener

compile: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC)

run: compile
	$(BUILD_DIR)/$(TARGET_EXEC) -i test/test.sysy -o out

test:
	$(BUILD_DIR)/$(TARGET_EXEC) -i test/test.sysy -o out

debug:
	gdb $(BUILD_DIR)/$(TARGET_EXEC) -x test/init.gdb


.PHONY: test 

MODE = koopa
LV = 1

autotest:
	autotest -${MODE} -s lv$(LV) .

clean:
	rm -f $(OBJS)