#include "BinaryFile.h"

#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>


/**
 * @brief Creates a binary file containing randomly generated integers using mersenne twister
 *
 * @param filename The name of the binary file to be created.
 */
void createBinaryFile(std::string filename)
{
    constexpr int NumValues{1000};
    
    std::random_device Seed;
    
    std::mt19937 Generator(Seed());
    std::uniform_int_distribution RandomValue(0, 999);
    
    int Values[NumValues]{};
    for (int& Value : Values) // modern way of writing `for(i)` when iterating something with begin() and end() markers
        Value = RandomValue(Generator);
    
    try
    {
        writeBinary(filename, Values, NumValues);
    }
    catch (...)
    {
        std::cerr << "Error writing binary file: " << filename << "\n";
    }
}

/**
 * @brief Writes an array of integers to a binary file.
 *
 * @param filename The name of the binary file to write data to.
 * @param values Pointer to an array of integers to be written to the file.
 * @param length Integers in the array. Can be zero if values == nullptr.
 *
 * @throws std::invalid_argument Length negative, or array null when length > 0
 * @throws std::runtime_error File cannot be opened/created, or writing fails
 */
void writeBinary(std::string filename, int* values, int length)
{
    if (length < 0 || (values == nullptr && length > 0))
        throw std::invalid_argument("invalid_argument");
    
    std::ofstream File(filename, std::ios::binary | std::ios::trunc);
    if (!File)
        throw std::runtime_error("Unable to write: " + filename + "... Can you write to this directory?");
    
    if (!File.is_open())
        throw std::runtime_error("Could not create/open file for writing: " + filename);
    
    constexpr std::streamsize WriteBuffer = sizeof(int);
    
    // ofstream doesn't throw; No try-catch needed.
    File.write(reinterpret_cast<const char*>(&length), WriteBuffer);
    if (length > 0)
        File.write(reinterpret_cast<const char*>(values), length * WriteBuffer);
    
    File.close();
    if (!File)
        throw std::runtime_error("Could not write file: " + filename);
}


// ===============================
// ===== BINARY READER CLASS =====
// ===============================


BinaryReader::BinaryReader(const std::string& filename)
{
    try
    {
        readValues(filename);
    }
    catch (...)
    {
        std::cerr << "Error reading binary file: " << filename << "\n";
    }
}

BinaryReader::~BinaryReader()
{
    delete[] Values;
}

int* BinaryReader::getValues() const
{
    return Values;
}

int BinaryReader::getSize() const
{
    return Size;
}

/**
 * @brief Reads integer values from a binary file and stores them in the reader's internal buffer.
 *
 * @param filename The name of the binary file being read
 *
 * @throws std::runtime_error File cannot be opened for reading, values to read length is negative or zero,
 *                            or if more values were expected than what was read from the file.
 */
void BinaryReader::readValues(std::string filename)
{
    std::ifstream File(filename, std::ios::binary);
    
    if (!File)
        throw std::runtime_error("Could not open file for reading: " + filename);
    
    if (!File.is_open())
        throw std::runtime_error("Could not open file for reading: " + filename);
    
    // ReSharper disable once CommentTypo
    constexpr std::streamsize ReadBuffer = sizeof(int); // `.read()` prefers datatype `streamsize`
    
    int NumValues{0};
    File.read(reinterpret_cast<char*>(&NumValues), ReadBuffer); // doesn't throw
    
    if (NumValues < 0)
        throw std::runtime_error(filename + ": Contains negative or zero values to read.");
    
    auto* NewValues = new int[NumValues];
    File.read(reinterpret_cast<char*>(NewValues), NumValues * ReadBuffer);
    if (!File || File.eof())
    {
        delete[] NewValues;
        throw std::runtime_error(filename + ": Expected more values than what was actually read.");
    }
    
    delete[] Values; // free memory for whatever existed before
    Values = NewValues;
    Size   = NumValues;
}
