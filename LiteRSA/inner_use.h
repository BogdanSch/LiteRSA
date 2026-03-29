#pragma once
#include <bitset>

using std::bitset;

constexpr size_t GRID_SIZE = 64;
constexpr size_t MIN_GENERATION_RANGE = 3;
constexpr size_t MAX_GENERATION_RANGE = 50000;

extern "C" {
    uint32_t GetRandomPrimeNumber();
    bool IsPrime(uint32_t);
    uint64_t GetGCD(uint64_t, uint64_t);
    int FindIndexOfFirstSignificantOne(bitset<GRID_SIZE>);
    uint64_t CalculatePhi(uint32_t, uint32_t);
}