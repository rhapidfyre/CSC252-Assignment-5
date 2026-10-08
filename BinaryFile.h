#pragma once

#include <string>

// Specific declarations called for in the assignment.
void createBinaryFile(std::string filename);
void writeBinary(std::string filename, int* values, int length);

// ReSharper disable CppClangTidyClangDiagnosticPadded
class BinaryReader
{
public:
    explicit BinaryReader(const std::string& filename);
    ~BinaryReader();

    // Prevents double free (as explained in Analyzer.h)
    BinaryReader(const BinaryReader&) = delete;  // copy constructor
    BinaryReader& operator=(const BinaryReader&) = delete;  // copy assignment
    
    int* getValues() const;
    int getSize() const;

private:
    void readValues(std::string filename);

    int* Values{nullptr};
    int Size{0};
};