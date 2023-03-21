#pragma once
#include <Arduino.h>
#include "Print.h"

uint8_t readInput();
int clipRound(int n, int min, int max);
void turnOffAll();
void writeOutput(int idx);
