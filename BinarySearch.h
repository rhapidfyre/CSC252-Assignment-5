#pragma once

#include "TestObject.h"

// Headers: This defines them so other files can reach them, without specifying their implementation.
//          Implement them in BinarySearch.cpp
bool binary_search_recursive(int* values, int key, int start, int end);
bool binary_search(int* values, int key, int size);



// ===========================================================================
// Testing implementation below. Write binary search logic above these lines.
// ===========================================================================

namespace CSC252
{
    /**
     * @class BinarySearchTest
     * @brief A test class for validating binary_search and binary_search_recursive.
     *
     * Reads one test case from a .in file and writes "true", "false", "invalid_argument" or "exception".
     * A case starts with an optional mode letter, then a key. Values (when present) must already be sorted.
     */
    class BinarySearchTest : public TestObject
    {
    public:
        explicit BinarySearchTest(const int TestNumber) : TestObject(TestNumber) {}
        ~BinarySearchTest() override;
    protected:
        void Execute(std::istream& Input, std::ostream& Output) override;
        bool Compare(const string& Expected, const string& Actual, TestResult& OutResult) override;
    };
}
