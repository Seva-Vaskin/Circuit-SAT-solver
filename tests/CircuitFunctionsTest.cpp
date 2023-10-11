#include "catch2/catch_test_macros.hpp"

#include "Circuit.h"

TEST_CASE("CircuitFunctionsTest", "[CircuitFunctionsTest]") {
    for (const auto &[a, b]: std::vector<std::pair<bool, bool>>{{0, 0},
                                                                {0, 1},
                                                                {1, 0},
                                                                {1, 1}}) {
        auto toVal = [](bool a) {
            return a ? Circuit::Value::True : Circuit::Value::False;
        };

        Circuit::Value aValue = toVal(a);
        Circuit::Value bValue = toVal(b);

        REQUIRE(Circuit::Functions::AND(a, b) == (a & b));
        REQUIRE(Circuit::Functions::AND(aValue, bValue) == toVal(a & b));

        REQUIRE(Circuit::Functions::OR(a, b) == (a | b));
        REQUIRE(Circuit::Functions::OR(aValue, bValue) == toVal(a | b));

        REQUIRE(Circuit::Functions::NOT(a) == !a);
        REQUIRE(Circuit::Functions::NOT(aValue) == toVal(!a));

        REQUIRE(Circuit::Functions::NAND(a, b) == !(a & b));
        REQUIRE(Circuit::Functions::NAND(aValue, bValue) == toVal(!(a & b)));

        REQUIRE(Circuit::Functions::NOR(a, b) == !(a | b));
        REQUIRE(Circuit::Functions::NOR(aValue, bValue) == toVal(!(a | b)));

        REQUIRE(Circuit::Functions::XOR(a, b) == (a ^ b));
        REQUIRE(Circuit::Functions::XOR(aValue, bValue) == toVal(a ^ b));

        REQUIRE(Circuit::Functions::NXOR(a, b) == !(a ^ b));
        REQUIRE(Circuit::Functions::NXOR(aValue, bValue) == toVal(!(a ^ b)));

        REQUIRE(Circuit::Functions::BUFF(a) == a);
        REQUIRE(Circuit::Functions::BUFF(aValue) == toVal(a));
    }
}