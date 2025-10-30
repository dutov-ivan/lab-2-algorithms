#include <iostream>

#include "csv_export.h"
#include "../include/generate.h"
#include "../include/astar.h"
#include "../include/print.h"
#include "../include/heuristic.h"
#include "../include/local_backtrack.h"

std::vector<SearchReport> perform_experiments(const std::unique_ptr<Search> &searcher, std::mt19937 &gen,
                                              const std::unique_ptr<Heuristic> &h,
                                              const std::string &search_name, const std::size_t experiment_count) {
    std::vector<SearchReport> results;
    results.reserve(experiment_count);

    std::cout << "=== " << search_name << " EXPERIMENTS ===" << std::endl;
    for (size_t i = 0; i < experiment_count; i++) {
        const Board initial_board = generate_initial_bitboard(gen);
        SearchResult result = searcher->search(initial_board, h);
        results.push_back({initial_board, result.solution, result.stats,});
    }
    return results;
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

void print_report_average(std::vector<SearchReport> &results) {
    SearchStats total_stats{};
    for (const auto &result: results) {
        total_stats += result.stats;
    }

    const std::size_t experiment_count = results.size();
    total_stats.iterations /= experiment_count;
    total_stats.nodesGenerated /= experiment_count;
    total_stats.nodesInMemory /= experiment_count;
    total_stats.deadEnds /= experiment_count;

    std::cout << "========================================" << std::endl;
    std::cout << "AVERAGE RESULTS" << std::endl;
    std::cout << "Average Iterations: " << total_stats.iterations << std::endl;
    std::cout << "Average Generated nodes: " << total_stats.nodesGenerated << std::endl;
    std::cout << "Average Nodes in memory (max): " << total_stats.nodesInMemory << std::endl;
    std::cout << "Average Dead ends encountered: " << total_stats.deadEnds << std::endl;
    std::cout << "========================================" << std::endl;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::unique_ptr<Search> > searchers;
    searchers.push_back(std::make_unique<AStarSearch>());
    searchers.push_back(std::make_unique<AnnealingThenBacktrack>(gen));

    std::vector<std::unique_ptr<Heuristic> > heuristics;
    heuristics.push_back(std::make_unique<CountAttackingPairs>());
    heuristics.push_back(std::make_unique<LineOccupancyHeuristic>());

    for (const auto &heuristic: heuristics) {
        for (const auto &searcher: searchers) {
            std::vector<SearchReport> results = perform_experiments(searcher, gen, heuristic, searcher->name(), 20);
            std::cout << "REPORT FOR ALGORITHM: " << searcher->name()
                    << " WITH HEURISTIC: " << heuristic->name() << std::endl;
            print_reports(results);
            print_report_average(results);
            const std::string filename = searcher->name() + "_" + heuristic->name() + "_experiments.csv";
            if (!save_reports_csv(results, filename)) {
                std::cerr << "CSV export failed\n";
            } else {
                std::cout << "Wrote " << filename << std::endl;
            }
            std::cout << "Press Enter to continue..." << std::endl;
            std::cin.get();
        }
    }

    return 0;
}
