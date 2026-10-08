#pragma once

#include "TestObject.h"

// Headers: This defines them so other files can reach them, without specifying their implementation.
//          Implement them in Analyzer.cpp
class Analyzer
{
public:
    Analyzer();
    explicit Analyzer(int* InValues, int InSize);
    virtual ~Analyzer();

    // `Analyzer NewAnalyzer = OldAnalyzer` would create a 2nd object pointing to the same array.
    // So this would cause a double free when destructed. Deleting this prevents that from occurring.
    //  Child classes don't need to implement this; This deletion is inherited.
    Analyzer(const Analyzer&) = delete; // Prevents copy constructor, which effectively uses the below assign
    Analyzer& operator=(const Analyzer&) = delete; // Prevents assignment operator, stopping pointer assingment

    int* cloneValues(int* values, int size);
    virtual std::string analyze() = 0; // requires implementation

protected:
    int* Values;
    int Size;
};

class DuplicatesAnalyser : public Analyzer
{
public:
    DuplicatesAnalyser(int* values, int size) : Analyzer(values, size) {}
    std::string analyze() override;
};

class MissingAnalyser : public Analyzer
{
public:
    MissingAnalyser(int* values, int size) : Analyzer(values, size) {}
    std::string analyze() override;
};

class SearchAnalyzer : public Analyzer
{
public:
    SearchAnalyzer(int* values, int size);
    std::string analyze() override;
};

class StatisticsAnalyzer : public Analyzer
{
public:
    StatisticsAnalyzer(int* values, int size) : Analyzer(values, size) {}
    std::string analyze() override;
};


// ===========================================================================
// Testing implementation below. Write analyzer logic above these lines.
// ===========================================================================

namespace CSC252
{
    /**
     * @class StatisticsAnalyzerTest
     * @brief A test class for validating StatisticsAnalyzer::analyze.
     *
     * Reads one test case from a .in file and writes whatever analyze() returns. A case is a list of integers
     * (any order, may be empty), with an optional mode letter in front:
     *
     *   <values...>      analyze() over the values. An exception here is unexpected, so the test reports an error.
     *   e <size>         the only case that expects an exception: construct with a null array and the declared
     *                    size. Writes "invalid_argument" or "exception" if the constructor threw, otherwise
     *                    whatever analyze() returns.
     */
    class StatisticsAnalyzerTest : public TestObject
    {
    public:
        explicit StatisticsAnalyzerTest(const int TestNumber) : TestObject(TestNumber) {}
        ~StatisticsAnalyzerTest() override;
    protected:
        void Execute(std::istream& Input, std::ostream& Output) override;
        bool Compare(const string& Expected, const string& Actual, TestResult& OutResult) override;
    private:
        void ExpectsThrowCase(std::istream& Input, std::ostream& Output);
    };

    /**
     * @class SearchAnalyzerTest
     * @brief A test class for validating SearchAnalyzer::analyze.
     *
     * The analyzer searches for random values, so every case uses data where the answer is guaranteed.
     * A case starts with a mode word, and each mode writes what analyze() returns:
     *
     *   all              data holds every number 0-999, so every search is found (100 of 100).
     *   none <values...> data outside 0-999 (or empty), so no search is found (0 of 100).
     *   reset            same data as "all". Two calls in a row, one result per line.
     *   null <size>      construct with a null array and the declared size, then writes analyze().
     *
     * Only "null" expects an exception, and it writes "invalid_argument" or "exception" if the constructor threw.
     * An exception in any other mode is unexpected, so the test reports an error.
     */
    class SearchAnalyzerTest : public TestObject
    {
    public:
        explicit SearchAnalyzerTest(const int TestNumber) : TestObject(TestNumber) {}
        ~SearchAnalyzerTest() override;
    protected:
        void Execute(std::istream& Input, std::ostream& Output) override;
        bool Compare(const string& Expected, const string& Actual, TestResult& OutResult) override;
    private:
        void ExpectsThrowCase(std::istream& Input, std::ostream& Output);
    };
}
