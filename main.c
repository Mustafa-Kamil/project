#include <stdio.h>
#include <stdlib.h>
#include "ip_packet_generator.h"

int main() {
    // Initialize the packet generator
    init_packet_generator();

    // Example parameters
    uint16_t payload_size = 100;  // 100 bytes of payload
    const char* src_ip = "192.168.1.100";  // Source IP
    const char* dst_ip = "192.168.1.200";  // Destination IP
    uint8_t protocol = 6;  // TCP protocol
    uint16_t total_size;

    // Generate an IP packet
    uint8_t* packet = generate_ip_packet(payload_size, src_ip, dst_ip, protocol, &total_size);
    
    if (packet) {
        // Print packet information
        print_packet_info(packet, total_size);

        // Free the packet memory
        free(packet);
    } else {
        printf("Failed to generate packet\n");
    }

    return 0;
} 