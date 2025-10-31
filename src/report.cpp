// csv_export.cpp
#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/report.h"

struct AverageStats;

static std::string board_to_singleline_string(const Board &board) {
    std::ostringstream oss;
    for (int col = 0; col < 8; ++col) {
        for (int row = 0; row < 8; ++row) {
            if (board.has_queen_at(queen_position(col, row))) {
                oss << row;
            }
        }
    }
    return oss.str();
}

// Properly escape a CSV field: double internal quotes, and wrap in quotes if it contains
// newline, comma, or quotes (RFC4180-style, sufficient for common CSV readers)
static std::string escape_csv_field(const std::string &field) {
    bool need_quotes = false;
    for (const char c: field) {
        if (c == '"' || c == ',' || c == '\n' || c == '\r') {
            need_quotes = true;
            break;
        }
    }
    if (!need_quotes) return field;

    std::string escaped;
    escaped.reserve(field.size() + 2);
    escaped.push_back('"');
    for (const char c: field) {
        if (c == '"') {
            escaped.push_back('"'); // double the quote
            escaped.push_back('"');
        } else {
            escaped.push_back(c);
        }
    }
    escaped.push_back('"');
    return escaped;
}

bool save_reports_csv(const std::vector<SearchReport> &results, const std::string &filename) {
    std::ofstream out(filename, std::ios::binary); // binary avoids CRLF surprises on Windows
    if (!out.is_open()) {
        std::cerr << "Failed to open CSV file for writing: " << filename << std::endl;
        return false;
    }

    out << "Стартова позиція,Ітерації,Згенеровано вузлів,Вузлів у пам'яті,Глухі кути\n";

    for (const auto &r: results) {
        const std::string start_field = board_to_singleline_string(r.startingPosition);
        out << escape_csv_field(start_field) << ','
                << r.stats.iterations << ','
                << r.stats.nodesGenerated << ','
                << r.stats.nodesInMemory << ','
                << r.stats.deadEnds << '\n';
    }

    out.close();
    return true;
}

bool save_average_stats_csv(const std::vector<std::pair<std::string, AverageStats> > &average_stats,
                            const std::string &filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Failed to open CSV file for writing: " << filename << std::endl;
        return false;
    }

    out << "Алгоритм,Середні ітерації,Середньо згенеровано вузлів,Середньо вузлів у пам'яті,Середньо глухих кутів\n";

    for (const auto &[algorithm_name, stats]: average_stats) {
        out << escape_csv_field(algorithm_name) << ','
                << stats.avg_iterations << ','
                << stats.avg_nodesGenerated << ','
                << stats.avg_nodesInMemory << ','
                << stats.avg_deadEnds << '\n';
    }

    out.close();
    return true;
}
