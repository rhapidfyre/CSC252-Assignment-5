#pragma once

#include "TestObject.h"
#include <iosfwd>

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
        SelectionSortTest(const int TestNumber) : TestObject(TestNumber) {};
    protected:
        void Execute(std::istream& Input, std::ostream& Output) override;
    };
    
    // A simple, undefined selection sort class object
    class SelectionSort
    {
    public:
        SelectionSort();
        ~SelectionSort();
        void Sort(int* Array, int Size);
    private:
        int* UnsortedArray;
        int* SortedArray;
    };
}
