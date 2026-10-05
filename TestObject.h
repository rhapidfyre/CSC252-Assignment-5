/*
 * Starcache Studios, LLC. 2026
 * TestObject.h
 *
 * @brief Base class for file-based tests.
 * 
 * Reads input from <testNumber>.in and
 * compares the test's output against <testNumber>.out.
 *
 * Override Execute() to run the code being tested.
 *
 */

// ReSharper disable CppClangTidyClangDiagnosticPadded
#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <istream>
#include <iterator>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using std::cout;
using std::cerr;
using std::size_t;
using std::string;
using std::vector;
using std::scoped_lock;


// ===========================================================================
// This is a recycled test object from other coursework and personal projects.
// You're not expected to understand it or modify it.
// ===========================================================================



enum class TestState : uint8_t
{
	Ready,
	Running,
	Passed,
	Failed,
	Error	// the case couldn't be run at all, as opposed to giving a wrong answer
};

/** Outcome of the most recent run. */
struct TestResult
{
	TestState State = TestState::Ready;

	// 1-based position of the first mismatch, 0 if there wasn't one
	size_t Line = 0;
	size_t Column = 0;

	string Message;
	string Expected;
	string Actual;
};

class TestObject
{
public:
	
	// Constructors
	TestObject(const TestObject&) = delete;
	explicit TestObject(int InTestNumber = 1): TestNumber(InTestNumber)
		, InputFile(PadSingleDigit(InTestNumber) + ".in"), OutputFile(PadSingleDigit(InTestNumber) + ".out") {}

	// Destructors
	virtual ~TestObject() = default;
	
	// Overloads
	TestObject& operator=(const TestObject&) = delete;

	// Execution
	bool Run();
	std::future<bool> RunAsync();

	/**
	 * Runs every .in file in the test directory against the .out file next to it and prints a summary.
	 * Tests are numbered by their position in the sorted file list, not by anything in the filename.
	 * Returns 0 if everything passed, so main() can return it directly.
	 */
	template <typename TestType>
	static int RunAll(const string& TestDirectory = "tests")
	{
		static_assert(std::is_base_of_v<TestObject, TestType>, "TestType must derive from TestObject");
		return RunAllInternal(TestDirectory, [](int Number) -> TestObject* { return new TestType(Number); }, false);
	}

	/** Same as RunAll with one thread per case. Only safe if the cases don't share files, globals or RNG state. */
	template <typename TestType>
	static int RunAllAsync(const string& TestDirectory = "tests")
	{
		static_assert(std::is_base_of_v<TestObject, TestType>, "TestType must derive from TestObject");
		return RunAllInternal(TestDirectory, [](int Number) -> TestObject* { return new TestType(Number); }, true);
	}


	/** Also points the input and output files back at <number>.in and <number>.out. */
	void SetTestNumber(int InTestNumber);
	int GetTestNumber() const;

	TestResult GetResult() const;
	string GetInputFile() const;
	string GetOutputFile() const;

	TestState GetState() const;
	void SetInputFile(const string& Path);
	void SetOutputFile(const string& Path);

	bool IsFinished() const;
	bool HasPassed() const;

	/** Moves on to the next numbered test and throws away the old result. */
	void Next();

protected:
	
	/**
	 * @brief The actual test. Override this for specific test implementations.
	 * @param Input The input stream for the test case.
	 * @param Output The answer for the test case.
	 */
	virtual void Execute(std::istream& Input, std::ostream& Output);

	/**
	 * @brief Decides pass or fail
	 * @param Expected The expected answer for the test case.
	 * @param Actual The actual answer for the test case.
	 * @param OutResult The result of the test.
	 */
	virtual bool Compare(const string& Expected, const string& Actual, TestResult& OutResult);

private:
	
	// Defines a pointer(*) to a function with an integer param (int) that creates a TestObject (TestObject*) instance.
	//  i.e.:	[](int Number) -> TestObject* { return new TestObject(Number); }
	using TestFactory = TestObject* (*)(int);

	static int RunAllInternal(const string& TestDirectory, TestFactory Factory, bool bRunAsync);
	static string PadSingleDigit(int InDigit);
	static string NormalizeNewlines(const string& Text);

	void CheckIdle() const;

	void Begin();
	bool RunTest();

	TestResult	Result;
	string		InputFile;
	string		OutputFile;
	int			TestNumber;

	mutable std::mutex Mutex;
};

inline string TestObject::PadSingleDigit(const int InDigit)
{
	if (InDigit >= 0 && InDigit < 10)
		return "0" + std::to_string(InDigit);
	return std::to_string(InDigit);
}

/**
 * @brief Normalizes newlines in the given text.
 * Windows adds a \r to every \n, so we need to drop the \r of a \r\n pair, a \r on its own stays.
 * Otherwise, the test would fail.
 * 
 * @param Text The text to normalize
 * @return The text with normalized newlines
 */
