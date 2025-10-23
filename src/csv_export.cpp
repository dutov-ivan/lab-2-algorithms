//
// Created by dutov on 10/23/2025.
//
// csv_export.cpp
#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/csv_export.h"

static std::string board_to_multiline_string(const Board &board) {
    std::ostringstream oss;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.has_queen_at(queen_position(col, row))) {
                oss << " Q ";
            } else {
                oss << " . ";
            }
        }
        if (row < 7) oss << '\n'; // keep last line without extra newline at end of field
    }
    return oss.str();
}

// Properly escape a CSV field: double internal quotes, and wrap in quotes if it contains
// newline, comma, or quotes (RFC4180-style, sufficient for common CSV readers)
static std::string escape_csv_field(const std::string &field) {
    bool need_quotes = false;
    for (char c: field) {
        if (c == '"' || c == ',' || c == '\n' || c == '\r') {
            need_quotes = true;
            break;
        }
    }
    if (!need_quotes) return field;

    std::string escaped;
    escaped.reserve(field.size() + 2);
    escaped.push_back('"');
    for (char c: field) {
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

    out << "Starting Position,Iterations,Nodes generated,Nodes in memory,Dead ends\n";

    for (const auto &r: results) {
        const std::string start_field = board_to_multiline_string(r.startingPosition);
        out << escape_csv_field(start_field) << ','
                << r.stats.iterations << ','
                << r.stats.nodesGenerated << ','
                << r.stats.nodesInMemory << ','
                << r.stats.deadEnds << '\n';
    }

    out.close();
    return true;
}
