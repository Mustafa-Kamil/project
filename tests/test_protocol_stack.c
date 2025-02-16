#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pdcp.h"
#include "../include/rlc.h"
#include "../include/mac.h"

void print_hex(const char* label, uint8_t* data, uint32_t size) {
    printf("%s (size: %u bytes):\n", label, size);
    for (uint32_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n\n");
}

int main() {
    // Initialize all layers
    pdcp_init();
    rlc_init(RLC_AM);
    mac_init();

    // Test data
    const char* test_data = "This is a test message going through the complete 5G L2 protocol stack!";
    uint32_t data_size = strlen(test_data) + 1;

    printf("Original Data: %s\n", test_data);
    print_hex("Original Data", (uint8_t*)test_data, data_size);

    // PDCP Layer (Uplink)
    uint32_t pdcp_pdu_size;
    uint8_t* pdcp_pdu = pdcp_pack_sdu((uint8_t*)test_data, data_size, &pdcp_pdu_size);
    print_hex("PDCP PDU", pdcp_pdu, pdcp_pdu_size);

    // RLC Layer (Uplink)
    uint32_t num_rlc_pdus;
    uint32_t max_rlc_pdu_size = 20;  // Small size to force segmentation
    uint8_t* rlc_pdus = rlc_segment_sdu(pdcp_pdu, pdcp_pdu_size, max_rlc_pdu_size, &num_rlc_pdus);
    print_hex("RLC PDUs", rlc_pdus, num_rlc_pdus * max_rlc_pdu_size);

    // MAC Layer (Uplink)
    uint32_t tb_size;
    uint8_t* transport_block = mac_create_transport_block(rlc_pdus, 
        num_rlc_pdus * max_rlc_pdu_size, &tb_size);
    print_hex("Transport Block", transport_block, tb_size);

    printf("\n--- Transport Block Loopback ---\n\n");

    // MAC Layer (Downlink)
    uint32_t mac_sdu_size;
    uint8_t* mac_sdu = mac_process_transport_block(transport_block, tb_size, &mac_sdu_size);
    print_hex("MAC SDU", mac_sdu, mac_sdu_size);

    // RLC Layer (Downlink)
    uint32_t rlc_sdu_size;
    uint8_t** rlc_pdu_array = (uint8_t**)malloc(num_rlc_pdus * sizeof(uint8_t*));
    uint32_t* rlc_pdu_sizes = (uint32_t*)malloc(num_rlc_pdus * sizeof(uint32_t));

    for (uint32_t i = 0; i < num_rlc_pdus; i++) {
        rlc_pdu_array[i] = mac_sdu + (i * max_rlc_pdu_size);
        rlc_pdu_sizes[i] = max_rlc_pdu_size;
    }

    uint8_t* rlc_sdu = rlc_reassemble_pdus(rlc_pdu_array, rlc_pdu_sizes, num_rlc_pdus, &rlc_sdu_size);
    print_hex("RLC SDU", rlc_sdu, rlc_sdu_size);

    // PDCP Layer (Downlink)
    uint32_t final_sdu_size;
    uint8_t* final_sdu = pdcp_unpack_pdu(rlc_sdu, rlc_sdu_size, &final_sdu_size);

    printf("Final Data: %s\n", final_sdu);
    print_hex("Final Data", final_sdu, final_sdu_size);

    // Verify data integrity
    if (data_size == final_sdu_size && 
        memcmp(test_data, final_sdu, data_size) == 0) {
        printf("Test passed: Data integrity maintained through all layers\n");
    } else {
        printf("Test failed: Data corruption detected\n");
    }

    // Cleanup
    free(pdcp_pdu);
    free(rlc_pdus);
    free(transport_block);
    free(mac_sdu);
    free(rlc_pdu_array);
    free(rlc_pdu_sizes);
    free(rlc_sdu);
    free(final_sdu);

    return 0;
} 