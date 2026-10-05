/**
 * @file    CSC252-Assignment-5.cpp
 * @authors Melanie Harris, ...
 * @date    05 OCT 2026
 * @version 0.1
 * @brief   Implementation and main driver for the Statistical Analysis assignment.
 * 
 * @license MIT
 */

#include "SelectionSort.h"
using CSC252::SelectionSort;
using CSC252::SelectionSortTest;
using std::cout;
using std::endl;

namespace Helpers
{
    static void print_console(const string& message) {cout << message << "\n";}
}

namespace
{
    const vector<string> TestDescriptions =
    {
        "Shuffle Test", "Shuffled Permutation", "Empty Array", "Single Element", "Already Sorted",
        "Reverse Sorted", "Same Value", "Mixed Duplicates", "Negatives & Extremes", "Two Elements"
    };
    
    string TestDescription(const size_t TestNumber)
    {
        std::ostringstream TestLabel;
        TestLabel << "Test " << std::setw(2) << std::setfill('0')
                  << TestNumber << " (" << TestDescriptions[TestNumber] << ")";
        return TestLabel.str();
    }
    
    string FilePath(const size_t TestNumber, const string& FileExtension)
    {
        std::ostringstream TestPath;
        TestPath << "tests/" << std::setw(2) << std::setfill('0') 
                 << TestDescription(TestNumber) << FileExtension;
        return TestPath.str();
    }
}

int main()
{
    Helpers::print_console("Beginning Selection Sort Tests...");
    
    vector<std::unique_ptr<SelectionSortTest>> SelectionSortTests;
    vector<std::future<bool>> SelectionSortTestFutures;
    
    for (size_t TestNumber = 0; TestNumber < TestDescriptions.size(); TestNumber++)
    {
        SelectionSortTests.push_back(std::make_unique<SelectionSortTest>(static_cast<int>(TestNumber)));
        SelectionSortTests.back()->SetInputFile(FilePath(TestNumber, ".in"));
        SelectionSortTests.back()->SetOutputFile(FilePath(TestNumber, ".out"));
        SelectionSortTestFutures.push_back(SelectionSortTests.back()->RunAsync());
    }
    
    size_t TestsFailed = 0;
    
    // Waits for each test to complete and then prints their result.
    for (size_t TestNumber = 0; TestNumber < SelectionSortTests.size(); TestNumber++)
    {
        SelectionSortTestFutures[TestNumber].get();
        const TestResult Result = SelectionSortTests[TestNumber]->GetResult();
        
        if (Result.State == TestState::Passed)
        {
            Helpers::print_console(TestDescription(TestNumber) + ": Passed.");
            continue; // Move to next test
        }
        
        TestsFailed++;
        if (Result.State == TestState::Error)
        {
            Helpers::print_console(TestDescription(TestNumber) + ": Errored.");
            continue;
        }

        Helpers::print_console(TestDescription(TestNumber) + ": Failed.");
    }
    
    Helpers::print_console("Selection Sort Tests Complete.");
    
}

