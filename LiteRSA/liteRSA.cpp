#include "pch.h"
#include "inner_use.h"
#include "outer_use.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <bitset>
#include <random>
#include <climits>

using std::bitset;
using std::default_random_engine;
using std::uniform_int_distribution;

uint64_t GetGCD(uint64_t a, uint64_t b) {
	if (b == 0) return a;
	return GetGCD(b, a % b);
}

bool IsPrime(uint32_t n) {
	if (n <= 1) return false;
	for (uint32_t i = 2; i * i <= n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}

uint32_t GetRandomPrimeNumber() {
	default_random_engine generator(time(0));
	uniform_int_distribution<uint32_t> distribution(MIN_RANDOM_PRIME_NUMBER, 100000);

	uint32_t result = distribution(generator);

	while (!IsPrime(result)) {
		result++;
	}

	return result;
}

int FindIndexOfFirstSignificantOne(bitset<GRID_SIZE> binaryNumber)
{
	for(int i = binaryNumber.size() - 1; i >= 0; i--) {
		if (binaryNumber[i] == 1) return i;
	}
	return -1;
}

LITERSA_API Pair GeneratePrimeFactors() {
	uint32_t p = GetRandomPrimeNumber();
	uint32_t q = p;
	while (q == p) {
		q = GetRandomPrimeNumber();
	}

	return {p, q};
}

LITERSA_API uint64_t GetPrimeFactorsProduct(uint32_t p, uint32_t q) {
	return static_cast<uint64_t>(p) * q;
}

LITERSA_API uint64_t GeneratePublicKey(uint32_t p, uint32_t q, uint64_t n) {
	uint64_t phi = (static_cast<uint64_t>(p) - 1) * (static_cast<unsigned long long>(q) - 1);

	uint64_t e = 3;
	while (true) {
		if (GetGCD(e, phi) == 1) break;
		e += 2;
	}
	return e;
}

LITERSA_API uint64_t GeneratePrivateKey(uint32_t p, uint32_t q, uint64_t e) {
	uint64_t phi = (static_cast<uint64_t>(p) - 1) * (static_cast<unsigned long long>(q) - 1);

	uint64_t d = 3;
	while (true) {
		if ((d * e) % phi == 1) break;
		d += 2;
	}
	return d;
}

LITERSA_API uint64_t Encode(int input, uint64_t n, uint64_t e) {
	bitset<GRID_SIZE> binaryE(e);
	int firstSignificantIndexOfOne = FindIndexOfFirstSignificantOne(binaryE);
	if (firstSignificantIndexOfOne == -1) return -1;

	uint64_t encodedData = input;
	for (int i = firstSignificantIndexOfOne - 1; i >= 0; i--) {
		encodedData = (encodedData * encodedData) % n;

		if (binaryE[i] == 1) {
			encodedData = encodedData * input % n;
		}
	}

	return encodedData;
}

LITERSA_API uint64_t Decode(uint64_t input, uint64_t n, uint64_t d) {
	bitset<GRID_SIZE> binaryD(d);
	int firstSignificantIndexOfOne = FindIndexOfFirstSignificantOne(binaryD);
	if (firstSignificantIndexOfOne == -1) return -1;

	uint64_t decodedData = input;
	for (int i = firstSignificantIndexOfOne - 1; i >= 0; i--) {
		decodedData = (decodedData * decodedData) % n;

		if (binaryD[i] == 1) {
			decodedData = decodedData * input % n;
		}
	}

	return decodedData;
}