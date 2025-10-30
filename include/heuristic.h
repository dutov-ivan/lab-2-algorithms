#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "common.h"
#include <string>

class Heuristic {
public:
    virtual ~Heuristic() = default;

    virtual unsigned int calculate(const Board &board) = 0;

    [[nodiscard]] std::string &name() { return name_; }

protected:
    explicit Heuristic(const std::string_view name) : name_(name) {
    }

private:
    std::string name_;
};

class CountAttackingPairs final : public Heuristic {
public:
    explicit CountAttackingPairs() : Heuristic("count attacking pairs") {
    }

    unsigned int calculate(const Board &board) override;
};

class LineOccupancyHeuristic final : public Heuristic {
public:
    explicit LineOccupancyHeuristic() : Heuristic("line occupancy heuristic") {
    }

    unsigned int calculate(const Board &board) override;
};

#endif
