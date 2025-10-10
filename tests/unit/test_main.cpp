#include "../framework/simple_test.h"

/**
 * Main test entry point
 * Runs all registered tests and returns exit code
 */

int main() {
    return Testing::TestRunner::instance().runTests();
}
