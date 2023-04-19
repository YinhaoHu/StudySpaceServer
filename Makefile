TARGET_EXEC := server

CXX := g++

CXXFLAGS := -Wall -std=c++20

SRC_DIR := ./src
LIB_DIR := ./lib
BUILD_DIR := ./build
RESULT_DIR := ./result

SRCS := $(shell find $(LIB_DIR) $(SRC_DIR) -name '*.cpp')

OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

$(RESULT_DIR)/$(TARGET_EXEC): $(OBJS)
	g++ $(CXXFLAGS) $(LIB_SRCS) $(SRCS) -o $@
	
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
