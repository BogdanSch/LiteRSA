#pragma once
#include <bitset>
#include "outer_use.h"

using std::bitset;

//constexpr size_t MAX_PATH = 256;
constexpr size_t GRID_SIZE = 64;
constexpr size_t MIN_GENERATION_RANGE = 50000;
constexpr size_t MAX_GENERATION_RANGE = 70000;

extern "C" {
    DWORD CalculateCRC(BYTE* mem, size_t size);
    BOOL CheckCB(LPCTSTR fn);
    uint32_t GetRandomPrimeNumber();
    bool IsPrime(uint32_t);
    uint64_t GetGCD(uint64_t, uint64_t);
    int FindIndexOfFirstSignificantOne(bitset<GRID_SIZE>);
    uint64_t CalculatePhi(Pair);
}