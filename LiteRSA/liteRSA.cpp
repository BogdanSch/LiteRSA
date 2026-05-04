#include "pch.h"
#include "inner_use.h"
#include "outer_use.h"
#include <time.h>
#include <bitset>
#include <random>
#include <climits>
#include <tchar.h>

using std::bitset, std::random_device, std::mt19937, std::uniform_int_distribution;

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
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<> distribution(MIN_GENERATION_RANGE, MAX_GENERATION_RANGE);

	uint32_t result = distribution(generator);
	while (!IsPrime(result)) {
		result++;
	}

	return result;
}

int FindIndexOfFirstSignificantOne(bitset<GRID_SIZE> binaryNumber)
{
	for (int i = binaryNumber.size() - 1; i >= 0; i--) {
		if (binaryNumber[i] == 1) return i;
	}
	return -1;
}

uint64_t CalculatePhi(Pair pair) {
	return (static_cast<uint64_t>(pair.first) - 1) * (static_cast<uint64_t>(pair.second) - 1);
}

DWORD CalculateCRC(BYTE* mem, size_t size) {
	DWORD* mem32 = (DWORD*)mem, crc = 0xFFFFFFFF;
	size_t count = size / sizeof(DWORD);
	size_t not_full_count = size % sizeof(DWORD);
	for (size_t i = 0; i < count; ++i)
		crc = _mm_crc32_u32(crc, mem32[i]);
	if (not_full_count) {
		DWORD value = 0; PBYTE mem8 = (PBYTE)(mem32 + count);
		for (size_t i = 0; i < not_full_count; ++i) {
			value |= (DWORD)mem8[i] << 8 * i;
		}
		crc = _mm_crc32_u32(crc, value);
	}
	crc = ~crc;
	return crc;
}

BOOL CheckCB(LPCTSTR fn) {
	HANDLE h = CreateFile(fn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		printf("Error when reading: %d\n", err);
		return FALSE;
	}

	BOOL b = FALSE;
	DWORD size = GetFileSize(h, NULL);
	if (size > 8) {
		PBYTE mem8 = new BYTE[size];
		if (mem8) {
			DWORD dwBytesRead = 0;
			BOOL successful = ReadFile(h, mem8, size, &dwBytesRead, NULL);

			if (successful && *(DWORD*)(mem8 + size - 4) == 0xFFFFFFFF)
			{
				DWORD cb = CalculateCRC(mem8, size - 8);
				b = cb == *(DWORD*)(mem8 + size - 8);
			}
			free(mem8);
		}
	}

	CloseHandle(h);
	return b;
}

LITERSA_API Pair GeneratePrimeFactors() {
	uint32_t p = GetRandomPrimeNumber();
	uint32_t q = p;
	while (q == p) {
		q = GetRandomPrimeNumber();
	}

	return { p, q };
}

LITERSA_API uint64_t GetPrimeFactorsProduct(Pair pair) {
	return static_cast<uint64_t>(pair.first) * pair.second;
}

LITERSA_API uint64_t GeneratePublicKey(Pair pair, uint64_t n) {
	uint64_t phi = CalculatePhi(pair);

	uint64_t e = 3;
	while (true) {
		if (GetGCD(e, phi) == 1) break;
		e += 2;
	}
	return e;
}

LITERSA_API uint64_t GeneratePrivateKey(Pair pair, uint64_t e) {
	uint64_t phi = CalculatePhi(pair);

	uint64_t d = 3;
	while (true) {
		if ((d * e) % phi == 1) break;
		d += 2;
	}
	return d;
}

LITERSA_API uint64_t Encrypt(uint64_t input, uint64_t n, uint64_t e) {
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

LITERSA_API uint64_t Decrypt(uint64_t input, uint64_t n, uint64_t d) {
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