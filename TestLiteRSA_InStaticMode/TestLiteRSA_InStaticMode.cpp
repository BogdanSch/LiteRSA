#include <iostream>
#include <cassert>
#include <vector>
#include "..\LiteRSA\outer_use.h"

using std::cout, std::string, std::vector;

int main()
{
	Pair primeFactors0 = GeneratePrimeFactors();
	uint64_t n0 = GetPrimeFactorsProduct(primeFactors0);
	uint64_t e0 = GeneratePublicKey(primeFactors0, n0);
	uint64_t d0 = GeneratePrivateKey(primeFactors0, e0);

	Pair primeFactors1 = GeneratePrimeFactors();
	uint64_t n1 = GetPrimeFactorsProduct(primeFactors1);
	uint64_t e1 = GeneratePublicKey(primeFactors1, n1);	
	uint64_t d1 = GeneratePrivateKey(primeFactors1, e1);

	string data = "This is a secret message. NO ONE should be able to read it.";
	for(char symbol : data) {
		uint64_t e1t = Encrypt(symbol, n1, e1);
		uint64_t d1e1t = Decrypt(e1t, n1, d1);
		assert((uint64_t) symbol == d1e1t);

		uint64_t e0d1e1t = Encrypt(d1e1t, n0, e0);
		uint64_t d0e0d1e1t = Decrypt(e0d1e1t, n0, d0);
		assert((uint64_t) symbol == d0e0d1e1t);
	}

	vector<uint64_t> numbersToTest = { 0, 1, 42, 12345, 65535, 10000000, 1000000000 };
	for(uint64_t number : numbersToTest) {
		uint64_t e1d0e0t = Encrypt(number, n1, e1);
		uint64_t d1e1d0e0t = Decrypt(e1d0e0t, n1, d1);
		assert((uint64_t) number == d1e1d0e0t);

		uint64_t e0t = Encrypt(d1e1d0e0t, n0, e0);
		uint64_t d0e0t = Decrypt(e0t, n0, d0);
		assert((uint64_t) number == d0e0t);
	}

	cout << "All tests passed successfully!\n";
}