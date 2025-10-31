#include <iostream>

#include "report.h"
#include "../include/generate.h"
#include "../include/astar.h"
#include "../include/print.h"
#include "../include/heuristic.h"
#include "../include/local_backtrack.h"

std::vector<SearchReport> perform_experiments(const std::unique_ptr<Search> &searcher, std::mt19937 &gen,
                                              std::size_t experiment_count);

AverageStats calculate_average_algorithm_stats(const std::vector<SearchReport> &results);


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::shared_ptr<Heuristic> > heuristics;
    heuristics.push_back(std::make_shared<CountAttackingPairs>());
    heuristics.push_back(std::make_shared<LineOccupancyHeuristic>());

    std::vector<std::unique_ptr<Search> > searchers;
    for (const auto &heuristic: heuristics) {
        searchers.push_back(std::make_unique<AStarSearch>(heuristic));
        searchers.push_back(std::make_unique<AnnealingThenBacktrack>(gen, heuristic));
    }

    std::vector<std::pair<std::string, AverageStats> > all_average_stats;

    for (const auto &searcher: searchers) {
        std::cout << "=== " << searcher->name() << " EXPERIMENTS ===" << std::endl;

        const auto derived = dynamic_cast<HeuristicSearch *>(searcher.get());
        std::shared_ptr<Heuristic> heuristic = nullptr;

        if (derived) {
            heuristic = derived->h(); // Safe only if derived is non-null
        }

        std::vector<SearchReport> search_reports = perform_experiments(searcher, gen, 20);

        std::cout << "REPORT FOR ALGORITHM: " << searcher->name();
        if (heuristic) {
            std::cout << " WITH HEURISTIC: " << heuristic->name();
        }
        std::cout << std::endl;

        print_reports(search_reports);

        const AverageStats average_stats = calculate_average_algorithm_stats(search_reports);

        std::string heuristic_name = heuristic ? heuristic->name() : "None";
        all_average_stats.emplace_back(searcher->name() + " + " + heuristic_name, average_stats);

        print_report_average(average_stats);

        if (std::string filename = searcher->name() + "_" + heuristic_name + "_experiments.csv"; !save_reports_csv(
            search_reports, filename)) {
            std::cerr << "CSV export failed\n";
        } else {
            std::cout << "Wrote " << filename << std::endl;
        }

        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get();
    }

    if (!save_average_stats_csv(all_average_stats, "average_algorithm_stats.csv")) {
        std::cerr << "CSV export failed\n";
    } else {
        std::cout << "Wrote average_algorithm_stats.csv" << std::endl;
    }

    return 0;
}

std::vector<SearchReport> perform_experiments(const std::unique_ptr<Search> &searcher, std::mt19937 &gen,
                                              const std::size_t experiment_count) {
    std::vector<SearchReport> results;
    results.reserve(experiment_count);

    for (size_t i = 0; i < experiment_count; i++) {
        const Board initial_board = generate_initial_bitboard(gen);
        const SearchResult result = searcher->search(initial_board);
        results.push_back({initial_board, result.solution, result.stats,});
    }
    return results;
}


AverageStats calculate_average_algorithm_stats(const std::vector<SearchReport> &results) {
    SearchStats total_stats{};
    for (const auto &result: results) {
        total_stats += result.stats;
    }

    const std::size_t experiment_count = results.size();
    const AverageStats average_results
    {
        static_cast<double>(total_stats.iterations) / static_cast<double>(
            experiment_count),
        static_cast<double>(total_stats.nodesGenerated) / static_cast<double>(
            experiment_count),
        static_cast<double>(total_stats.nodesInMemory) / static_cast<double>(
            experiment_count),
        static_cast<double>(total_stats.deadEnds) / static_cast<double>(experiment_count)
    };
    return average_results;
}

