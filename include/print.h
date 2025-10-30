#ifndef PRINT_H
#define PRINT_H
#include <cstdint>
#include <memory>

#include "common.h"
#include "heuristic.h"

void print_bitboard(const Board &board);

void depict_state(const Board &board, const std::unique_ptr<Heuristic> &h);

#endif // PRINT_H
