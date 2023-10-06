#include "catch2/catch_test_macros.hpp"

#include "Circuit.h"

TEST_CASE("CircuitFunctionsTest", "[CircuitFunctionsTest]") {
    for (const auto &[a, b]: std::vector<std::pair<bool, bool>>{{0, 0},
                                                                {0, 1},
                                                                {1, 0},
                                                                {1, 1}}) {
        REQUIRE(Circuit::Functions::AND({a, b}) == (a & b));
        REQUIRE(Circuit::Functions::OR({a, b}) == (a | b));
        REQUIRE(Circuit::Functions::NAND({a, b}) == !(a & b));
        REQUIRE(Circuit::Functions::NOR({a, b}) == !(a | b));
        REQUIRE(Circuit::Functions::XOR({a, b}) == (a ^ b));
        REQUIRE(Circuit::Functions::NXOR({a, b}) == !(a ^ b));
    }
}