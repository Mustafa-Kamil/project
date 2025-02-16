#ifndef PROFILING_H
#define PROFILING_H

#include <stdint.h>
#include <time.h>

struct layer_stats {
    double uplink_time;    // Time taken for uplink processing
    double downlink_time;  // Time taken for downlink processing
    uint32_t bytes_processed;
    uint32_t pdus_generated;
};

struct protocol_stats {
    struct layer_stats pdcp;
    struct layer_stats rlc;
    struct layer_stats mac;
    double total_time;
    uint32_t total_packets;
};

void init_protocol_stats(void);
void start_profiling(void);
void stop_profiling(void);
void print_protocol_stats(void);
void update_layer_stats(struct layer_stats* stats, clock_t start_time, uint32_t bytes, uint32_t pdus);

#endif // PROFILING_H 