#ifndef PDCP_H
#define PDCP_H

#include <stdint.h>

// PDCP PDU header structure
struct pdcp_header {
    uint32_t sequence_number;  // PDCP sequence number
    uint8_t data_type;        // Control or User plane
    uint8_t reserved;         // Reserved bits
};

// PDCP functions
void pdcp_init(void);
uint8_t* pdcp_pack_sdu(uint8_t* sdu, uint32_t sdu_size, uint32_t* pdu_size);
uint8_t* pdcp_unpack_pdu(uint8_t* pdu, uint32_t pdu_size, uint32_t* sdu_size);

#endif // PDCP_H 