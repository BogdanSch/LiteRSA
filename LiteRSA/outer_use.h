#pragma once

#ifdef LITERSA_EXPORTS
#define LITERSA_API extern "C" _declspec(dllexport)
#else
#define LITERSA_API extern "C" _declspec(dllimport)
#endif

struct Pair {
	uint32_t first;
	uint32_t second;
};
LITERSA_API uint64_t Encrypt(uint64_t input, uint64_t n, uint64_t e);
LITERSA_API uint64_t Decrypt(uint64_t input, uint64_t n, uint64_t d);
LITERSA_API Pair GeneratePrimeFactors();
LITERSA_API uint64_t GetPrimeFactorsProduct(Pair pair);
LITERSA_API uint64_t GeneratePublicKey(Pair pair, uint64_t n);
LITERSA_API uint64_t GeneratePrivateKey(Pair pair, uint64_t e);