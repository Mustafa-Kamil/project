#ifndef IP_PACKET_GENERATOR_H
#define IP_PACKET_GENERATOR_H

#include <stdint.h>

// IPv4 header structure
struct ip_header {
    uint8_t version_ihl;        // Version (4 bits) + Internet header length (4 bits)
    uint8_t tos;                // Type of service
    uint16_t total_length;      // Total length
    uint16_t identification;    // Identification
    uint16_t flags_fragment;    // Flags (3 bits) + Fragment offset (13 bits)
    uint8_t ttl;                // Time to live
    uint8_t protocol;           // Protocol
    uint16_t checksum;          // Header checksum
    uint32_t src_addr;          // Source address
    uint32_t dst_addr;          // Destination address
};

// Function declarations
void init_packet_generator(void);
uint8_t* generate_ip_packet(uint16_t payload_size, const char* src_ip, const char* dst_ip, uint8_t protocol, uint16_t* total_size);
uint16_t calculate_checksum(void* addr, int count);
void print_packet_info(uint8_t* packet, uint16_t total_size);

#endif // IP_PACKET_GENERATOR_H 