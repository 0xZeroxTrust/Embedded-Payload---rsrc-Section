#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_DEPRECATE
typedef unsigned char BYTE;
typedef unsigned int SIZE_T;

// Function to XOR encrypt a payload with a single byte key
void XorByOneKey(BYTE* pShellcode, SIZE_T sShellcodeSize, BYTE bKey) {
    for (SIZE_T i = 0; i < sShellcodeSize; i++) {
        pShellcode[i] = pShellcode[i] ^ bKey;
        //printf("pShellcode : %s \n", pShellcode);
    }
}

// Function to read a file into a buffer
BYTE* ReadFileToBuffer(const char* filePath, SIZE_T* fileSize) {
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer for file content
    BYTE* buffer = (BYTE*)malloc(*fileSize);
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }

    // Read file content into buffer
    fread(buffer, 1, *fileSize, file);
    fclose(file);
    return buffer;
}

// Function to write a buffer to a file
int WriteBufferToFile(const char* filePath, BYTE* buffer, SIZE_T bufferSize) {
    FILE* file = fopen(filePath, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return -1;
    }

    fwrite(buffer, 1, bufferSize, file);
    fclose(file);
    return 0;
}

int main() {
    // File path of the payload
    const char* filePath = "file.bin";  // Replace with your file path

    // Read the file into a buffer
    SIZE_T payloadSize = 0;
    BYTE* payload = ReadFileToBuffer(filePath, &payloadSize);
    if (payload == NULL) {
        return -1;
    }

    // Key to use for encryption
    BYTE encryptionKey = 0x5A;  // You can change this key to any byte value

    // Encrypt the payload
    XorByOneKey(payload, payloadSize, encryptionKey);

    // Save the encrypted payload back to a file
    const char* encryptedFilePath = "encrypted_payload.ico";  // Output file path
    if (WriteBufferToFile(encryptedFilePath, payload, payloadSize) != 0) {
        free(payload);
        return -1;
    }

    // Print success message
    printf("Payload encrypted and saved to %s\n", encryptedFilePath);
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


    // Free the allocated buffer
    free(payload);

    return 0;
}
