#ifndef SEARCH_H
#define SEARCH_H

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "common.h"
#include "heuristic.h"

struct SearchStats {
    std::size_t iterations = 0;
    std::size_t nodesGenerated = 0;
    std::size_t nodesInMemory = 0;

    std::size_t deadEnds = 0;

    SearchStats &operator+=(const SearchStats &other) {
        iterations += other.iterations;
        nodesGenerated += other.nodesGenerated;
        nodesInMemory = std::max(nodesInMemory, other.nodesInMemory);
        deadEnds += other.deadEnds;
        return *this;
    }
};

struct SearchResult {
    SearchStats stats;
    Board solution;
    bool solved = false;
};


class Search {
public:
    explicit Search(const std::string_view name) : name_(name) {
    }

    virtual SearchResult search(Board start) = 0;

    virtual ~Search() = default;

    [[nodiscard]] const std::string &name() const {
        return name_;
    }

private:
    std::string name_;
};

class HeuristicSearch : public Search {
public:
    explicit HeuristicSearch(const std::string_view name,
                             std::shared_ptr<Heuristic> h) : Search(name), h_(std::move(h)) {
    }

    [[nodiscard]] std::shared_ptr<Heuristic> h() const { return h_; }

private:
    std::shared_ptr<Heuristic> h_;
};

#endif // SEARCH_H
