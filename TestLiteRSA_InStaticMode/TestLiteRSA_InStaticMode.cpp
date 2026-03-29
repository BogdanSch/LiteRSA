#include <iostream>
#include <vector>
#include <cassert>
#include "..\LiteRSA\outer_use.h"

using std::cout;
using std::string;
using std::vector;

int main()
{
	Pair primeFactors0 = GeneratePrimeFactors();
	uint64_t n0 = GetPrimeFactorsProduct(primeFactors0.first, primeFactors0.second);
	uint64_t e0 = GeneratePublicKey(primeFactors0.first, primeFactors0.second, n0);
	uint64_t d0 = GeneratePrivateKey(primeFactors0.first, primeFactors0.second, e0);

	Pair primeFactors1 = GeneratePrimeFactors();
	uint64_t n1 = GetPrimeFactorsProduct(primeFactors1.first, primeFactors1.second);
	uint64_t e1 = GeneratePublicKey(primeFactors1.first, primeFactors1.second, n1);
	uint64_t d1 = GeneratePrivateKey(primeFactors1.first, primeFactors1.second, e1);

	string data = "This is a secret message. NO ONE should be able to read it.";

	for(char symbol : data) {
		uint64_t e1t = Encode(symbol, n1, e1);
		uint64_t d1t = Decode(e1t, n1, d1);
		assert((uint64_t) symbol == d1t);

		uint64_t e0d1e1t = Encode(symbol, n0, e0);
		uint64_t d0e0d1e1t = Decode(e0d1e1t, n0, d0);
		assert((uint64_t)symbol == d0e0d1e1t);
	}

	cout << "All tests passed successfully!\n";

	/*char symbol = 'A';
	long long encodedSymbol = Encode(symbol, n0, e0);
	long long decodedSymbol = Decode(encodedSymbol, n0, d0);

	cout << encodedSymbol << "\n";
	cout << (char)decodedSymbol << "\n";*/
}