inline string TestObject::NormalizeNewlines(const string& Text)
{
	string Normalized;
	Normalized.reserve(Text.size());

	for (size_t Index = 0; Index < Text.size(); ++Index)
	{
		// drop the \r of a \r\n pair, a \r on its own stays
		if (Text[Index] == '\r' && Index + 1 < Text.size() && Text[Index + 1] == '\n')
		{
			continue;
		}

		Normalized += Text[Index];
	}

	return Normalized;
}

/**
 * Executes all test cases found in the specified directory, matching `.in` files with
 * their corresponding `.out` files based on filenames, and prints a summary of the results.
 *
 * @param TestDirectory The path to the directory containing the input (`.in`) test files.
 * @param Factory A function to create new instances of `TestObject` for each test case.
 * @param bRunAsync If true, tests are executed asynchronously; otherwise, tests are run sequentially.
 * @return Returns 0 if all tests passed, or 1 if any test failed or encountered an error.
 */
inline int TestObject::RunAllInternal(const string& TestDirectory, TestFactory Factory, bool bRunAsync)
{
	vector<std::filesystem::path> InputFiles;

	try
	{
		for (const std::filesystem::directory_entry& Entry : std::filesystem::directory_iterator(TestDirectory))
		{
			if (Entry.is_regular_file() && Entry.path().extension() == ".in")
			{
				InputFiles.push_back(Entry.path());
			}
		}
	}
	catch (const std::exception& Error)
	{
		cerr << "Test discovery failed: " << Error.what() << '\n';
		return 1;
	}

	if (InputFiles.empty())
	{
		cerr << "No .in test files found in " << TestDirectory << ".\n";
		return 1;
	}

	vector<std::unique_ptr<TestObject>> Tests;
	vector<std::future<bool>> Futures;

	std::ranges::sort(InputFiles);
	Tests.reserve(InputFiles.size());
	Futures.reserve(InputFiles.size());

	try
	{
		for (size_t Index = 0; Index < InputFiles.size(); ++Index)
		{
			std::filesystem::path ExpectedPath = InputFiles[Index];
			ExpectedPath.replace_extension(".out");

			std::unique_ptr<TestObject> Test(Factory(static_cast<int>(Index)));
			Test->SetInputFile(InputFiles[Index].string());
			Test->SetOutputFile(ExpectedPath.string());
			Tests.push_back(std::move(Test));

			if (bRunAsync)
			{
				Futures.push_back(Tests.back()->RunAsync());
			}
		}
	}
	catch (const std::exception& Error)
	{
		cerr << "Test setup failed: " << Error.what() << '\n';
		return 1;
	}

	size_t NumPassed = 0;
	size_t NumFailed = 0;
	size_t NumErrors = 0;

	for (size_t Index = 0; Index < Tests.size(); ++Index)
	{
		const string Name = InputFiles[Index].string();
		bool bPassed;

		try
		{
			bPassed = bRunAsync ? Futures[Index].get() : Tests[Index]->Run();
		}
		catch (const std::exception& Error)
		{
			cerr << "[ERROR] " << Name << ": " << Error.what() << '\n';
			NumErrors++;
			continue;
		}
		catch (...)
		{
			cerr << "[ERROR] " << Name << ": unknown exception.\n";
			NumErrors++;
			continue;
		}

		const TestResult TestResult = Tests[Index]->GetResult();

		if (bPassed)
		{
			cout << "[PASS]  " << Name << '\n';
			NumPassed++;
		}
		else if (TestResult.State == TestState::Error)
		{
			cout << "[ERROR] " << Name << ": " << TestResult.Message << '\n';
			NumErrors++;
		}
		else
		{
			cout << "[FAIL]  " << Name << ": " << TestResult.Message << '\n';
			NumFailed++;
		}
	}

	cout << InputFiles.size() << " tests executed: " << NumPassed << " passed, "
			  << NumFailed << " failed, " << NumErrors << " errors.\n";

	return NumFailed == 0 && NumErrors == 0 ? 0 : 1;
}

/**
 * Initiates the asynchronous execution of the test case.
 * Sets up the initial state and launches the test on a separate thread.
 *
 * @return A future object representing the result of the test execution. The future will hold a boolean value,
 *         where true indicates the test passed, and false indicates it failed.
 * @throws Any exception that occurs during the attempt to start the test thread.
 */
inline std::future<bool> TestObject::RunAsync()
{
	// has to happen here and not on the worker, otherwise GetState() could still say Ready after we return
	Begin();

	try
	{
		return std::async(std::launch::async, [this]() { return RunTest(); });
	}
	
	catch (...) // catches all errors and exceptions regardless of type
	{
		scoped_lock Lock(Mutex);
		Result.State = TestState::Error;
		Result.Message = "Could not start the test thread.";
		throw;
	}
}

inline bool TestObject::Run()
{
	Begin();
	return RunTest();
}

