#ifndef RLC_H
#define RLC_H

#include <stdint.h>

// RLC modes
enum rlc_mode {
    RLC_TM,  // Transparent Mode
    RLC_UM,  // Unacknowledged Mode
    RLC_AM   // Acknowledged Mode
};

// RLC header structure
struct rlc_header {
    uint16_t sequence_number;
    uint8_t segment_offset;
    uint8_t flags;  // Last segment, etc.
};

// RLC functions
void rlc_init(enum rlc_mode mode);
uint8_t* rlc_segment_sdu(uint8_t* sdu, uint32_t sdu_size, uint32_t max_pdu_size, uint32_t* num_pdus);
uint8_t* rlc_reassemble_pdus(uint8_t** pdus, uint32_t* pdu_sizes, uint32_t num_pdus, uint32_t* sdu_size);

#endif // RLC_H 