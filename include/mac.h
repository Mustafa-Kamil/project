#ifndef MAC_H
#define MAC_H

#include <stdint.h>

// MAC header structure
struct mac_header {
    uint8_t lcid;          // Logical Channel ID
    uint16_t length;       // Length of payload
    uint8_t flags;         // Various control flags
};

// MAC functions
void mac_init(void);
uint8_t* mac_create_transport_block(uint8_t* data, uint32_t data_size, uint32_t* tb_size);
uint8_t* mac_process_transport_block(uint8_t* tb, uint32_t tb_size, uint32_t* data_size);

#endif // MAC_H 