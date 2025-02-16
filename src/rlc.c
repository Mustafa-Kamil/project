#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rlc.h"

static enum rlc_mode current_mode;
static uint16_t rlc_sn = 0;

void rlc_init(enum rlc_mode mode) {
    current_mode = mode;
    rlc_sn = 0;
}

uint8_t* rlc_segment_sdu(uint8_t* sdu, uint32_t sdu_size, uint32_t max_pdu_size, uint32_t* num_pdus) {
    if (!sdu || !num_pdus || sdu_size == 0 || max_pdu_size <= sizeof(struct rlc_header)) {
        return NULL;
    }

    uint32_t payload_size = max_pdu_size - sizeof(struct rlc_header);
    *num_pdus = (sdu_size + payload_size - 1) / payload_size;
    uint32_t total_size = *num_pdus * max_pdu_size;

    uint8_t* pdus = (uint8_t*)malloc(total_size);
    if (!pdus) return NULL;

    uint32_t remaining = sdu_size;
    uint32_t offset = 0;

    for (uint32_t i = 0; i < *num_pdus; i++) {
        struct rlc_header* header = (struct rlc_header*)(pdus + (i * max_pdu_size));
        header->sequence_number = rlc_sn++;
        header->segment_offset = offset;
        
        uint32_t current_payload = (remaining < payload_size) ? remaining : payload_size;
        header->flags = (i == *num_pdus - 1) ? 0x01 : 0x00;  // Set last segment flag

        memcpy(pdus + (i * max_pdu_size) + sizeof(struct rlc_header),
               sdu + offset,
               current_payload);

        remaining -= current_payload;
        offset += current_payload;
    }

    return pdus;
}

uint8_t* rlc_reassemble_pdus(uint8_t** pdus, uint32_t* pdu_sizes, uint32_t num_pdus, uint32_t* sdu_size) {
    if (!pdus || !pdu_sizes || !sdu_size || num_pdus == 0) {
        return NULL;
    }

    // Calculate total SDU size
    *sdu_size = 0;
    for (uint32_t i = 0; i < num_pdus; i++) {
        *sdu_size += pdu_sizes[i] - sizeof(struct rlc_header);
    }

    uint8_t* sdu = (uint8_t*)malloc(*sdu_size);
    if (!sdu) return NULL;

    uint32_t offset = 0;
    for (uint32_t i = 0; i < num_pdus; i++) {
        struct rlc_header* header = (struct rlc_header*)pdus[i];
        uint32_t payload_size = pdu_sizes[i] - sizeof(struct rlc_header);

        memcpy(sdu + offset,
               pdus[i] + sizeof(struct rlc_header),
               payload_size);
        
        offset += payload_size;
    }

    return sdu;
} 