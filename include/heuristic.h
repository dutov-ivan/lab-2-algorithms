#ifndef HEURISTIC_H
#define HEURISTIC_H
#include <cstdint>

#include "common.h"

double count_attacking_pairs(const Board &state);

double line_occupancy_heuristic(const Board &state);

#endif
