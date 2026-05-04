#include <iostream>
#include <Windows.h>

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
BOOL WriteCB(LPCTSTR fn) {
	HANDLE hFile = CreateFile(fn,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	BOOL bSuccess = FALSE;
	DWORD size = GetFileSize(hFile, NULL);
	if (size != INVALID_FILE_SIZE && size >= 8) {
		PBYTE mem8 = new BYTE[size];
		if (mem8) {
			DWORD dwBytesRead = 0;
			if (ReadFile(hFile, mem8, size, &dwBytesRead, NULL) && dwBytesRead == size) {
				DWORD* pLastDword = (DWORD*)(mem8 + size - 4);
				if (*pLastDword == 0xFFFFFFFF) {
					size -= 8;
				}
				SetFilePointer(hFile, size, NULL, FILE_BEGIN);
				SetEndOfFile(hFile);

				DWORD dwCB[2]{};
				dwCB[0] = CalculateCRC(mem8, size);
				dwCB[1] = 0xFFFFFFFF;

				DWORD dwBytesWritten = 0;
				if (WriteFile(hFile, dwCB, sizeof(dwCB), &dwBytesWritten, NULL) && dwBytesWritten == sizeof(dwCB)) {
					bSuccess = TRUE;
				}
			}
		}
	}

	CloseHandle(hFile);

	return bSuccess;
}

int wmain(int argc, wchar_t* argv[])
{
    if (!WriteCB(argv[1])) {
        fwprintf(stderr, L"WriteCB failed for %s (err=%lu)\n", argv[1], GetLastError());
        return 1;
    }
	if (!CheckCB(argv[1])) {
        fwprintf(stderr, L"CheckCB failed for %s (err=%lu)\n", argv[1], GetLastError());
        return 1;
    }
	wprintf(L"Successfully added the crc to the dll.");

    return 0;
}