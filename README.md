# Assignment 5 Requirements and Development Plan

Due Saturday, October 10, 2026 at 11:59 PM.

Plan to submit before Saturday evening so we have time to handle any submission issues.

## Purpose

Develop a program that reads and analyzes binary data using selection sort, recursive binary search, and the required analyzer classes.

We'll divide the instructions into requirements, develop tests for each portion, and use those tests during implementation and code review. The final submission will include a Visual Studio project and pseudocode document. Each member will also submit their own peer evaluation.

## Source

The assignment document, which serves as the requirements source, is included in the repository.

Use the function and class declarations provided with the assignment. Any requirements that aren't clear will be discussed before implementation. We also need the Part4 output to confirm the final output format.

## Coding convention

- Declarations shall use function and class declarations required by the assignment.
- Index-returning functions shall use -1 for “not found.” Index 0 is a valid index.
- Count-returning functions shall use 0 for no results.
- Boolean functions shall use true and false.
- Use standard exceptions (throw) for invalid input or operational errors.
- Expected outcomes, such as “not found,” shall not throw.
- Catch exceptions where they can be handled or reported, including the program entry point.
- Lines should be kept within a 120 character limit
- Single statements (return, break, continue) should be on their own lines.
- Ternaries shall be kept simple and not contain nested conditions.
- Each function shall be given a brief header comment describing its purpose, parameters, return value, and any exceptions it throws.

## Functional requirements

### Selection sort

| ID | Requirement | Acceptance |
|---|---|---|
| SORT-01 | Define selection_sort with an integer pointer and integer size | Function is defined with the required parameters |
| SORT-02 | Implement a custom selection sort algorithm | Code uses selection sort to sort an unordered list of integers without using a C++ library |
| SORT-03 | Sort ascending in place | The original array is sorted in ascending order and contains all original values, including duplicates |
| SORT-04 | Handle duplicate entries | Duplicate entries are properly sorted |
| SORT-05 | Handle already sorted input | A list already sorted remains sorted |
| SORT-06 | Handle reverse input | Output is correctly sorted in ascending order |
| SORT-07 | Handle single element input | The array retains its single value |

Selection sort shall use ascending order so that the statistics and binary search portions use the same sort order.

If course discussions establish a different requirement, we'll update the affected requirements and tests before making that change.

### Recursive binary search

| ID | Requirement | Acceptance |
|---|---|---|
| SEARCH-01 | Define binary_search with an integer array, search value, and array size | Function matches the supplied declaration, parameters in listed order |
| SEARCH-02 | Define binary_search_recursive with an integer pointer, search value, starting index, and ending index | Function matches the supplied declaration |
| SEARCH-03 | Call binary_search_recursive from binary_search | Array, search value, starting index, and last index |
| SEARCH-04 | Recursive binary search algorithm | Each call searches a smaller portion of the sorted array |
| SEARCH-05 | Stop when value found or search range is empty | Search returns the correct result, and does not continue indefinitely |
| SEARCH-06 | Find value within array | Searching first, last, and middle by index returns expected result |
| SEARCH-07 | Search non-existant value | Search returns no result |
| SEARCH-08 | Handle duplicate values | Search identifies a matching value; finding the first occurrence is not required |
| SEARCH-09 | Handle single element arrays | Single-element array returns anticipated results |

Binary search requires a sorted array. For development, use a sorted array to create and test Recursive Binary Search.

### Analyzer

| ID | Requirement | Acceptance |
|---|---|---|
| AN-01 | Define Analyzer class | An Analyzer class used to call analyzer methods |
| AN-02 | Overridable analyze() method | A default implementation of a method to be overriden by derived classes |


### SearchAnalyzer

| ID | Requirement | Acceptance |
|---|---|---|
| SA-01 | Define SearchAnalyzer as a public subclass of Analyzer | Class inherits from Analyzer and can be used through an Analyzer pointer |
| SA-02 | Sort the data in the constructor | Constructor calls selection_sort with the analysis array and size after base initialization |
| SA-03 | Override analyze | Method matches the base declaration and uses the override keyword |
| SA-04 | Generate 100 random integers during each analyze call | Exactly 100 search values are generated |
| SA-05 | Generate values from 0 to 999 | Every generated value is within the range, including both endpoints |
| SA-06 | Search for each generated value | Each value is passed to binary_search |
| SA-07 | Count successful searches | Count increases once for each search that finds a value |
| SA-08 | Return the count as a std::string | Returned string represents the count and matches the required output format |
| SA-09 | Reset the count for each analyze call | Repeated calls do not include counts from earlier calls |

Repeated search values count separately. If the same value is generated five times and exists in the data, it contributes five to the count.

The returned count must be between 0 and 100. We'll test this using data that guarantees all searches succeed and data that guarantees none succeed.

### StatisticsAnalyzer

Blocked by SORT-01, AN-01, & SA-01

