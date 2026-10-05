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

    /**
     * @brief Processes input data to test the SelectionSort class and outputs the result.
     *
     * This method allows us to catch test cases that expect an exception.
     *
     * @param Input The input stream containing test data. Expected to start with 'e' or 'E'
     *        followed by the vector size and integer elements.
     * @param Output The output stream where results or exception messages are written.
     *        If successful, writes "OK" followed by the sorted integers. For exceptions,
     *        writes "invalid_argument" or "exception".
     *
     * @throws std::runtime_error If the input does not begin with 'e/E' followed by an integer size.
     */
    void ExpectsThrowCase(std::istream& Input, std::ostream& Output)
    {
        char FirstCharacter{'x'};
        int VectorSize{-1};
        if (!(Input >> FirstCharacter >> VectorSize)) // test case input incorrect
            throw std::runtime_error("Throw test input must start with e/E and a size");
        vector<int> Values;
        int Value;
        while (Input >> Value)
            Values.push_back(Value);

        if (!Input.eof())
            throw std::runtime_error("Input contained non-integer value(s)");

        // A size larger than the data would make Sort read past the array. That is a mistake in the test
        // file, not in the sort, so it is reported as a test error rather than a pass or fail.
        // (Negative sizes are deliberately allowed through, since the sort is expected to reject them.)
        if (FirstCharacter == 'E' && VectorSize >= 0 && static_cast<size_t>(VectorSize) > Values.size())
            throw std::runtime_error("Throw test declares a size larger than its values");

        // 'e' passes nullptr. 'E' passes a real pointer, even for an empty array, because an empty vector's
        // data() may be null, which would turn "valid empty array" into the same case as "null pointer".
        int Placeholder{0};
        int* Data = nullptr;
        if (FirstCharacter != 'e')
        {
            Data = &Placeholder;
            if (!Values.empty())
                Data = Values.data();
        }

        try // the try case is expected to throw an exception
        {
            CSC252::SelectionSort SortObject;
            SortObject.Sort(Data, VectorSize);
        }
        catch (const std::invalid_argument&)
        {
            Output << "invalid_argument";
            return;
        }
        catch (const std::exception&)
        {
            Output << "exception";
            return;
        }
        catch (...) // unexpected exception caught
        {
            throw std::runtime_error("Unexpected exception caught - this is an unexpected test failure.");
        }
        Output << "OK";
        for (const int& OutValue : Values)
            Output << ' ' << OutValue;
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
    // Explicitly allows us to catch test cases that expect an exception
    // i.e.: "This test makes sure an exception is thrown by the algorithm"
    Input >> std::ws;
    const int FirstCharacter = Input.peek(); // read the first character without consuming it
    if (FirstCharacter == 'e' || FirstCharacter == 'E')
    {
        ExpectsThrowCase(Input, Output);
        return;
    }
    
    // Expected Exception handled. Any exception hereafter is unexpected and a test case failure.
    
    // Read the input file's contents
    vector<int> Values{};
    int Value{-1};
    while (Input >> Value)
        Values.push_back(Value);

    // If the end of file was not reached, the cast to int failed, meaning the input contained non-integer values
    if (!Input.eof())
        throw std::runtime_error("Input contained non-integer value(s)");

    SelectionSort Sorter;
    Sorter.Sort(Values.data(), static_cast<int>(Values.size()));

    // Adds a space between each value
    for (size_t Index = 0; Index < Values.size(); ++Index)
    {
        if (Index > 0)
            Output << ' ';
        Output << Values[Index];
    }
}

bool CSC252::SelectionSortTest::Compare(const string& Expected, const string& Actual, TestResult& OutResult)
{
    bool bExpected, bActual;
    const vector<int> NeededValues = ValidIntegers(Expected, bExpected);
    const vector<int> ActualValues = ValidIntegers(Actual, bActual);
    
    if (!bExpected)
    {
        // Only the throw-case words are allowed here. Anything else is a typo in a .out file, not a result.
        bool bNotOk = Expected.rfind("OK", 0) == 0;
        bool bInvalidArgs = Expected == "invalid_argument";
        bool bException = Expected == "exception";
        if (!(bNotOk || bInvalidArgs || bException))
            throw std::runtime_error("Expected output contained non-integer value(s)");

        if (TestObject::Compare(Expected, Actual, OutResult))
            return true;

        OutResult.Message = "Expected '" + Expected + "' but got '" + Actual + "'";
        return false;
    }

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
