#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <cassert>
#include <vector>

using std::cout, std::wcout, std::string, std::vector, std::runtime_error, std::wstring;

#ifdef UNICODE
    #define _tcout wcout
#else
    #define _tcout cout
#endif

struct Pair {
    unsigned __int32 first;
    unsigned __int32 second;
};

typedef Pair(*GeneratePrimeFactors)();
typedef unsigned __int64 (*GetPrimeFactorsProduct)(Pair p);
typedef unsigned __int64 (*GeneratePublicKey)(Pair p, unsigned __int64 n);
typedef unsigned __int64 (*GeneratePrivateKey)(Pair p, unsigned __int64 e);
typedef unsigned __int64 (*Encrypt)(unsigned __int64 input, unsigned __int64 n, unsigned __int64 e);
typedef unsigned __int64 (*Decrypt)(unsigned __int64 input, unsigned __int64 n, unsigned __int64 d);

GeneratePrimeFactors gpf;
GetPrimeFactorsProduct gpfp;
GeneratePublicKey gpk;
GeneratePrivateKey gprk;
Encrypt enc;
Decrypt dec;
HMODULE h, hUA, hEN;

static FARPROC tryLoadFunction(HMODULE hModule, const char* functionName) {
    FARPROC func = GetProcAddress(hModule, functionName);
    if (func == nullptr) {
		throw runtime_error("Failed to get the " + string(functionName) + " function address.");
    }
    return func;
}

int main(int argc, char* argv[])
{
    h = LoadLibrary(L"LiteRSA.dll");
    if(h == nullptr) {
        cout << "Failed to load the library!\n";
        return 1;
	}

    const int BUFFER_MAX = 30;
	TCHAR surname[BUFFER_MAX];
	TCHAR faculty[BUFFER_MAX];
	TCHAR group[BUFFER_MAX];
	TCHAR discipline[BUFFER_MAX];
    if (argv[1][0] == '1') {
        hUA = LoadLibrary(L"Resources_UA.dll");

        if (hUA == nullptr) {
            cout << "Library was not loaded!\n";
            return 1;
        }
        else {
            SetConsoleOutputCP(1251);
            SetConsoleCP(1251);
            setlocale(LC_ALL, "Ukrainian");
            LoadString(hUA, 101, surname, BUFFER_MAX);
            LoadString(hUA, 102, faculty, BUFFER_MAX);
            LoadString(hUA, 103, group, BUFFER_MAX);
            LoadString(hUA, 104, discipline, BUFFER_MAX);
            FreeLibrary(hUA);
        }
    } 
    else {
        hEN = LoadLibrary(L"Resources_EN.dll");

        if (hEN == nullptr) {
            cout << "Library was not loaded!\n";
			return 1;
        }
        else {
            LoadString(hEN, 101, surname, BUFFER_MAX);
            LoadString(hEN, 102, faculty, BUFFER_MAX);
            LoadString(hEN, 103, group, BUFFER_MAX);
            LoadString(hEN, 104, discipline, BUFFER_MAX);
            FreeLibrary(hEN);
        }
    }
	_tcout << surname << "\n" << faculty << "\n" << group << "\n" << discipline << "\n";

    try
    {
        gpf = (GeneratePrimeFactors)tryLoadFunction(h, "GeneratePrimeFactors");
        gpfp = (GetPrimeFactorsProduct)tryLoadFunction(h, "GetPrimeFactorsProduct");
        gpk = (GeneratePublicKey)tryLoadFunction(h, "GeneratePublicKey");
        gprk = (GeneratePrivateKey)tryLoadFunction(h, "GeneratePrivateKey");
        enc = (Encrypt)tryLoadFunction(h, "Encrypt");
        dec = (Decrypt)tryLoadFunction(h, "Decrypt");
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << "\n";
        FreeLibrary(h);
        return 1;
    }

    Pair primeFactors0 = gpf();
    uint64_t n0 = gpfp(primeFactors0);
    uint64_t e0 = gpk(primeFactors0, n0);
    uint64_t d0 = gprk(primeFactors0, e0);

    Pair primeFactors1 = gpf();
    uint64_t n1 = gpfp(primeFactors1);
    uint64_t e1 = gpk(primeFactors1, n1);
    uint64_t d1 = gprk(primeFactors1, e1);

    string data = "This is a secret message. NO ONE should be able to read it!";
    for (char symbol : data) {
        uint64_t e1t = enc(symbol, n1, e1);
        uint64_t d1e1t = dec(e1t, n1, d1);
        assert((uint64_t)symbol == d1e1t);

        uint64_t e0d1e1t = enc(d1e1t, n0, e0);
        uint64_t d0e0d1e1t = dec(e0d1e1t, n0, d0);
        assert((uint64_t)symbol == d0e0d1e1t);
    }

    vector<uint64_t> numbersToTest = { 0, 1, 42, 12345, 65535, 10000000, 1000000000 };
    for (uint64_t number : numbersToTest) {
        uint64_t e1d0e0t = enc(number, n1, e1);
        uint64_t d1e1d0e0t = dec(e1d0e0t, n1, d1);
        assert((uint64_t)number == d1e1d0e0t);

        uint64_t e0t = enc(d1e1d0e0t, n0, e0);
        uint64_t d0e0t = dec(e0t, n0, d0);
        assert((uint64_t)number == d0e0t);
    }

    cout << "All tests passed successfully!\n";
	FreeLibrary(h);
}