#include "SelectionSort.h"


CSC252::SelectionSort::SelectionSort() : UnsortedArray(nullptr), SortedArray(nullptr)
{
    
}

CSC252::SelectionSort::~SelectionSort()
{
    if (UnsortedArray != nullptr)
    {
        delete[] UnsortedArray;
        UnsortedArray = nullptr;
    }
        
    if (SortedArray != nullptr)
    {
        delete[] SortedArray;
        SortedArray = nullptr;
    }
    
}

void CSC252::SelectionSort::Sort(int* Array, int Size)
{
    
}


// ===========================================================================
// Testing implementation below. Write selection sort logic above these lines.
// ===========================================================================

namespace
{
    /**
     * @brief Parses a string to extract valid integers and determines if the parsing was successful.
     *
     * Returns a vector containing the validated integers extracted from the input string.
     *
     * @param ResultText The input string containing integers to validate.
     * @param bValid True if the entire string is valid ints. False otherwise.
     * @return A vector containing the integers extracted from the string.
     */
    vector<int> ValidIntegers(const string& ResultText, bool& bValid)
    {
        vector<int> Values;
        std::istringstream Stream(ResultText);
        int Value;
        while (Stream >> Value)
            Values.push_back(Value);
        bValid = Stream.eof(); // only valid if we reached the end of the stream
        return Values;
    }
}

CSC252::SelectionSortTest::~SelectionSortTest()
{
    
}

/**
 * @brief Executes a test case to validate the SelectionSort implementation.
 *
 * Reads a sequence of integers from the input stream, sorts them using the
 * SelectionSort class, and writes the sorted integers to the output stream.
 *
 * @param Input The input stream containing a sequence of integers to sort.
 * @param Output The output stream to write the sorted integers.
 */
void CSC252::SelectionSortTest::Execute(std::istream& Input, std::ostream& Output)
{
    vector<int> Values;
    int Value;
    while (Input >> Value)
        Values.push_back(Value);

    SelectionSort Sorter;
    Sorter.Sort(Values.data(), static_cast<int>(Values.size()));

    for (size_t Index = 0; Index < Values.size(); ++Index)
    {
        if (Index > 0)
            Output << ' ';
        Output << Values[Index];
    }
}

bool CSC252::SelectionSortTest::Compare(
    const string& Expected, const string& Actual, TestResult& OutResult)
{
    bool bExpected, bActual;
    const vector<int> NeededValues = ValidIntegers(Actual, bExpected);
    const vector<int> ActualValues = ValidIntegers(Actual, bActual);
    if (!bExpected)
        throw std::runtime_error("Input contained non-integer value(s)");
    
    if (!bActual)
    {
        OutResult.Message = "Output contained non-integer value(s)";
        return false;
    }
    
    if (NeededValues.size() != ActualValues.size())
    {
        OutResult.Message = "Output and input were not the same size.";
        return false;
    }
        
    for (size_t Index = 0; Index < ActualValues.size(); Index++)
    {
        if (NeededValues[Index] != ActualValues[Index])
        {
            OutResult.Message = "Actual Value '" + std::to_string(ActualValues[Index]) + "' at index " 
                              + std::to_string(Index) + " did not match expected value '" 
                              + std::to_string(NeededValues[Index]) + "'";
            return false;
        }
    }
    
    OutResult.Message = "OK";
    return true;
}
