#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pdcp.h"

void print_hex(uint8_t* data, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main() {
    // Initialize PDCP
    pdcp_init();

    // Test data
    const char* test_data = "Hello, this is a test message for PDCP layer!";
    uint32_t sdu_size = strlen(test_data) + 1;  // Include null terminator
    
    // Pack SDU into PDU
    uint32_t pdu_size;
    uint8_t* pdu = pdcp_pack_sdu((uint8_t*)test_data, sdu_size, &pdu_size);
    
    if (!pdu) {
        printf("Failed to pack SDU\n");
        return 1;
    }

    printf("Original Data: %s\n", test_data);
    printf("PDU with header (hex):\n");
    print_hex(pdu, pdu_size);

    // Unpack PDU back to SDU
    uint32_t unpacked_sdu_size;
    uint8_t* unpacked_sdu = pdcp_unpack_pdu(pdu, pdu_size, &unpacked_sdu_size);
    
    if (!unpacked_sdu) {
        printf("Failed to unpack PDU\n");
        free(pdu);
        return 1;
    }

    printf("Unpacked Data: %s\n", unpacked_sdu);

    // Verify data integrity
    if (sdu_size == unpacked_sdu_size && 
        memcmp(test_data, unpacked_sdu, sdu_size) == 0) {
        printf("Test passed: Data integrity maintained\n");
    } else {
        printf("Test failed: Data corruption detected\n");
    }

    // Cleanup
    free(pdu);
    free(unpacked_sdu);

    return 0;
} 