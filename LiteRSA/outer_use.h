#pragma once
#include <tuple>

using std::tuple;

#ifdef LITERSA_EXPORTS
#define LITERSA_API extern "C" _declspec(dllexport)
#else
#define LITERSA_API extern "C" _declspec(dllimport)
#endif


LITERSA_API struct Pair {
	uint32_t first;
	uint32_t second;
};
LITERSA_API uint64_t Encode(int input, uint64_t n, uint64_t e);
LITERSA_API uint64_t Decode(uint64_t input, uint64_t n, uint64_t d);
LITERSA_API Pair GeneratePrimeFactors();
LITERSA_API uint64_t GetPrimeFactorsProduct(uint32_t p, uint32_t q);
LITERSA_API uint64_t GeneratePublicKey(uint32_t p, uint32_t q, uint64_t n);
LITERSA_API uint64_t GeneratePrivateKey(uint32_t p, uint32_t q, uint64_t e);