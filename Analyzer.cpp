#include "Analyzer.h"
#include "BinarySearch.h"
#include "SelectionSort.h"
#include <random>


// I couldn't find instructors or definitions in the assignment content, so this tries to match the Part4 output.
// Outputs the number of duplicated values, counting a value's occurrence once.
std::string DuplicatesAnalyser::analyze()
{
    try
    {
        selection_sort(Values, Size);
    }
    catch (...)
    {
        return "There were invalid arguments when trying to analyze duplicated values.";
    }

    // Equal values sit next to each other in sorted data, so a duplicated value is a run of two or more.
    // Index goes one past the end so that the final run is measured too.
    int StartIndex{0}, NumDuplicated{0};
    for (int Index = 1; Index <= Size; Index++)
    {
        if (Index < Size && Values[Index] == Values[StartIndex])
            continue; // still inside the current run

        if (Index - StartIndex > 1)
            NumDuplicated++;
        StartIndex = Index;
    }

    return "There were " + std::to_string(NumDuplicated) + " duplicated values";
}

// I couldn't find instructors or definitions in the assignment content, so this tries to match the Part4 output.
// Outputs the number of missing values.
std::string MissingAnalyser::analyze()
{
    constexpr int SmallestValue{0};
    constexpr int LargestValue{999};
    
    try
    {
        selection_sort(Values, Size);
    }
    catch (...)
    {
        return "There were invalid arguments when trying to analyze missing values.";
    }

    int NumMissing{0};
    for (int Value = SmallestValue; Value <= LargestValue; Value++)
    {
        if (!binary_search(Values, Value, Size))
            NumMissing++;
    }

    return "There were " + std::to_string(NumMissing) + " missing values";
}

SearchAnalyzer::SearchAnalyzer(int* values, int size) : Analyzer(values, size)
{
    // Sort upon construction
    selection_sort(Values, Size);
}

std::string SearchAnalyzer::analyze()
{
    constexpr int NumSearches{100}; // constexpr = known at compile time. Impossible to misinterpret.

    // Seeded from the system on every call, so each call searches for a different set of values.
    std::random_device Seed;
    
    // recycled mersenne twister algo from Assignment 4
    std::mt19937 Generator(Seed());
    // generate 100 random integer values in the range of 0 to 999 (SA-02, 05, 06, 07, and 08)
    std::uniform_int_distribution RandomValue(0, 999); // both ends are included

    int NumFound{0};
    for (int Search = 0; Search < NumSearches; Search++)
    {
        if (binary_search(Values, RandomValue(Generator), Size))
            NumFound++;
    }

    return "There were "
        + std::to_string(NumFound)    + " out of " 
        + std::to_string(NumSearches) + " random values found";
}

std::string StatisticsAnalyzer::analyze()
{
    selection_sort(Values, Size); // also rejects a null array or a negative size

    if (Size < 0)
        throw std::invalid_argument("invalid_argument");

    // Empty data is valid. There is nothing to read or divide by, so every statistic is reported as zero.
    if (Size == 0)
    {
        return "The minimum value is 0\n"
               "The maximum value is 0\n"
               "The mean value is 0.000000\n"
               "The median value is 0\n"
               "The mode value is 0 which occurred 0 times";
    }
    
    // largest possible sum of `int*` is an INT_MAX number of indices, which fits `long long`
    long long Sum{0};
    for (int Index = 0; Index < Size; Index++)
        Sum += Values[Index];

    // Equal values sit next to each other in sorted data, so the mode is the longest run of one value.
    // Index goes one past the end so that the final run is measured too.
    int RunningIndex{0}, Mode{0}, ModeCount{0};
    for (int Index = 1; Index <= Size; Index++)
    {
        if (Index < Size && Values[Index] == Values[RunningIndex])
            continue; // still inside the current run

        const int RunLength = Index - RunningIndex;
        if (RunLength > ModeCount) // only a longer run replaces the mode, so the first of a tie is kept
        {
            Mode = Values[RunningIndex];
            ModeCount = RunLength;
        }
        RunningIndex = Index;
    }
    
    const int Middle = Size / 2; // Gets the index of the literal middle, hence 'Middle'
    
    // The two middle values are widened before adding, for the same reason as Sum. Half of that fits in an int again.
    const int Median = Size % 2 == 0
        ? static_cast<int>((static_cast<long long>(Values[Middle - 1]) + Values[Middle]) / 2)
        : Values[Middle];
    
    return "The minimum value is " + std::to_string(Values[0]) + "\n"
         + "The maximum value is " + std::to_string(Values[Size - 1]) + "\n"
         + "The mean value is "    + std::to_string(static_cast<double>(Sum) / Size) + "\n"
         + "The median value is "  + std::to_string(Median) + "\n"
         + "The mode value is "    + std::to_string(Mode) + " which occurred " 
                                   + std::to_string(ModeCount) + " times";
}



