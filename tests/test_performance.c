#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pdcp.h"
#include "../include/rlc.h"
#include "../include/mac.h"
#include "../include/ip_packet_generator.h"
#include "../include/profiling.h"

void process_packet_with_profiling(uint8_t* ip_packet, uint16_t total_size) {
    clock_t layer_start;
    
    // PDCP Layer (Uplink)
    layer_start = clock();
    uint32_t pdcp_pdu_size;
    uint8_t* pdcp_pdu = pdcp_pack_sdu(ip_packet, total_size, &pdcp_pdu_size);
    update_layer_stats(&stats.pdcp, layer_start, total_size, 1);

    // RLC Layer (Uplink)
    layer_start = clock();
    uint32_t num_rlc_pdus;
    uint32_t max_rlc_pdu_size = 50;
    uint8_t* rlc_pdus = rlc_segment_sdu(pdcp_pdu, pdcp_pdu_size, max_rlc_pdu_size, &num_rlc_pdus);
    update_layer_stats(&stats.rlc, layer_start, pdcp_pdu_size, num_rlc_pdus);

    // MAC Layer (Uplink)
    layer_start = clock();
    uint32_t tb_size;
    uint8_t* transport_block = mac_create_transport_block(rlc_pdus, 
        num_rlc_pdus * max_rlc_pdu_size, &tb_size);
    update_layer_stats(&stats.mac, layer_start, num_rlc_pdus * max_rlc_pdu_size, 1);

    // Transport Block Loopback

    // MAC Layer (Downlink)
    layer_start = clock();
    uint32_t mac_sdu_size;
    uint8_t* mac_sdu = mac_process_transport_block(transport_block, tb_size, &mac_sdu_size);
    stats.mac.downlink_time += ((double)(clock() - layer_start)) / CLOCKS_PER_SEC;

    // RLC Layer (Downlink)
    layer_start = clock();
    uint32_t rlc_sdu_size;
    uint8_t** rlc_pdu_array = (uint8_t**)malloc(num_rlc_pdus * sizeof(uint8_t*));
    uint32_t* rlc_pdu_sizes = (uint32_t*)malloc(num_rlc_pdus * sizeof(uint32_t));

    for (uint32_t i = 0; i < num_rlc_pdus; i++) {
        rlc_pdu_array[i] = mac_sdu + (i * max_rlc_pdu_size);
        rlc_pdu_sizes[i] = max_rlc_pdu_size;
    }

    uint8_t* rlc_sdu = rlc_reassemble_pdus(rlc_pdu_array, rlc_pdu_sizes, num_rlc_pdus, &rlc_sdu_size);
    stats.rlc.downlink_time += ((double)(clock() - layer_start)) / CLOCKS_PER_SEC;

    // PDCP Layer (Downlink)
    layer_start = clock();
    uint32_t final_size;
    uint8_t* final_packet = pdcp_unpack_pdu(rlc_sdu, rlc_sdu_size, &final_size);
    stats.pdcp.downlink_time += ((double)(clock() - layer_start)) / CLOCKS_PER_SEC;

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
    // Initialize everything
    init_packet_generator();
    pdcp_init();
    rlc_init(RLC_AM);
    mac_init();
    init_protocol_stats();

    // Test parameters
    uint16_t payload_sizes[] = {100, 500, 1000, 1500, 2000, 3000, 4000, 5000};
    const char* src_ip = "192.168.1.100";
    const char* dst_ip = "192.168.1.200";
    uint8_t protocol = 6;
    int iterations = 100;  // Number of iterations for each size

    start_profiling();

    for (int i = 0; i < sizeof(payload_sizes)/sizeof(payload_sizes[0]); i++) {
        printf("\nTesting with payload size: %u bytes\n", payload_sizes[i]);
        
        for (int j = 0; j < iterations; j++) {
            uint16_t total_size;
            uint8_t* ip_packet = generate_ip_packet(payload_sizes[i], src_ip, dst_ip, protocol, &total_size);
            
            if (ip_packet) {
                process_packet_with_profiling(ip_packet, total_size);
                stats.total_packets++;
                free(ip_packet);
            }
        }
    }

    stop_profiling();
    print_protocol_stats();

    return 0;
} 