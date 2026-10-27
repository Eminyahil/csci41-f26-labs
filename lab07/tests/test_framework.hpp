// CSCI 41 test framework.
//
// The whole thing is 70 lines and there is no magic in it you cannot read.
// Three things to use:
//
//   TEST("a name")  { ... }   declares a test
//   CHECK(expr)               fails the test if expr is false
//   CHECK_EQ(got, want)       same, but prints BOTH values when it fails
//
// You do not write main(). This header writes it for you, and it returns
// 0 if every test passed and 1 if any test failed, which is what CMake,
// CI and the lab grader all look at.
#ifndef CSCI41_TEST_FRAMEWORK_HPP
#define CSCI41_TEST_FRAMEWORK_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace csci41 {

struct TestCase {
    const char* name;
    void (*fn)();
};

// Every TEST() adds itself to this list before main() starts.
inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

inline int& failures_in_current_test() {
    static int n = 0;
    return n;
}

inline void report_failure(const char* file, int line, const std::string& what) {
    ++failures_in_current_test();
    if (file && *file)
        std::cout << "      FAIL  " << file << ":" << line << "\n";
    else
        std::cout << "      FAIL" << "\n";
    std::cout << "            " << what << std::endl;
}

// A little struct whose constructor runs before main(). That is the only
// trick in the file: creating one of these registers a test.
struct Registrar {
    Registrar(const char* name, void (*fn)()) { registry().push_back({name, fn}); }
};

inline int run_all() {
    int passed = 0, failed = 0;
    std::cout << "running " << registry().size() << " tests\n\n";
    for (const TestCase& t : registry()) {
        failures_in_current_test() = 0;
        std::cout << "  " << t.name << std::endl;
        // A test that throws fails by itself rather than taking the whole
        // run down with it. That matters while a function is still a stub.
        try {
            t.fn();
        } catch (const std::exception& e) {
            report_failure("", 0, std::string("threw: ") + e.what());
        } catch (...) {
            report_failure("", 0, "threw an unknown exception");
        }
        if (failures_in_current_test() == 0) { ++passed; }
        else { ++failed; }
    }
    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}

}  // namespace csci41

// Paste the test's own name into the function name so two tests cannot clash.
#define CSCI41_CAT2(a, b) a##b
#define CSCI41_CAT(a, b) CSCI41_CAT2(a, b)

#define TEST(name)                                                      \
    static void CSCI41_CAT(csci41_test_, __LINE__)();                   \
    static ::csci41::Registrar CSCI41_CAT(csci41_reg_, __LINE__)(       \
        name, &CSCI41_CAT(csci41_test_, __LINE__));                     \
    static void CSCI41_CAT(csci41_test_, __LINE__)()

// #expr turns the expression into a string, so the report can show the
// code you actually wrote rather than just "false".
#define CHECK(expr)                                                     \
    do {                                                                \
        if (!(expr))                                                    \
            ::csci41::report_failure(__FILE__, __LINE__,                \
                                     "CHECK(" #expr ") was false");     \
    } while (0)

#define CHECK_EQ(got, want)                                             \
    do {                                                                \
        auto csci41_g = (got);                                          \
        auto csci41_w = (want);                                         \
        if (!(csci41_g == csci41_w)) {                                   \
            std::ostringstream csci41_os;                               \
            csci41_os << "CHECK_EQ(" #got ", " #want ")\n"              \
                      << "                  got: " << csci41_g << "\n" \
                      << "               wanted: " << csci41_w;         \
            ::csci41::report_failure(__FILE__, __LINE__, csci41_os.str()); \
        }                                                               \
    } while (0)

int main() { return ::csci41::run_all(); }

#endif  // CSCI41_TEST_FRAMEWORK_HPP
