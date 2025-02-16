#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/mac.h"

static uint8_t current_lcid = 1;

void mac_init(void) {
    current_lcid = 1;
}

uint8_t* mac_create_transport_block(uint8_t* data, uint32_t data_size, uint32_t* tb_size) {
    if (!data || !tb_size || data_size == 0) {
        return NULL;
    }

    *tb_size = sizeof(struct mac_header) + data_size;
    uint8_t* tb = (uint8_t*)malloc(*tb_size);
    if (!tb) return NULL;

    // Create MAC header
    struct mac_header* header = (struct mac_header*)tb;
    header->lcid = current_lcid;
    header->length = data_size;
    header->flags = 0;  // No special flags

    // Copy data
    memcpy(tb + sizeof(struct mac_header), data, data_size);

    return tb;
}

uint8_t* mac_process_transport_block(uint8_t* tb, uint32_t tb_size, uint32_t* data_size) {
    if (!tb || !data_size || tb_size <= sizeof(struct mac_header)) {
        return NULL;
    }

    struct mac_header* header = (struct mac_header*)tb;
    *data_size = header->length;

    uint8_t* data = (uint8_t*)malloc(*data_size);
    if (!data) return NULL;

    memcpy(data, tb + sizeof(struct mac_header), *data_size);

    return data;
} 