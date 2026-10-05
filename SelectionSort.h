#pragma once

#include "TestObject.h"
#include <iosfwd>

// Headers: This defines them so other files can reach them, without specifying their implementation.
//          Implement them in SelectionSort.cpp
void selection_sort(int* Array, int Size);



// ===========================================================================
// Testing implementation below. Write binary search logic above these lines.
// ===========================================================================

namespace CSC252
{    
    /**
     * @class SelectionSortTest
     * @brief A test class for validating the functionality of the SelectionSort class.
     *
     * This class inherits from TestObject and overrides the Execute method to perform
     * specific testing of the SelectionSort implementation.
     */
    class SelectionSortTest : public TestObject
    {
    public:
        explicit SelectionSortTest(const int TestNumber) : TestObject(TestNumber) {}
        ~SelectionSortTest() override;
    protected:
        void Execute(std::istream& Input, std::ostream& Output) override;
        bool Compare(const string& Expected, const string& Actual, TestResult& OutResult) override;
    };
}