// ===========================================================================
// Testing implementation below. Write analyzer logic above these lines.
// ===========================================================================

CSC252::StatisticsAnalyzerTest::~StatisticsAnalyzerTest()
{

}

// GENERATED BY CLAUDE/ANTHROPIC AI - Not required by the assignment.
// These requirement lists are generated by Claude/Anthropic AI to make my life easier because I spent 12 hours on this.
// 
// Requirement coverage, by test number (requirement IDs are from README.md). Every test builds the analyzer
// through an Analyzer pointer and prints all five lines, so STATS-02, STATS-03, STATS-09 and STATS-10 are
// exercised by every case and are only listed where a case is aimed at them.
//   00 Odd Count Unsorted       STATS-01, STATS-04
//   01 Even Median Exact        STATS-05
//   02 Even Median Remainder    STATS-05 (the mean of the two middle values is truncated)
//   03 Single Element           STATS-04, STATS-11
//   04 Two Equal Elements       STATS-05, STATS-06
//   05 All Same                 STATS-06
//   06 Mode Tie First Wins      STATS-07
//   07 Mode At End              STATS-08
//   08 Mode At Start            STATS-06
//   09 Mode Three Way Tie       STATS-01, STATS-07
//   10 Negatives And Zero       STATS-02, STATS-03, QA-02
//   11 Largest Ints             STATS-05, QA-02 (adding two INT_MAX values must not overflow)
//   12 Smallest And Largest     STATS-02, STATS-03, STATS-05, QA-02
//   13 Empty Array              STATS-11, STATS-12
//   14 Null Array Throws        QA-02 (throws on invalid input, per the README coding convention)
//   15 Null Size Zero           STATS-11, STATS-12 (empty data is valid)
//   16 All Unique Mode          STATS-06, STATS-07

/**
 * @brief Executes a test case to validate StatisticsAnalyzer.
 *
 * See the class comment in Analyzer.h for the input format.
 *
 * @param Input The input stream containing one statistics test case.
 * @param Output The output stream to write the analyzer's result to.
 *
 * @throws std::runtime_error If the test case itself is malformed. That is a mistake in the test file,
 *         not a result from the analyzer, so it is reported as a test error.
 */
void CSC252::StatisticsAnalyzerTest::Execute(std::istream& Input, std::ostream& Output)
{
    // Explicitly allows us to catch test cases that expect an exception
    // i.e.: "This test makes sure an exception is thrown by the analyzer"
    Input >> std::ws;
    const int FirstCharacter = Input.peek(); // read the first character without consuming it
    if (FirstCharacter == 'e')
    {
        this->ExpectsThrowCase(Input, Output);
        return;
    }

    // Expected Exception handled. Any exception hereafter is unexpected and a test case error.

    vector<int> Values;
    int Value;
    while (Input >> Value)
        Values.push_back(Value);
    if (!Input.eof())
        throw std::runtime_error("Input contained non-integer value(s)");

    int Spare{0};
    std::unique_ptr<Analyzer> AnalyzerObject =
        std::make_unique<StatisticsAnalyzer>(Values.empty() ? &Spare : Values.data(), static_cast<int>(Values.size()));
    
    Output << AnalyzerObject->analyze();
}

bool CSC252::StatisticsAnalyzerTest::Compare(const string& Expected, const string& Actual, TestResult& OutResult)
{
    if (TestObject::Compare(Expected, Actual, OutResult))
        return true;

    OutResult.Message = "Expected '" + Expected + "' but got '" + Actual + "'";
    return false;
}

