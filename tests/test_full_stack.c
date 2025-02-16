#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pdcp.h"
#include "../include/rlc.h"
#include "../include/mac.h"
#include "../include/ip_packet_generator.h"

void print_hex(const char* label, uint8_t* data, uint32_t size) {
    printf("%s (size: %u bytes):\n", label, size);
    for (uint32_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n\n");
}

// Function to process packet through the protocol stack
void process_packet(uint8_t* ip_packet, uint16_t total_size) {
    printf("\n=== Processing IP Packet ===\n");
    print_packet_info(ip_packet, total_size);
    print_hex("IP Packet", ip_packet, total_size);

    // PDCP Layer (Uplink)
    uint32_t pdcp_pdu_size;
    uint8_t* pdcp_pdu = pdcp_pack_sdu(ip_packet, total_size, &pdcp_pdu_size);
    print_hex("PDCP PDU", pdcp_pdu, pdcp_pdu_size);

    // RLC Layer (Uplink)
    uint32_t num_rlc_pdus;
    uint32_t max_rlc_pdu_size = 50;  // Configurable segment size
    uint8_t* rlc_pdus = rlc_segment_sdu(pdcp_pdu, pdcp_pdu_size, max_rlc_pdu_size, &num_rlc_pdus);
    print_hex("RLC PDUs", rlc_pdus, num_rlc_pdus * max_rlc_pdu_size);

    // MAC Layer (Uplink)
    uint32_t tb_size;
    uint8_t* transport_block = mac_create_transport_block(rlc_pdus, 
        num_rlc_pdus * max_rlc_pdu_size, &tb_size);
    print_hex("Transport Block", transport_block, tb_size);

    printf("\n=== Transport Block Loopback ===\n");

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
    uint32_t final_size;
    uint8_t* final_packet = pdcp_unpack_pdu(rlc_sdu, rlc_sdu_size, &final_size);

    printf("\n=== Received IP Packet ===\n");
    print_packet_info(final_packet, final_size);
    print_hex("Final IP Packet", final_packet, final_size);

    // Verify data integrity
    if (total_size == final_size && 
        memcmp(ip_packet, final_packet, total_size) == 0) {
        printf("Test passed: IP packet integrity maintained through protocol stack\n");
    } else {
        printf("Test failed: IP packet corruption detected\n");
    }

    // Cleanup
    free(pdcp_pdu);
    free(rlc_pdus);
    free(transport_block);
    free(mac_sdu);
    free(rlc_pdu_array);
    free(rlc_pdu_sizes);
    free(rlc_sdu);
    free(final_packet);
}

int main() {
    // Initialize all layers
    init_packet_generator();
    pdcp_init();
    rlc_init(RLC_AM);
    mac_init();

    // Test with different packet sizes
    uint16_t payload_sizes[] = {100, 500, 1000, 1500};
    const char* src_ip = "192.168.1.100";
    const char* dst_ip = "192.168.1.200";
    uint8_t protocol = 6;  // TCP

    for (int i = 0; i < sizeof(payload_sizes)/sizeof(payload_sizes[0]); i++) {
        printf("\n\n=== Testing with payload size: %u bytes ===\n", payload_sizes[i]);
        uint16_t total_size;
        uint8_t* ip_packet = generate_ip_packet(payload_sizes[i], src_ip, dst_ip, protocol, &total_size);
        
        if (ip_packet) {
            process_packet(ip_packet, total_size);
            free(ip_packet);
        } else {
            printf("Failed to generate IP packet\n");
        }
    }

    return 0;
} 