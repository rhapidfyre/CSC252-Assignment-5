
#include "BinarySearch.h"

// Implement binary search here
bool binary_search_recursive(int* values, int key, int start, int end)
{
    return false;
}

bool binary_search(int* values, int key, int size)
{
    return false;
}


// ===========================================================================
// Testing implementation below. Write binary search logic above these lines.
// ===========================================================================

CSC252::BinarySearchTest::~BinarySearchTest()
{

}

/**
 * @brief Executes a test case to validate binary_search and binary_search_recursive.
 *
 * See the class comment in BinarySearch.h for the input format.
 *
 * @param Input The input stream containing one search test case.
 * @param Output The output stream to write "true", "false", "invalid_argument" or "exception".
 *
 * @throws std::runtime_error If the test case itself is malformed. That is a mistake in the test file,
 *         not a result from the search, so it is reported as a test error.
 */
void CSC252::BinarySearchTest::Execute(std::istream& Input, std::ostream& Output)
{
    // A leading letter selects the mode. A number (or '-') means a plain binary_search over a real array.
    Input >> std::ws;
    char Mode{'h'};
    const int First = Input.peek();
    if (First == 'e' || First == 'E' || First == 'r' || First == 'n')
        Input >> Mode;

    const bool bUsesNullArray = Mode == 'e' || Mode == 'n';
    const bool bUsesRange = Mode == 'r' || Mode == 'n';
    const bool bUsesDeclaredSize = Mode == 'e' || Mode == 'E';

    int Key{0};
    int Start{0};
    int End{-1};
    int Size{0};
    if (!(Input >> Key))
        throw std::runtime_error("Search test input must contain a key");
    if (bUsesDeclaredSize && !(Input >> Size))
        throw std::runtime_error("Search test input must contain a size after the key");
    if (bUsesRange && !(Input >> Start >> End))
        throw std::runtime_error("Search test input must contain a start and end index after the key");

    vector<int> Values;
    if (!bUsesNullArray)
    {
        int Value;
        while (Input >> Value)
            Values.push_back(Value);

        if (!Input.eof())
            throw std::runtime_error("Input contained non-integer value(s)");
    }

    if (Mode == 'h')
        Size = static_cast<int>(Values.size());

    // A range or size that reaches past the values would make the search read past the array. That is a mistake in
    // the test file, not in the search, so it is reported as a test error. (Negative values are deliberately allowed
    // through, since rejecting them is the search's job.)
    if (Mode == 'E' && Size >= 0 && static_cast<size_t>(Size) > Values.size())
        throw std::runtime_error("Search test declares a size larger than its values");
    if (Mode == 'r' && Start >= 0 && Start <= End && static_cast<size_t>(End) >= Values.size())
        throw std::runtime_error("Search test range ends past its values");

    // A real pointer is passed even for an empty array, because an empty vector's data() may be null,
    // which would turn "valid empty array" into the same case as "null array".
    int Placeholder{0};
    int* Data = nullptr;
    if (!bUsesNullArray)
    {
        Data = &Placeholder;
        if (!Values.empty())
            Data = Values.data();
    }

    try // the call under test may throw. What it throws is the result.
    {
        bool bFound;
        if (bUsesRange)
            bFound = binary_search_recursive(Data, Key, Start, End);
        else
            bFound = binary_search(Data, Key, Size);

        Output << (bFound ? "true" : "false");
    }
    catch (const std::invalid_argument&)
    {
        Output << "invalid_argument";
    }
    catch (const std::exception&)
    {
        Output << "exception";
    }
    catch (...)
    {
        throw std::runtime_error("Unexpected exception");
    }
}

bool CSC252::BinarySearchTest::Compare(const string& Expected, const string& Actual, TestResult& OutResult)
{
    // Only the four result words are allowed. Anything else is a typo in a .out file, not a result.
    const bool bExpected = Expected == "true";
    const bool bInvalidArg = Expected == "invalid_argument";
    const bool bException = Expected == "exception";
    if (!(bExpected || bInvalidArg || bException))
        throw std::runtime_error("Expected output must be true, false, invalid_argument or exception");

    if (TestObject::Compare(Expected, Actual, OutResult))
        return true;

    OutResult.Message = "Expected '" + Expected + "' but got '" + Actual + "'";
    return false;
}
