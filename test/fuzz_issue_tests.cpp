/*
Copyright (c) 2019-2026,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "test.hpp"
#include "units/units.hpp"

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace units;
TEST(fuzzFailures, convFailures)
{
    EXPECT_NO_THROW(unit_from_string("+z264"));
    EXPECT_NO_THROW(unit_from_string("+4560"));
    EXPECT_NO_THROW(unit_from_string("24250v"));
    EXPECT_NO_THROW(unit_from_string("8("));
    EXPECT_NO_THROW(unit_from_string("8{"));
    EXPECT_NO_THROW(unit_from_string("0^%"));
    EXPECT_NO_THROW(unit_from_string("10*1001"));
    EXPECT_NO_THROW(unit_from_string("s)^(z()."));
    EXPECT_NO_THROW(unit_from_string("--+10*+1+110 U"));
    EXPECT_NO_THROW(unit_from_string("EQXUN[{ ["));
}

std::string loadFailureFile(const std::string& type, int index)
{
    std::string fileName(TEST_FILE_FOLDER "/fuzz_issues/");
    fileName.append(type);
    fileName += std::to_string(index);
    std::ifstream crashFile(fileName, std::ios::in | std::ios::binary);
    if (crashFile) {
        std::vector<char> buffer(std::istreambuf_iterator<char>(crashFile), {});

        std::string cdata(buffer.begin(), buffer.end());
        return cdata;
    }
    return std::string{};
}

class crashProblems : public ::testing::TestWithParam<int> {};

TEST_P(crashProblems, crashFiles)
{
    auto cdata = loadFailureFile("crash", GetParam());
    EXPECT_NO_THROW(unit_from_string(cdata));
}

INSTANTIATE_TEST_SUITE_P(crashFiles, crashProblems, ::testing::Range(1, 29));

TEST(fuzzFailures, timeouts)
{  // testing string that have caused a timeout from fuzz testing
    EXPECT_NO_THROW(unit_from_string("3*3*"));
    EXPECT_NO_THROW(unit_from_string("((())"));
}

class timeoutProblems : public ::testing::TestWithParam<int> {};

TEST_P(timeoutProblems, timeoutFiles)
{
    auto cdata = loadFailureFile("timeout", GetParam());
    ASSERT_FALSE(cdata.empty());
    precise_unit val;
    EXPECT_NO_THROW(val = unit_from_string(cdata));
    if (!is_error(val)) {
        auto str = to_string(val);
        auto u2 = unit_from_string(str);
        EXPECT_FALSE(is_error(u2));
    }
}

INSTANTIATE_TEST_SUITE_P(
    timeoutFiles,
    timeoutProblems,
    ::testing::Range(1, 29));

TEST(fuzzFailures, timeoutSingleProblems)
{
    auto cdata = loadFailureFile("timeout", 28);
    ASSERT_FALSE(cdata.empty());
    precise_unit val;
    EXPECT_NO_THROW(val = unit_from_string(cdata));
    if (!is_error(val)) {
        auto str = to_string(val);
        auto u2 = unit_from_string(str);
        EXPECT_FALSE(is_error(u2));
    }
}

class slowProblems : public ::testing::TestWithParam<int> {};

TEST_P(slowProblems, slowFiles)
{
    auto cdata = loadFailureFile("slow", GetParam());
    ASSERT_FALSE(cdata.empty());
    EXPECT_NO_THROW(unit_from_string(cdata));
}

INSTANTIATE_TEST_SUITE_P(slowFiles, slowProblems, ::testing::Range(1, 40));

class oomProblems : public ::testing::TestWithParam<int> {};

TEST_P(oomProblems, oomFiles)
{
    auto cdata = loadFailureFile("oom", GetParam());
    ASSERT_FALSE(cdata.empty());
    EXPECT_NO_THROW(unit_from_string(cdata));
}

INSTANTIATE_TEST_SUITE_P(oomFiles, oomProblems, ::testing::Range(1, 66));

class roundTripString : public ::testing::TestWithParam<std::string> {};

TEST_P(roundTripString, rtripconversions)
{
    auto u1 = unit_from_string(GetParam());
    EXPECT_FALSE(is_error(u1));
    auto str = to_string(u1);
    auto u2 = unit_from_string(str);
    EXPECT_FALSE(is_error(u2));
    EXPECT_EQ(unit_cast(u2), unit_cast(u1));
}
// these are all strings that at one point produced issues
static const std::vector<std::string> testStrings{
    "10*6.-10*6.-",
    "mm-5",
    "D/am",
    "/0j",
    "BQfr",
    "oCoC",
    "1_",
    "Bs1",
    "l-Ym",
    "oCC0",
    "oCGC",
    "(G)1",
    "Km6",
    "{A}999999`",
    "FU7\xb2t",
    "FU7-C\xb2t",
    "A\\-\xb2ps",
    "{inDex}",
    "F{U}{U}",
    "per2rUkUper2U+UK",
    ".1.1.1.1e0.1.NNU",
    "/-3Mh/L",
    "NpmeterUS--3",
    "sqZ+l",
};

INSTANTIATE_TEST_SUITE_P(
    fuzzFailure,
    roundTripString,
    ::testing::ValuesIn(testStrings));

class errorString : public ::testing::TestWithParam<std::string> {};

TEST_P(errorString, conversionErrors)
{
    auto u1 = unit_from_string(GetParam());
    EXPECT_TRUE(is_error(u1));
}
// these are all strings that at one point produced issues
static const std::vector<std::string>
    errorStrings{"Au0m", "br0", "\\\\{U}", "--0-5"};

INSTANTIATE_TEST_SUITE_P(
    fuzzFailure,
    errorString,
    ::testing::ValuesIn(errorStrings));

TEST(fuzzFailures, rtripconversions)
{
    auto u1 = unit_from_string("^");
    EXPECT_FALSE(is_error(u1));
    auto str = to_string(u1);
    EXPECT_FALSE(is_error(unit_from_string(str)));
}

TEST(fuzzFailures, rtripconversions5)
{
    std::string tstring = "ya\xb2";
    tstring += '0';
    auto u1 = unit_from_string(tstring);
    EXPECT_FALSE(is_error(u1));
    auto str = to_string(u1);
    auto u2 = unit_from_string(str);
    EXPECT_FALSE(is_error(u2));
    EXPECT_EQ(u2, u1);
}

TEST(fuzzFailures, rtripconversions6)
{
    std::string tstring = "V\xb2";
    tstring += "+*";
    auto u1 = unit_from_string(tstring);
    EXPECT_TRUE(is_error(u1));
}

TEST(fuzzFailures, rtripconversions12)
{
    std::string tstring = "\\\xbd";
    tstring += "2";
    auto u1 = unit_from_string(tstring);
    EXPECT_FALSE(is_error(u1));
    auto str = to_string(u1);
    auto u2 = unit_from_string(str);
    EXPECT_FALSE(is_error(u2));
    EXPECT_EQ(u2, u1);
}

TEST(fuzzFailures, rtripconversions13)
{
    std::string tstring = "m";
    auto u1 = unit_from_string(tstring);
    EXPECT_FALSE(is_error(u1));
    auto str = to_string(u1);
    auto u2 = unit_from_string(str);
    EXPECT_FALSE(is_error(u2));
    EXPECT_EQ(u2, u1);
}

class rtripProblems : public ::testing::TestWithParam<int> {};

TEST_P(rtripProblems, rtripFiles)
{
    auto cdata = loadFailureFile("rtrip_fail", GetParam());
    auto u1 = unit_from_string(cdata);
    if (GetParam() == 38) {
        std::cerr << "rtrip parametrized input length: " << cdata.size()
                  << "\n";
        std::cerr << "rtrip parametrized input (hex): ";
        for (const auto byte : cdata) {
            std::cerr << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<unsigned int>(
                             static_cast<unsigned char>(byte));
        }
        std::cerr << std::dec << "\n";
        std::cerr << "rtrip parametrized u1 error: " << std::boolalpha
                  << is_error(u1) << ", multiplier: " << u1.multiplier()
                  << "\n";
    }
    if (!is_error(u1)) {
        auto str = to_string(u1);
        auto u2 = unit_from_string(str);
        if (GetParam() == 38) {
            std::cerr << "rtrip parametrized serialized: [" << str << "]\n";
            std::cerr << "rtrip parametrized u2 error: " << is_error(u2)
                          << ", multiplier: " << u2.multiplier()
                          << ", e flag: " << u2.base_units().has_e_flag()
                          << ", equation: " << u2.base_units().is_equation()
                          << "\n";
                std::cerr << "rtrip parametrized u1 flags: e="
                          << u1.base_units().has_e_flag()
                          << ", equation=" << u1.base_units().is_equation()
                          << "\n";
            std::cerr << "rtrip parametrized root1 error: "
                      << is_error(root(u1, 2))
                      << ", e flag: "
                      << root(u1, 2).base_units().has_e_flag() << "\n";
            std::cerr << "rtrip parametrized root2 error: "
                      << is_error(root(u2, 2))
                      << ", e flag: "
                      << root(u2, 2).base_units().has_e_flag() << "\n";
        }
        EXPECT_FALSE(is_error(u2));
        if (u2 == u1) {
            EXPECT_EQ(u2, u1);
            EXPECT_EQ(unit_cast(u2), unit_cast(u1));
            EXPECT_FALSE(units::unit_cast(u2) != units::unit_cast(u1));
        } else if (!is_error(root(u2, 2))) {
            EXPECT_EQ(root(unit_cast(u2), 2), root(unit_cast(u1), 2));
            EXPECT_FALSE(
                root(units::unit_cast(u2), 2) != root(units::unit_cast(u1), 2));
        } else if (!is_error(root(u2, 3))) {
            EXPECT_EQ(root(unit_cast(u2), 3), root(unit_cast(u1), 3));
            EXPECT_FALSE(
                root(units::unit_cast(u2), 3) != root(units::unit_cast(u1), 3));
        } else {
            EXPECT_TRUE(unit_cast(u2) == unit_cast(u1));
            EXPECT_FALSE(units::unit_cast(u2) != units::unit_cast(u1));
        }
    }
}

INSTANTIATE_TEST_SUITE_P(rtripFiles, rtripProblems, ::testing::Range(1, 39));

TEST(fuzzFailures, rtripSingleProblems)
{
    auto cdata = loadFailureFile("rtrip_fail", 38);
    ASSERT_FALSE(cdata.empty());
    auto u1 = unit_from_string(cdata);
    std::cerr << "rtrip input (hex): ";
    for (const auto byte : cdata) {
        std::cerr
            << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(static_cast<unsigned char>(byte));
    }
    std::cerr << std::dec << "\n";
    std::cerr << "input length: " << cdata.size() << "\n";
    std::cerr << "u1 error: " << std::boolalpha << is_error(u1)
              << ", multiplier: " << u1.multiplier() << "\n";
    if (is_error(u1)) {
        std::cerr << "u1 was rejected; no round-trip values available\n";
    } else {
        auto str = to_string(u1);
        auto u2 = unit_from_string(str);
        auto preciseRoot1 = root(u1, 2);
        auto preciseRoot2 = root(u2, 2);
        auto root1 = root(unit_cast(u1), 2);
        auto root2 = root(unit_cast(u2), 2);
        const auto printUnitData = [](const char* name, const precise_unit& unit) {
            const auto base = unit.base_units();
            std::cerr << name << " base: m=" << base.meter()
                      << ", kg=" << base.kg() << ", s=" << base.second()
                      << ", A=" << base.ampere() << ", K=" << base.kelvin()
                      << ", mol=" << base.mole() << ", cd=" << base.candela()
                      << ", rad=" << base.radian()
                      << ", currency=" << base.currency()
                      << ", count=" << base.count()
                      << ", i=" << base.has_i_flag()
                      << ", e=" << base.has_e_flag()
                      << ", equation=" << base.is_equation() << "\n";
        };
        std::cerr << "serialized: [" << str << "]\n";
        std::cerr << "u2 error: " << is_error(u2)
                  << ", multiplier: " << u2.multiplier()
                  << ", e flag: " << u2.base_units().has_e_flag()
                  << ", equation: " << u2.base_units().is_equation() << "\n";
        std::cerr << "u1 flags: e=" << u1.base_units().has_e_flag()
                  << ", equation=" << u1.base_units().is_equation() << "\n";
        printUnitData("u1", u1);
        printUnitData("u2", u2);
        printUnitData("precise root1", preciseRoot1);
        printUnitData("precise root2", preciseRoot2);
        std::cerr << "precise root1 error: " << is_error(preciseRoot1)
                  << ", multiplier: " << preciseRoot1.multiplier()
                  << ", e flag: " << preciseRoot1.base_units().has_e_flag()
                  << "\n";
        std::cerr << "precise root2 error: " << is_error(preciseRoot2)
                  << ", multiplier: " << preciseRoot2.multiplier()
                  << ", e flag: " << preciseRoot2.base_units().has_e_flag()
                  << "\n";
        std::cerr << "root1 error: " << is_error(root1)
                  << ", multiplier: " << root1.multiplier()
                  << ", e flag: " << root1.base_units().has_e_flag() << "\n";
        std::cerr << "root2 error: " << is_error(root2)
                  << ", multiplier: " << root2.multiplier()
                  << ", e flag: " << root2.base_units().has_e_flag() << "\n";
        EXPECT_FALSE(is_error(u2));
        if (u2 == u1) {
            EXPECT_EQ(u2, u1);
            EXPECT_EQ(unit_cast(u2), unit_cast(u1));
            EXPECT_FALSE(units::unit_cast(u2) != units::unit_cast(u1));
        } else if (!is_error(root(u2, 2))) {
            std::cerr << "selected branch: square root\n";
            EXPECT_EQ(root(unit_cast(u2), 2), root(unit_cast(u1), 2));
            EXPECT_FALSE(
                root(units::unit_cast(u2), 2) != root(units::unit_cast(u1), 2));
        } else if (!is_error(root(u2, 3))) {
            std::cerr << "selected branch: cube root\n";
            EXPECT_EQ(root(unit_cast(u2), 3), root(unit_cast(u1), 3));
            EXPECT_FALSE(
                root(units::unit_cast(u2), 3) != root(units::unit_cast(u1), 3));
        } else {
            std::cerr << "selected branch: direct unit comparison\n";
            auto uc1 = unit_cast(u1);
            auto uc2 = unit_cast(u2);
            EXPECT_EQ(uc2, uc1);
            EXPECT_FALSE(uc2 != uc1);
        }
    }
}

class rtripflagProblems : public ::testing::TestWithParam<int> {};

TEST_P(rtripflagProblems, rtripflagFiles)
{
    auto cdata = loadFailureFile("rtrip_flag", GetParam());
    if (cdata.size() <= 4) {
        return;
    }
    std::string test1 = cdata.substr(4);
    std::uint32_t flags;
    std::memcpy(&flags, cdata.data(), 4);
    auto u1 = unit_from_string(test1, flags);
    if (!is_error(u1)) {
        auto str = to_string(u1);
        auto u2 = unit_from_string(str);
        EXPECT_FALSE(is_error(u2));
        if (u2 == u1) {
            EXPECT_EQ(u2, u1);
            EXPECT_EQ(unit_cast(u2), unit_cast(u1));
            EXPECT_FALSE(units::unit_cast(u2) != units::unit_cast(u1));
        } else if (!is_error(root(u2, 2))) {
            EXPECT_EQ(root(unit_cast(u2), 2), root(unit_cast(u1), 2));
            EXPECT_FALSE(
                root(units::unit_cast(u2), 2) != root(units::unit_cast(u1), 2));
        } else if (!is_error(root(u2, 3))) {
            EXPECT_EQ(root(unit_cast(u2), 3), root(unit_cast(u1), 3));
            EXPECT_FALSE(
                root(units::unit_cast(u2), 3) != root(units::unit_cast(u1), 3));
        } else {
            EXPECT_TRUE(unit_cast(u2) == unit_cast(u1));
            EXPECT_FALSE(units::unit_cast(u2) != units::unit_cast(u1));
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
    rtripflagFiles,
    rtripflagProblems,
    ::testing::Range(1, 7));

class measProblems : public ::testing::TestWithParam<int> {};

TEST_P(measProblems, measFiles)
{
    auto cdata = loadFailureFile("meas_fail", GetParam());
    auto m1 = measurement_from_string(cdata);
    if (isnormal(m1)) {
        auto str = to_string(m1);
        auto m2 = measurement_from_string(str);
        ASSERT_TRUE(m2.units().has_same_base(m1.units()) || isnormal(m2));
        auto mc2 = measurement_cast(m2);
        auto mc1 = measurement_cast(m1);
        if (m2 == m1) {
            EXPECT_EQ(m2, m1);
            EXPECT_FALSE(m2 != m1);
        } else if (isnormal(root(m2, 2))) {
            EXPECT_EQ(root(mc2, 2), root(mc1, 2));
            EXPECT_FALSE(root(mc2, 2) != root(mc1, 2));
        } else if (isnormal(root(m2, 3))) {
            EXPECT_EQ(root(mc2, 3), root(mc1, 3));
            EXPECT_FALSE(root(mc2, 3) != root(mc1, 3));
        } else if (isnormal(mc1) && isnormal(mc2)) {
            EXPECT_TRUE(mc2 == mc1);
            EXPECT_FALSE(mc2 != mc1);
        } else {
            auto uc1 = unit_cast(m1.as_unit());
            auto uc2 = unit_cast(m2.as_unit());
            EXPECT_TRUE(uc2 == uc1);
            EXPECT_FALSE(uc2 != uc1);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(measFiles, measProblems, ::testing::Range(0, 33));

class meas64Problems : public ::testing::TestWithParam<int> {};

TEST_P(meas64Problems, measFiles)
{
    auto cdata = loadFailureFile("meas64_fail", GetParam());
    auto m1 = measurement_from_string(cdata);
    if (isnormal(m1)) {
        auto str = to_string(m1);
        auto m2 = measurement_from_string(str);
        ASSERT_TRUE(m2.units().has_same_base(m1.units()) || isnormal(m2));
        auto mc2 = measurement_cast(m2);
        auto mc1 = measurement_cast(m1);
        if (m2 == m1) {
            EXPECT_EQ(m2, m1);
            EXPECT_FALSE(m2 != m1);
        } else if (isnormal(root(m2, 2))) {
            EXPECT_EQ(root(mc2, 2), root(mc1, 2));
            EXPECT_FALSE(root(mc2, 2) != root(mc1, 2));
        } else if (isnormal(root(m2, 3))) {
            EXPECT_EQ(root(mc2, 3), root(mc1, 3));
            EXPECT_FALSE(root(mc2, 3) != root(mc1, 3));
        } else if (isnormal(mc1) && isnormal(mc2)) {
            EXPECT_TRUE(mc2 == mc1);
            EXPECT_FALSE(mc2 != mc1);
        } else {
            auto uc1 = unit_cast(m1.as_unit());
            auto uc2 = unit_cast(m2.as_unit());
            EXPECT_TRUE(uc2 == uc1);
            EXPECT_FALSE(uc2 != uc1);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(meas64Files, meas64Problems, ::testing::Range(0, 6));