void CSC252::StatisticsAnalyzerTest::ExpectsThrowCase(std::istream& Input, std::ostream& Output)
{
    char Action{'x'};
    int Size{-1};
    if (!(Input >> Action >> Size)) // test case input incorrect
        throw std::runtime_error("Throw test input must start with e and a size");

    try // the try case is expected to throw an exception
    {
        StatisticsAnalyzer AnalyzerObject(nullptr, Size);
        Output << AnalyzerObject.analyze();
    }
    catch (const std::invalid_argument&)
    {
        Output << "invalid_argument";
    }
    catch (const std::exception&)
    {
        Output << "exception";
    }
}

CSC252::SearchAnalyzerTest::~SearchAnalyzerTest()
{

}

// GENERATED BY CLAUDE/ANTHROPIC AI - Not required by the assignment.
// These requirement lists are generated by Claude/Anthropic AI to make my life easier because I spent 12 hours on this.
// 
// Requirement coverage, by test number (requirement IDs are from README.md). Every test builds the analyzer
// through an Analyzer pointer, so SA-01 and SA-03 are exercised by all of them.
//   00 All Found          SA-02, SA-05, SA-06, SA-07, SA-08 (unsorted data holding every value 0-999)
//   01 None Found         SA-07, SA-08
//   02 Empty Array        SA-07, SA-08, QA-02
//   03 Count Resets       SA-09
//   04 Null Array Throws  QA-02 (throws on invalid input, per the README coding convention)
//   05 Null Size Zero     QA-02 (null with size 0 is valid)
// SA-04 (exactly 100 searches) is only partly visible: "All Found" expects exactly 100 found. SA-05's endpoints
// (0 and 999 individually) are not tested, since that needs many repeated calls.

/**
 * @brief Executes a test case to validate SearchAnalyzer.
 *
 * See the class comment in Analyzer.h for the modes.
 *
 * @param Input The input stream containing one search analyzer test case.
 * @param Output The output stream to write the result to.
 *
 * @throws std::runtime_error If the test case itself is malformed. That is a mistake in the test file,
 *         not a result from the analyzer, so it is reported as a test error.
 */
void CSC252::SearchAnalyzerTest::Execute(std::istream& Input, std::ostream& Output)
{
    string ActionWord;
    if (!(Input >> ActionWord))
        throw std::runtime_error("Test must start with an action word");

    if (ActionWord == "null")
    {
        this->ExpectsThrowCase(Input, Output);
        return;
    }

    // Expected Exception handled. Any exception hereafter is unexpected and a test case error.
    const bool bScrambledData = ActionWord == "all" || ActionWord == "reset";
    const bool bFileData = ActionWord == "none";

    vector<int> Values{};
    if (bScrambledData)
    {
        // Generates random values as specified by the instructions
        for (int Index = 0; Index < 1000; Index++)
            Values.push_back(999-Index); // Reverses the order of the values, ensuring it's not ordered.
    }

    else if (bFileData)
    {
        int Value;
        while (Input >> Value)
            Values.push_back(Value);
        
        if (!Input.eof())
            throw std::runtime_error("Input contained non-integer value(s)");
    }

    else
        throw std::runtime_error("Unknown ActionWord: " + ActionWord);

    int Spare{0};
    std::unique_ptr<Analyzer> AnalyzerObject =
        std::make_unique<SearchAnalyzer>(Values.empty() ? &Spare : Values.data(), static_cast<int>(Values.size()));

    // 'reset' tests SA-09: Repeated calls are different counts
    if (ActionWord == "reset")
    {
        const string FirstAnswer = AnalyzerObject->analyze();
        Output << FirstAnswer << "\n" << AnalyzerObject->analyze();
        return;
    }

    Output << AnalyzerObject->analyze();
}

bool CSC252::SearchAnalyzerTest::Compare(const string& Expected, const string& Actual, TestResult& OutResult)
{
    if (TestObject::Compare(Expected, Actual, OutResult))
        return true;

    OutResult.Message = "Expected '" + Expected + "' but got '" + Actual + "'";
    return false;
}

void CSC252::SearchAnalyzerTest::ExpectsThrowCase(std::istream& Input, std::ostream& Output)
{
    int Size{-1};
    if (!(Input >> Size)) // test case input incorrect
        throw std::runtime_error("Throw test input must give a size after 'null'");

    try // the try case is expected to throw an exception
    {
        SearchAnalyzer AnalyzerObject(nullptr, Size);
        Output << AnalyzerObject.analyze();
    }
    catch (const std::invalid_argument&)
    {
        Output << "invalid_argument";
    }
    catch (const std::exception&)
    {
        Output << "exception";
    }
}
