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
    BinaryReader(BinaryReader&& other) = delete; // move constructor
    BinaryReader& operator=(const BinaryReader&) = delete;  // copy assignment
    BinaryReader& operator=(BinaryReader&& other) = delete; // move assignment

    // [[nodiscard]] = Tells compiler to issue a warning if the return value is not caught.
    //   My IDE recommended this, and I used it because it's an excellent maintainability feature.
    [[nodiscard]] int* getValues() const;
    [[nodiscard]] int getSize() const;

private:
    void readValues(std::string filename);

    int* Values{nullptr};
    int Size{0};
};