inline int TestObject::GetTestNumber() const
{
	scoped_lock Lock(Mutex);
	return TestNumber;
}

inline void TestObject::SetTestNumber(const int InTestNumber)
{
	scoped_lock Lock(Mutex);
	CheckIdle();

	const string BaseName = PadSingleDigit(InTestNumber);

	TestNumber = InTestNumber;
	InputFile  = BaseName + ".in";
	OutputFile = BaseName + ".out";
	Result = {};
}

inline void TestObject::Next()
{
	scoped_lock Lock(Mutex);
	CheckIdle();

	TestNumber++;

	// goes back to the numbered files even if the last test had custom paths set
	const string BaseName = PadSingleDigit(TestNumber);
	InputFile = BaseName + ".in";
	OutputFile = BaseName + ".out";
	Result = {};
}

inline void TestObject::Execute(std::istream& Input, std::ostream& Output)
{
}

inline TestResult TestObject::GetResult() const
{
	scoped_lock Lock(Mutex);
	return Result;
}

inline string TestObject::GetInputFile() const
{
	scoped_lock Lock(Mutex);
	return InputFile;
}

inline string TestObject::GetOutputFile() const
{
	scoped_lock Lock(Mutex);
	return OutputFile;
}

inline TestState TestObject::GetState() const
{
	scoped_lock Lock(Mutex);
	return Result.State;
}

inline void TestObject::SetInputFile(const string& Path)
{
	scoped_lock Lock(Mutex);
	CheckIdle();

	InputFile = Path;
	Result = {};
}

inline void TestObject::SetOutputFile(const string& Path)
{
	scoped_lock Lock(Mutex);
	CheckIdle();

	OutputFile = Path;
	Result = {};
}

inline bool TestObject::IsFinished() const
{
	const TestState State = GetState();
	return State != TestState::Ready && State != TestState::Running;
}

inline bool TestObject::HasPassed() const
{
	return GetState() == TestState::Passed;
}

inline bool TestObject::Compare(const string& Expected, const string& Actual, TestResult& OutResult)
{
	const string Left  = NormalizeNewlines(Expected);
	const string Right = NormalizeNewlines(Actual);

	if (Left == Right)
	{
		OutResult.Message = "Output matched.";
		return true;
	}

	// walk the common prefix to find where they split
	OutResult.Line = 1;
	OutResult.Column = 1;

	// Using lambda to make the loop condition more readable
	const auto IsWitinBounds = [&Left, &Right](const size_t Index)
	{
		return Index < Left.size() && Index < Right.size();
	};
	
	for (size_t Index = 0; IsWitinBounds(Index) && Left[Index] == Right[Index]; Index++)
	{
		if (Left[Index] == '\n')
		{
			OutResult.Line++;
			OutResult.Column = 1;
		}
		else
			OutResult.Column++;
	}

	OutResult.Message = "Output differs at line " + std::to_string(OutResult.Line)
					  + ", column " + std::to_string(OutResult.Column) + ".";

	return false;
}

inline void TestObject::CheckIdle() const
{
	if (Result.State == TestState::Running)
		throw std::logic_error("Test is already running.");
}

inline void TestObject::Begin()
{
	// Scoped lock releases the lock after the scope is lost
	scoped_lock Lock(Mutex);
	CheckIdle();

	Result = {};
	Result.State = TestState::Running;
}

inline bool TestObject::RunTest()
{
	// Filled in locally and swapped in at the end, so GetResult() never returns a half written result.
	TestResult NewResult;

	try
	{
		std::ifstream Input(InputFile);
		if (!Input)
			throw std::runtime_error("Could not open input file: " + InputFile);

		// binary so the text comes through untouched, Compare deals with the line endings
		std::ifstream ExpectedFile(OutputFile, std::ios::binary);
		if (!ExpectedFile)
			throw std::runtime_error("Could not open expected output file: " + OutputFile);

		NewResult.Expected.assign(std::istreambuf_iterator<char>(ExpectedFile), std::istreambuf_iterator<char>());
		if (ExpectedFile.bad())
			throw std::runtime_error("Could not read expected output file: " + OutputFile);

		std::ostringstream Output;
		Execute(Input, Output);
		NewResult.Actual = Output.str();

		if (Input.bad() || !Output)
			throw std::runtime_error("An input/output stream error occurred.");

		const bool bMatched = Compare(NewResult.Expected, NewResult.Actual, NewResult);
		NewResult.State = bMatched ? TestState::Passed : TestState::Failed;
	}
	catch (const std::exception& Error)
	{
		NewResult.State = TestState::Error;
		NewResult.Message = Error.what();
	}
	catch (...)
	{
		NewResult.State = TestState::Error;
		NewResult.Message = "An unknown exception occurred.";
	}

	const bool bPassed = NewResult.State == TestState::Passed;
	scoped_lock Lock(Mutex);
	Result = std::move(NewResult);
	return bPassed;
}
