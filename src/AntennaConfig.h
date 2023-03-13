#pragma once
#include <stdint.h>

// don't change that number
#define ANTENNA_COUNT 6

struct AntennaDefinition
{
    char name[16];
    char desc[16];
};

struct AntennaConfig
{
    uint8_t enable;
    uint8_t flags_0;
    uint8_t flags_1;
    uint8_t flags_2;
    AntennaDefinition antennas[ANTENNA_COUNT];
};
