#ifndef PRINT_H
#define PRINT_H
#include <cstdint>

#include "common.h"

void print_bitboard(const Board &board);

void depict_state(const Board &board, HeuristicFunction h);

#endif // PRINT_H
