# Genesis Plus GX Unit Test Makefile
# Uses standard g++ for compilation (not DevkitPPC)

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -pedantic
CXXFLAGS += -I./framework -I./mocks
LDFLAGS :=

# Directories
FRAMEWORK_DIR := framework
MOCKS_DIR := mocks
UNIT_DIR := unit
BUILD_DIR := build

# Output
TEST_EXECUTABLE := $(BUILD_DIR)/test_runner

# Source files
MOCK_SOURCES := $(wildcard $(MOCKS_DIR)/*.cpp)
TEST_SOURCES := $(wildcard $(UNIT_DIR)/test_*.cpp)
MAIN_SOURCE := $(UNIT_DIR)/test_main.cpp

# Object files
MOCK_OBJECTS := $(patsubst $(MOCKS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MOCK_SOURCES))
TEST_OBJECTS := $(patsubst $(UNIT_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SOURCES))
ALL_OBJECTS := $(MOCK_OBJECTS) $(TEST_OBJECTS)

# Colors for output
GREEN := \033[32m
RED := \033[31m
YELLOW := \033[33m
RESET := \033[0m

# Default target
.PHONY: all
all: tests

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile mock sources
$(BUILD_DIR)/%.o: $(MOCKS_DIR)/%.cpp | $(BUILD_DIR)
	@echo "$(YELLOW)Compiling mock:$(RESET) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test sources
$(BUILD_DIR)/%.o: $(UNIT_DIR)/%.cpp | $(BUILD_DIR)
	@echo "$(YELLOW)Compiling test:$(RESET) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Link test executable
$(TEST_EXECUTABLE): $(ALL_OBJECTS)
	@echo "$(YELLOW)Linking:$(RESET) $@"
	@$(CXX) $(CXXFLAGS) $(ALL_OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(GREEN)Build complete:$(RESET) $@"

# Build tests
.PHONY: tests
tests: $(TEST_EXECUTABLE)

# Run tests
.PHONY: run
run: tests
	@echo "\n$(YELLOW)Running tests...$(RESET)\n"
	@./$(TEST_EXECUTABLE) && echo "\n$(GREEN)All tests passed!$(RESET)" || (echo "\n$(RED)Tests failed!$(RESET)" && exit 1)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "$(YELLOW)Cleaning build artifacts...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)Clean complete$(RESET)"

# Rebuild everything
.PHONY: rebuild
rebuild: clean all

# Static analysis with cppcheck (optional)
.PHONY: static-analysis
static-analysis:
	@echo "$(YELLOW)Running static analysis...$(RESET)"
	@which cppcheck > /dev/null 2>&1 && \
		cppcheck --enable=all --suppress=missingIncludeSystem \
		--quiet --error-exitcode=1 \
		$(MOCKS_DIR) $(UNIT_DIR) || \
		echo "$(YELLOW)cppcheck not installed, skipping$(RESET)"

# Code coverage (if gcov/lcov available)
.PHONY: coverage
coverage: CXXFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: clean tests
	@echo "$(YELLOW)Running tests with coverage...$(RESET)"
	@./$(TEST_EXECUTABLE)
	@echo "$(YELLOW)Generating coverage report...$(RESET)"
	@which lcov > /dev/null 2>&1 && \
		lcov --capture --directory $(BUILD_DIR) --output-file $(BUILD_DIR)/coverage.info && \
		genhtml $(BUILD_DIR)/coverage.info --output-directory $(BUILD_DIR)/coverage_html && \
		echo "$(GREEN)Coverage report generated in $(BUILD_DIR)/coverage_html$(RESET)" || \
		echo "$(YELLOW)lcov not installed, coverage report not generated$(RESET)"

# Help target
.PHONY: help
help:
	@echo "Genesis Plus GX Test Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make tests           - Build test executable"
	@echo "  make run             - Build and run all tests"
	@echo "  make clean           - Remove build artifacts"
	@echo "  make rebuild         - Clean and rebuild"
	@echo "  make static-analysis - Run cppcheck (if available)"
	@echo "  make coverage        - Generate code coverage report"
	@echo "  make help            - Show this help message"
	@echo ""
	@echo "Test executable: $(TEST_EXECUTABLE)"
	@echo "Test sources: $(words $(TEST_SOURCES)) files"

# Dependencies
-include $(ALL_OBJECTS:.o=.d)

# Generate dependency files
$(BUILD_DIR)/%.o: $(MOCKS_DIR)/%.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: $(UNIT_DIR)/%.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
