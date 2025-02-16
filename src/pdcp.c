#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pdcp.h"

static uint32_t pdcp_sn = 0;  // PDCP sequence number counter

// Simple XOR-based encryption/decryption key (in real implementation, this would be more secure)
static const uint8_t encryption_key[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

void pdcp_init(void) {
    pdcp_sn = 0;
}

// Helper function to encrypt/decrypt data
static void encrypt_decrypt_data(uint8_t* data, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        data[i] ^= encryption_key[i % sizeof(encryption_key)];
    }
}

uint8_t* pdcp_pack_sdu(uint8_t* sdu, uint32_t sdu_size, uint32_t* pdu_size) {
    if (!sdu || !pdu_size || sdu_size == 0) {
        return NULL;
    }

    // Calculate PDU size (header + encrypted data)
    *pdu_size = sizeof(struct pdcp_header) + sdu_size;
    
    // Allocate memory for PDU
    uint8_t* pdu = (uint8_t*)malloc(*pdu_size);
    if (!pdu) {
        return NULL;
    }

    // Prepare header
    struct pdcp_header* header = (struct pdcp_header*)pdu;
    header->sequence_number = pdcp_sn++;
    header->data_type = 0;  // User plane data
    header->reserved = 0;

    // Copy and encrypt the data
    memcpy(pdu + sizeof(struct pdcp_header), sdu, sdu_size);
    encrypt_decrypt_data(pdu + sizeof(struct pdcp_header), sdu_size);

    return pdu;
}

uint8_t* pdcp_unpack_pdu(uint8_t* pdu, uint32_t pdu_size, uint32_t* sdu_size) {
    if (!pdu || !sdu_size || pdu_size <= sizeof(struct pdcp_header)) {
        return NULL;
    }

    // Extract header
    struct pdcp_header* header = (struct pdcp_header*)pdu;
    
    // Calculate SDU size
    *sdu_size = pdu_size - sizeof(struct pdcp_header);
    
    // Allocate memory for SDU
    uint8_t* sdu = (uint8_t*)malloc(*sdu_size);
    if (!sdu) {
        return NULL;
    }

    // Copy and decrypt the data
    memcpy(sdu, pdu + sizeof(struct pdcp_header), *sdu_size);
    encrypt_decrypt_data(sdu, *sdu_size);

    return sdu;
} 