| ID | Requirement | Acceptance |
|---|---|---|
| STATS-01 | Sort the data in analyze | Method calls selection_sort before calculating the statistics |
| STATS-02 | Find the minimum using the sorted array | Minimum is the first value in a nonempty array |
| STATS-03 | Find the maximum using the sorted array | Maximum is the last value in a nonempty array |
| STATS-04 | Calculate the median for an odd number of elements | Median is the middle value |
| STATS-05 | Calculate the median for an even number of elements | Median is the mean of the two middle values |
| STATS-06 | Calculate the mode | Mode is the value that occurs most frequently |
| STATS-07 | Handle multiple values tied for mode | First value in the sorted array with the highest frequency is selected |
| STATS-08 | Handle a most frequent value at the end of the array | Final group of repeated values is included in the mode calculation |
| STATS-09 | Preserve the existing statistics | Previously required calculations and output remain correct |
| STATS-10 | Override analyze | Method matches the base declaration and uses the override keyword |
| STATS-11 | Doesn't Read Outside Array | Does not allow accessing indicies outside the bounds of the array |
| STATS-12 | Prevents #DIV/0 | Does not allow division by zero |

C++ truncates integer math by default. Allow it to do so.

## Quality and submission requirements

| ID | Requirement | Acceptance |
|---|---|---|
| QA-01 | Compile without errors or warnings | Clean build with no errors or warnings |
| QA-02 | Test Cases | Additional tests to cover unanticipated or overlooked edge cases |
| QA-03 | Complete program | Binary file creation, reading, analysis, and output work together |
| QA-04 | Match the Part4 output format | Labels and output order match |
| QA-05 | Complete the pseudocode | Explains the submitted program and matches the implementation |
| REL-01 | List the other group members | Submission includes the names of the other members |
| REL-02 | One ZIP containing the project and pseudocode | Both are included, along with any files needed to build and run the project |
| REL-04 | Name the ZIP Properly | Uses the required naming convention |
| REL-05 | Each member submits a peer evaluation | Each member completes and submits their own form |

QA & release requirements must be verified with Visual Studio regardless of IDE used.

## Proposed workload distribution

| Owner | Implementation | Additional responsibilities |
|---|---|---|
| Teammate A | selection_sort and StatisticsAnalyzer | Tests for their portion, pseudocode, and review of the search portion |
| Teammate B | binary_search, binary_search_recursive, and SearchAnalyzer | Tests for their portion, pseudocode, and review of the sorting and statistics portion |
| Melanie | Test driver and program integration | Project owner, requirements, setup, coordination, code review, document assembly, and submission checks |

## Test plan

I have developed a test object previously for other assignments and classes. I will recycle this into our project, and revise it for testing our project.

## Development workflow

1. Write the test cases and expected results.
2. Implement the required behavior
3. Add edge case tests as needed
4. Make any needed corrections
5. Push commits/Pull Request for each requirement as they are being met
6. Review PRs and merge on an ongoing basis.
7. Pass remaining tests and ensure completion after merging

If everyone is familiar with git, work on a branch and merge to main.

If you are unfamiliar with git, work on the `main` branch.

The protected branch will be `release`, which will be used for submission.

During code review, check the algorithm itself as well as the output. Selection sort must use the required algorithm, and binary search must use recursion. Also check array access, recursion stopping conditions, search results at index 0, fractional medians, mode ties, data ownership, overrides, and random-search counts.

## Development schedule

All times below are Pacific time.

| Date | Work | Completion target |
|---|---|---|
| 04 Oct | Planning & Design | Requirements are ready for group review |
| 05 Oct | Prepare Tests & Repository | Project builds, tests run (and fail accordingly) |
| 06 Oct | Implement Algorithms & Basic Program | Core functions become available |
| 07 Oct | Continue Development | Core functionality |
| 08 Oct | Finish development and implementation | Core functionality is complete and tests are becoming successful |
| 09 Oct | Debugging & Review | Project is being finalized for submission |
| 10 Oct | Build, Release, & Submit | Peer evaluations and final submission before 1159 PM PST |

Required functionality takes priority over this schedule and plan.

## Pseudocode

Each member will provide pseudocode for their portion. The final pseudocode will be created to cover:

- Selection sort
- recursive binary search
- SearchAnalyzer
- StatisticsAnalyzer, including median and mode
- Any additional portions required
- Each group member who participated

## Final checklist

- [ ] Confirm the assignment number and deadline
- [ ] Confirm the group members and workload distribution.
- [ ] Agree on program behavior.
- [ ] Complete selection sort and tests pass
- [ ] Complete the binary search helper, recursive function, and tests
- [ ] Complete SearchAnalyzer and its tests
- [ ] Complete StatisticsAnalyzer changes and tests
- [ ] Verify binary file reading and writing and the existing analyzers
- [ ] Complete code review and resolve remaining issues
- [ ] Build in Visual Studio with no errors or warnings
- [ ] Confirm the complete output matches the required format
- [ ] Complete the pseudocode document
- [ ] Create the ZIP with the project, code files, and pseudocode
- [ ] Extract the ZIP into a new directory and verify it builds and runs
