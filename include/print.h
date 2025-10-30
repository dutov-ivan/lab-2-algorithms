#ifndef PRINT_H
#define PRINT_H
#include "common.h"
#include "heuristic.h"
#include "report.h"

void print_bitboard(const Board &board);

void depict_state(const Board &board, const std::unique_ptr<Heuristic> &h);

void print_reports(std::vector<SearchReport> &results);

void print_report_average(const AverageStats &results);

#endif // PRINT_H
