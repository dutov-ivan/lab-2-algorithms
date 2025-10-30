//
// Created by dutov on 10/23/2025.
//

#ifndef LAB_2_ALGORITHMS_CSV_EXPORT_H
#define LAB_2_ALGORITHMS_CSV_EXPORT_H
#include "search.h"
#include <vector>
#include <string>

struct SearchReport {
    Board startingPosition;
    Board solutionPosition;
    SearchStats stats;
};

struct AverageStats {
    double avg_iterations;
    double avg_nodesGenerated;
    double avg_nodesInMemory;
    double avg_deadEnds;
};

bool save_reports_csv(const std::vector<SearchReport> &results, const std::string &filename);
bool save_average_stats_csv(const std::vector<std::pair<std::string, AverageStats>> &average_stats,
                             const std::string &filename);
#endif //LAB_2_ALGORITHMS_CSV_EXPORT_H
