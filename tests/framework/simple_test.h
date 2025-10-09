#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

// For isatty() to detect if output is a terminal
#ifdef _WIN32
    #include <io.h>
    #define isatty _isatty
    #define fileno _fileno
#else
    #include <unistd.h>
#endif

/**
 * Simple lightweight testing framework for Genesis Plus GX
 * Based on the snes9xGC/fceuGC test framework pattern
 * 
 * This framework provides:
 * - Test registration and automatic discovery
 * - Assertion macros with line numbers
 * - Test result reporting
 * - No external dependencies (uses only C++ standard library)
 */

namespace Testing {

// Forward declarations
class TestCase;
class TestRunner;

//-----------------------------------------------------------------------------
// Test Result Tracking
//-----------------------------------------------------------------------------
struct TestResult {
    std::string testName;
    bool passed;
    std::string failureMessage;
    std::string file;
    int line;
    
    TestResult(const std::string& name) 
        : testName(name), passed(true), file(""), line(0) {}
};

//-----------------------------------------------------------------------------
// Test Case Base Class
//-----------------------------------------------------------------------------
class TestCase {
public:
    TestCase(const std::string& name) : name_(name) {}
    virtual ~TestCase() {}
    
    virtual void run() = 0;
    
    const std::string& getName() const { return name_; }
    
protected:
    std::string name_;
    friend class TestRunner;
};

//-----------------------------------------------------------------------------
// Test Runner (Singleton)
//-----------------------------------------------------------------------------
class TestRunner {
public:
    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }
    
    void registerTest(TestCase* test) {
        tests_.push_back(test);
    }
    
    int runTests() {
        // Check if output is a TTY (terminal) or being redirected
        bool useColors = isatty(fileno(stdout));
        
        const char* green = useColors ? "\033[32m" : "";
        const char* red = useColors ? "\033[31m" : "";
        const char* reset = useColors ? "\033[0m" : "";
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Genesis Plus GX Test Suite" << std::endl;
        std::cout << "========================================\n" << std::endl;
        
        int totalTests = 0;
        int passedTests = 0;
        int failedTests = 0;
        
        for (auto* test : tests_) {
            totalTests++;
            currentTest_ = TestResult(test->getName());
            
            std::cout << "Running: " << test->getName() << " ... ";
            std::cout.flush();
            
            try {
                test->run();
                
                if (currentTest_.passed) {
                    std::cout << green << "PASSED" << reset << std::endl;
                    passedTests++;
                } else {
                    std::cout << red << "FAILED" << reset << std::endl;
                    std::cout << "  " << currentTest_.file << ":" << currentTest_.line << std::endl;
                    std::cout << "  " << currentTest_.failureMessage << std::endl;
                    failedTests++;
                }
            } catch (const std::exception& e) {
                std::cout << red << "EXCEPTION" << reset << std::endl;
                std::cout << "  " << e.what() << std::endl;
                failedTests++;
            } catch (...) {
                std::cout << red << "UNKNOWN EXCEPTION" << reset << std::endl;
                failedTests++;
            }
        }
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Test Results:" << std::endl;
        std::cout << "  Total:  " << totalTests << std::endl;
        std::cout << "  Passed: " << green << passedTests << reset << std::endl;
        std::cout << "  Failed: " << red << failedTests << reset << std::endl;
        std::cout << "========================================\n" << std::endl;
        
        return (failedTests == 0) ? 0 : 1;
    }
    
    void recordFailure(const std::string& message, const char* file, int line) {
        currentTest_.passed = false;
        currentTest_.failureMessage = message;
        currentTest_.file = file;
        currentTest_.line = line;
    }
    
private:
    TestRunner() {}
    ~TestRunner() {
        for (auto* test : tests_) {
            delete test;
        }
    }
    
    TestRunner(const TestRunner&) = delete;
    TestRunner& operator=(const TestRunner&) = delete;
    
    std::vector<TestCase*> tests_;
    TestResult currentTest_ = TestResult("");
};

//-----------------------------------------------------------------------------
// Test Registration Helper
//-----------------------------------------------------------------------------
template<typename T>
class TestRegistrar {
public:
    TestRegistrar(const std::string& name) {
        TestRunner::instance().registerTest(new T(name));
    }
};

} // namespace Testing

//-----------------------------------------------------------------------------
// Test Definition Macros
//-----------------------------------------------------------------------------

// Define a test case
#define TEST(TestName) \
    class Test_##TestName : public Testing::TestCase { \
    public: \
        Test_##TestName(const std::string& name) : TestCase(name) {} \
        void run() override; \
    }; \
    static Testing::TestRegistrar<Test_##TestName> registrar_##TestName(#TestName); \
    void Test_##TestName::run()

//-----------------------------------------------------------------------------
// Assertion Macros
//-----------------------------------------------------------------------------

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #condition; \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            std::ostringstream msg; \
            msg << "Assertion failed: !(" << #condition << ")"; \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #expected << " == " << #actual; \
            msg << "\n  Expected: " << (expected); \
            msg << "\n  Actual:   " << (actual); \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_NE(not_expected, actual) \
    do { \
        if ((not_expected) == (actual)) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #not_expected << " != " << #actual; \
            msg << "\n  Not expected: " << (not_expected); \
            msg << "\n  Actual:       " << (actual); \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_LT(val1, val2) \
    do { \
        if (!((val1) < (val2))) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #val1 << " < " << #val2; \
            msg << "\n  Value 1: " << (val1); \
            msg << "\n  Value 2: " << (val2); \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_LE(val1, val2) \
    do { \
        if (!((val1) <= (val2))) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #val1 << " <= " << #val2; \
            msg << "\n  Value 1: " << (val1); \
            msg << "\n  Value 2: " << (val2); \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_GT(val1, val2) \
    do { \
        if (!((val1) > (val2))) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #val1 << " > " << #val2; \
            msg << "\n  Value 1: " << (val1); \
            msg << "\n  Value 2: " << (val2); \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_GE(val1, val2) \
    do { \
        if (!((val1) >= (val2))) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #val1 << " >= " << #val2; \
            msg << "\n  Value 1: " << (val1); \
            msg << "\n  Value 2: " << (val2); \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_STREQ(expected, actual) \
    do { \
        if (std::strcmp((expected), (actual)) != 0) { \
            std::ostringstream msg; \
            msg << "Assertion failed: strcmp(" << #expected << ", " << #actual << ") == 0"; \
            msg << "\n  Expected: \"" << (expected) << "\""; \
            msg << "\n  Actual:   \"" << (actual) << "\""; \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_STRNE(not_expected, actual) \
    do { \
        if (std::strcmp((not_expected), (actual)) == 0) { \
            std::ostringstream msg; \
            msg << "Assertion failed: strcmp(" << #not_expected << ", " << #actual << ") != 0"; \
            msg << "\n  Not expected: \"" << (not_expected) << "\""; \
            msg << "\n  Actual:       \"" << (actual) << "\""; \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != nullptr) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #ptr << " == nullptr"; \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == nullptr) { \
            std::ostringstream msg; \
            msg << "Assertion failed: " << #ptr << " != nullptr"; \
            Testing::TestRunner::instance().recordFailure(msg.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while(0)

#endif // SIMPLE_TEST_H
