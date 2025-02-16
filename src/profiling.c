#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/profiling.h"

static struct protocol_stats stats;
static clock_t profile_start;

void init_protocol_stats(void) {
    memset(&stats, 0, sizeof(struct protocol_stats));
}

void start_profiling(void) {
    profile_start = clock();
}

void stop_profiling(void) {
    stats.total_time = ((double)(clock() - profile_start)) / CLOCKS_PER_SEC;
}

void update_layer_stats(struct layer_stats* layer, clock_t start_time, uint32_t bytes, uint32_t pdus) {
    double time_taken = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
    layer->uplink_time += time_taken;
    layer->bytes_processed += bytes;
    layer->pdus_generated += pdus;
}

void print_protocol_stats(void) {
    printf("\n=== Protocol Stack Performance Statistics ===\n");
    printf("Total processing time: %.6f seconds\n", stats.total_time);
    printf("Total packets processed: %u\n\n", stats.total_packets);

    printf("PDCP Layer Statistics:\n");
    printf("  Uplink processing time: %.6f seconds\n", stats.pdcp.uplink_time);
    printf("  Downlink processing time: %.6f seconds\n", stats.pdcp.downlink_time);
    printf("  Bytes processed: %u\n", stats.pdcp.bytes_processed);
    printf("  PDUs generated: %u\n\n", stats.pdcp.pdus_generated);

    printf("RLC Layer Statistics:\n");
    printf("  Uplink processing time: %.6f seconds\n", stats.rlc.uplink_time);
    printf("  Downlink processing time: %.6f seconds\n", stats.rlc.downlink_time);
    printf("  Bytes processed: %u\n", stats.rlc.bytes_processed);
    printf("  PDUs generated: %u\n\n", stats.rlc.pdus_generated);

    printf("MAC Layer Statistics:\n");
    printf("  Uplink processing time: %.6f seconds\n", stats.mac.uplink_time);
    printf("  Downlink processing time: %.6f seconds\n", stats.mac.downlink_time);
    printf("  Bytes processed: %u\n", stats.mac.bytes_processed);
    printf("  PDUs generated: %u\n\n", stats.mac.pdus_generated);

    // Calculate throughput
    double total_mb = (double)stats.pdcp.bytes_processed / (1024 * 1024);
    printf("Overall Performance:\n");
    printf("  Throughput: %.2f MB/s\n", total_mb / stats.total_time);
    printf("  Average packet processing time: %.6f seconds\n", 
           stats.total_time / stats.total_packets);
} 