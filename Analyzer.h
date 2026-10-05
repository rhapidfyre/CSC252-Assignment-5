#pragma once

class Analyzer
{
public:
    Analyzer() {}
    virtual ~Analyzer() = default;
};

class SearchAnalyzer : public Analyzer
{
public:
    SearchAnalyzer() {}
    virtual ~SearchAnalyzer() = default;
};

class StatisticsAnalyzer : public Analyzer
{
public:
    StatisticsAnalyzer() {}
    virtual ~StatisticsAnalyzer() = default;
};


// ===========================================================================
// Testing implementation below. Write binary search logic above these lines.
// ===========================================================================

namespace CSC525
{

}