#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned int SIZE_T;

// Function to XOR encrypt a payload with a single byte key
void XorByOneKey(BYTE* pShellcode, SIZE_T sShellcodeSize, BYTE bKey) {
	size_t i =0;
    for (i = 0; i < sShellcodeSize; i++) {
        pShellcode[i] = pShellcode[i] ^ bKey;
    }
}

int main() {
    // Example payload to encrypt
    BYTE payload[] = "VirtualAlloca";
    SIZE_T payloadSize = sizeof(payload) - 1;  // Exclude the null terminator

    // Key to use for encryption
    BYTE encryptionKey = 0x5A;  // You can change this key to any byte value

    // Encrypt the payload
    XorByOneKey(payload, payloadSize, encryptionKey);
    printf("Encrpted Payload: %s\n", payload);
    
    // Print the XOR-encrypted result in hexadecimal array format
    printf("unsigned char encrypted_result[] = {");
    SIZE_T i;
    for (i = 0; i < payloadSize; i++) {
        printf("0x%02X", payload[i]);
        if (i < payloadSize - 1) {
            printf(", ");
        }
    }
    printf("};\n");

    // Decrypt the payload (for demonstration purposes)
    XorByOneKey(payload, payloadSize, encryptionKey);

    // Print the original and decrypted payload
    printf("Original Payload: %s\n", payload);

    return 0;
}

