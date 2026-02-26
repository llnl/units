/*
Copyright (c) 2019-2020,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "test.hpp"
#include "units/units.hpp"

#include <iostream>
#include <string>
#include <tuple>

using unitD = std::tuple<const char*, const char*, units::precise_unit>;
#ifdef ENABLE_UNIT_MAP_ACCESS
// Check the order of the array make sure it is order for searching
TEST(dod, order)
{
    std::size_t unit_count{0};
    const void* dod = units::detail::dodrawData(unit_count);
    const auto* doddata = reinterpret_cast<const unitD*>(dod);
    for (size_t ii = 1; ii < unit_count; ++ii) {
        EXPECT_LT(
            std::string(std::get<0>(doddata[ii - 1])),
            std::string(std::get<0>(doddata[ii])));
    }
}

// check to make sure the name conversions match
TEST(dod, conversions)
{
    std::size_t unit_count{0};
    const void* dod = units::detail::dodrawData(unit_count);
    const auto* doddata = reinterpret_cast<const unitD*>(dod);
    int missed{0};
    int correct{0};
    int matchWithCount{0};
    int matchWithPU{0};
    int matchUnitBase{0};
    int mismatchCommodity{0};
    int commQuantity{0};
    int commQuantity2{0};
    int convertible{0};
    int skipped{0};
    int failedConversion{0};
    int defaulted{0};
    for (size_t ii = 1; ii < unit_count; ++ii) {
        std::string ustr = std::string(std::get<1>(doddata[ii]));
        auto unit = units::measurement_from_string(ustr).as_unit();
        auto udir = units::unit_from_string(ustr);
        auto dodunit = std::get<2>(doddata[ii]);
        if (is_valid(unit)) {
            if (unit != dodunit && udir != dodunit) {
                if (units::unit_cast(unit) == units::unit_cast(dodunit)) {
                    ++mismatchCommodity;
                } else {
                    if (dodunit.commodity() != 0) {
                        ++commQuantity;
                        std::cout << ' ' << ii << " \""
                                  << std::get<0>(doddata[ii]) << "\" " << ustr
                                  << " conversion with commodity doesn't match "
                                  << to_string(unit) << " vs. "
                                  << to_string(dodunit) << '\n';
                    } else if (unit.commodity() != 0) {
                        ++commQuantity2;
                        std::cout << ' ' << ii << " \""
                                  << std::get<0>(doddata[ii]) << "\" " << ustr
                                  << " conversion with commodity doesn't match "
                                  << to_string(unit) << " vs. "
                                  << to_string(dodunit) << '\n';
                    } else if (dodunit.is_per_unit()) {
                        ++matchWithPU;
                        continue;
                    } else if (
                        unit / units::precise::count == dodunit ||
                        unit * units::precise::count == dodunit) {
                        ++matchWithCount;
                        continue;
                    } else if (unit.has_same_base(dodunit)) {
                        ++matchUnitBase;
                    } else if (!std::isnan(units::convert(unit, dodunit))) {
                        ++convertible;
                    } else {
                        EXPECT_EQ(unit, dodunit)
                            << ' ' << ii << " \"" << std::get<0>(doddata[ii])
                            << "\" " << ustr << " conversion does not match "
                            << to_string(unit) << " vs. " << to_string(dodunit);

                        ++missed;
                    }
                }
            } else {
                ++correct;
            }
        } else {
            if (dodunit == units::precise::one / units::precise::count) {
                ++defaulted;
            } else if (dodunit.commodity() == 0) {
                std::cout << "FAILED CONVERSION " << ii << " \""
                          << std::get<0>(doddata[ii]) << "\" " << ustr
                          << " not convertible to " << to_string(dodunit)
                          << '\n';
                ++failedConversion;
            } else {
                std::cout << "SKIPPED " << ii << " \""
                          << std::get<0>(doddata[ii]) << "\" " << ustr
                          << " not convertible to " << to_string(dodunit)
                          << '\n';
                ++skipped;
            }
        }
    }
    std::cout << missed << " dod units not translated properly\n";
    std::cout << mismatchCommodity << " dod units with mismatched commodity\n";
    std::cout << commQuantity << " dod units with commodities not matching\n";
    std::cout << commQuantity2
              << " unit translations with commodities not matching\n";
    std::cout << skipped << " dod units skipped\n";
    std::cout << failedConversion << " dod units with failed conversion\n";
    std::cout << defaulted << " dod units are still on default value\n";
    std::cout << matchWithPU << " dod units matched with pu modifier\n";
    std::cout << matchUnitBase
              << " dod units matched with same base but different number\n";
    std::cout << matchWithCount << " dod units match with a count modifier\n";
    std::cout << convertible << " dod unit are convertible to eachother\n";
    std::cout << correct << " dod units correctly translated\n";
}

#endif

TEST(dod, units)
{
    using namespace units::precise;
    EXPECT_EQ(units::dod_unit("03"), s);

    EXPECT_EQ(
        units::dod_unit("17"),
        units::precise_unit(100.0, lb));

    EXPECT_FALSE(is_valid(units::dod_unit("chaos")));
}