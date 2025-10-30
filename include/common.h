#ifndef COMMON_H
#define COMMON_H
#include <cstdint>

struct Board {
    explicit Board(const std::uint64_t bb = 0) : bitboard(bb) {
    }

    [[nodiscard]] std::uint64_t get() const;

    Board *clear_queen(std::uint8_t position);

    [[nodiscard]] bool has_queen_at(std::uint8_t position) const;

    Board *set_queen(std::uint8_t position);

    [[nodiscard]] Board queens_attacked_from(std::uint8_t position) const;

    [[nodiscard]] std::uint8_t count_queens() const;

    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] int queen_row(std::uint8_t col) const;

    struct Iterator {
        std::uint64_t bb;

        bool operator!=(const Iterator &other) const { return bb != other.bb; }

        Iterator &operator++() {
            bb &= bb - 1;
            return *this;
        } // pop lowest bit
        int operator*() const { return __builtin_ctzll(bb); } // index of LSB
    };

    [[nodiscard]] Iterator begin() const { return Iterator{bitboard}; }
    [[nodiscard]] static Iterator end() { return Iterator{}; }

private:
    std::uint64_t bitboard;
};

std::uint8_t queen_position(std::uint8_t col, std::uint8_t row);

#endif // COMMON_H
