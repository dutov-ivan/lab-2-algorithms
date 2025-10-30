#include "../include/print.h"

#include <iostream>
#include <memory>

#include "common.h"
#include "heuristic.h"
#include "report.h"

void print_bitboard(const Board &board) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.has_queen_at(queen_position(col, row))) {
                std::cout << " Q ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << std::endl;
    }
}

void depict_state(const Board &board, const std::unique_ptr<Heuristic> &h) {
    print_bitboard(board);
    const double h_value = h->calculate(board);
    std::cout << "Heuristic value: " << h_value << std::endl;
}

void print_reports(std::vector<SearchReport> &results) {
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << "EXPERIMENT " << (i + 1) << ": SOLVED" << std::endl;
        std::cout << "Starting position:" << std::endl;
        print_bitboard(results[i].startingPosition);
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "End result:" << std::endl;
        print_bitboard(results[i].solutionPosition);


        const auto &[iterations, nodesGenerated, nodesInMemory, deadEnds] = results[i].stats;
        std::cout << "Iterations: " << (iterations) << std::endl;
        std::cout << "Generated nodes: " << (nodesGenerated) << std::endl;
        std::cout << "Nodes in memory (max): " << (nodesInMemory) << std::endl;
        std::cout << "Dead ends encountered: " << (deadEnds) << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

void print_report_average(const AverageStats& results) {
    std::cout << "========================================" << std::endl;
    std::cout << "AVERAGE RESULTS" << std::endl;
    std::cout << "Average Iterations: " << results.avg_iterations << std::endl;
    std::cout << "Average Generated nodes: " << results.avg_nodesGenerated << std::endl;
    std::cout << "Average Nodes in memory (max): " << results.avg_nodesInMemory << std::endl;
    std::cout << "Average Dead ends encountered: " << results.avg_deadEnds << std::endl;
    std::cout << "========================================" << std::endl;
}
