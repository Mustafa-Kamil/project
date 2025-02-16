#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ip_packet_generator.h"

static uint16_t packet_id = 0;

// Convert string IP to uint32_t
static uint32_t ip_to_uint32(const char* ip) {
    uint32_t result = 0;
    unsigned int a, b, c, d;
    sscanf(ip, "%u.%u.%u.%u", &a, &b, &c, &d);
    result = (a << 24) | (b << 16) | (c << 8) | d;
    return result;
}

// Convert uint32_t IP to string
static void uint32_to_ip(uint32_t ip, char* str) {
    unsigned char bytes[4];
    bytes[0] = ip >> 24;
    bytes[1] = (ip >> 16) & 0xFF;
    bytes[2] = (ip >> 8) & 0xFF;
    bytes[3] = ip & 0xFF;
    sprintf(str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

void init_packet_generator(void) {
    srand((unsigned int)time(NULL));
    packet_id = (uint16_t)rand();
}

uint8_t* generate_ip_packet(uint16_t payload_size, const char* src_ip, const char* dst_ip, 
                           uint8_t protocol, uint16_t* total_size) {
    *total_size = sizeof(struct ip_header) + payload_size;
    
    uint8_t* packet = (uint8_t*)malloc(*total_size);
    if (!packet) return NULL;

    struct ip_header* header = (struct ip_header*)packet;
    
    header->version_ihl = 0x45;
    header->tos = 0;
    header->total_length = (payload_size + sizeof(struct ip_header));  // No swap needed
    header->identification = packet_id;  // No swap needed
    header->flags_fragment = 0x4000;
    header->ttl = 64;
    header->protocol = protocol;
    header->checksum = 0;
    header->src_addr = ip_to_uint32(src_ip);
    header->dst_addr = ip_to_uint32(dst_ip);

    header->checksum = calculate_checksum(header, sizeof(struct ip_header));

    uint8_t* payload = packet + sizeof(struct ip_header);
    for (int i = 0; i < payload_size; i++) {
        payload[i] = rand() % 256;
    }

    return packet;
}

uint16_t calculate_checksum(void* addr, int count) {
    uint32_t sum = 0;
    uint16_t* ptr = addr;

    while (count > 1) {
        sum += *ptr++;
        count -= 2;
    }

    if (count > 0) {
        sum += *(uint8_t*)ptr;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}

void print_packet_info(uint8_t* packet, uint16_t total_size) {
    struct ip_header* header = (struct ip_header*)packet;
    char src_ip[16];  // Changed from INET_ADDRSTRLEN
    char dst_ip[16];  // Changed from INET_ADDRSTRLEN
    
    printf("\nIP Packet Information:\n");
    printf("Version: %d\n", (header->version_ihl >> 4) & 0x0F);
    printf("Header Length: %d bytes\n", (header->version_ihl & 0x0F) * 4);
    printf("Total Length: %d bytes\n", header->total_length);
    printf("Identification: 0x%04x\n", header->identification);
    printf("TTL: %d\n", header->ttl);
    printf("Protocol: %d\n", header->protocol);
    printf("Checksum: 0x%04x\n", header->checksum);
    
    // Use our conversion function instead of inet_ntop
    uint32_to_ip(header->src_addr, src_ip);
    uint32_to_ip(header->dst_addr, dst_ip);
    
    printf("Source IP: %s\n", src_ip);
    printf("Destination IP: %s\n", dst_ip);
    printf("Payload Size: %d bytes\n", total_size - sizeof(struct ip_header));
} 