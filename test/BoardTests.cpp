//
// Created by dutov on 10/23/2025.
//

#include <gtest/gtest.h>

#include "common.h"

std::uint8_t position_from_coords(const std::string &coords) {
    char file = coords[0]; // 'a' to 'h'
    char rank = coords[1]; // '1' to '8'
    int col = file - 'a';
    int row = rank - '1';
    return row * 8 + col;
}

TEST(BoardTest, SetAndClearQueen) {
    Board board;
    board.set_queen(0);
    EXPECT_TRUE(board.has_queen_at(0));
    board.clear_queen(0);
    EXPECT_FALSE(board.has_queen_at(0));
}

TEST(QueenAttacks, QueensAttackedFrom) {
    Board board;
    board.set_queen(position_from_coords("d2")); // Place a queen at d2
    board.set_queen(position_from_coords("d4")); // Place a queen at d4
    board.set_queen(position_from_coords("a5")); // Place a queen at a5
    board.set_queen(position_from_coords("g2")); // Place a queen at g4


    Board attacked = board.queens_attacked_from(position_from_coords("d2"));
    // Check a position that should not be attacked
    EXPECT_FALSE(attacked.has_queen_at(position_from_coords("d2"))); // same square
    EXPECT_TRUE(attacked.has_queen_at(position_from_coords("d4"))); // same column
    EXPECT_TRUE(attacked.has_queen_at(position_from_coords("a5"))); // diagonal
    EXPECT_TRUE(attacked.has_queen_at(position_from_coords("g2"))); // horizontal
}

TEST(QueenIterator, IterateQueens) {
    Board board;
    board.set_queen(position_from_coords("a1"));
    board.set_queen(position_from_coords("c3"));
    board.set_queen(position_from_coords("e5"));

    std::vector<std::uint8_t> expected_positions = {
        position_from_coords("a1"),
        position_from_coords("c3"),
        position_from_coords("e5")
    };

    std::vector<std::uint8_t> actual_positions;
    for (const auto pos: board) {
        actual_positions.push_back(pos);
    }

    EXPECT_EQ(expected_positions, actual_positions);
}

TEST(CountQueensTest, CountQueens) {
    Board board;
    EXPECT_EQ(board.count_queens(), 0);
    EXPECT_TRUE(board.is_empty());
    board.set_queen(position_from_coords("a1"));
    board.set_queen(position_from_coords("b2"));
    board.set_queen(position_from_coords("c3"));
    EXPECT_EQ(board.count_queens(), 3);
    board.clear_queen(position_from_coords("b2"));
    EXPECT_EQ(board.count_queens(), 2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
