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
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using unitD = std::tuple<const char*, const char*, units::precise_unit>;
#ifdef ENABLE_UNIT_MAP_ACCESS
// Check the order of the array make sure it is order for searching
TEST(x12, order)
{
    std::size_t unit_count{0};
    const void* x12 = units::detail::x12rawData(unit_count);
    const auto* x12data = reinterpret_cast<const unitD*>(x12);
    for (size_t ii = 1; ii < unit_count; ++ii) {
        EXPECT_LT(
            std::string(std::get<0>(x12data[ii - 1])),
            std::string(std::get<0>(x12data[ii])));
    }
}

// check to make sure the name conversions match
TEST(x12, conversions)
{
    std::size_t unit_count{0};
    const void* x12 = units::detail::x12rawData(unit_count);
    const auto* x12data = reinterpret_cast<const unitD*>(x12);
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
        std::string ustr = std::string(std::get<1>(x12data[ii]));
        auto unit = units::measurement_from_string(ustr).as_unit();
        auto udir = units::unit_from_string(ustr);
        auto x12unit = std::get<2>(x12data[ii]);
        if (is_valid(unit)) {
            if (unit != x12unit && udir != x12unit) {
                if (units::unit_cast(unit) == units::unit_cast(x12unit)) {
                    ++mismatchCommodity;
                } else {
                    if (x12unit.commodity() != 0) {
                        ++commQuantity;
                        std::cout << ' ' << ii << " \""
                                  << std::get<0>(x12data[ii]) << "\" " << ustr
                                  << " conversion with commodity doesn't match "
                                  << to_string(unit) << " vs. "
                                  << to_string(x12unit) << '\n';
                    } else if (unit.commodity() != 0) {
                        ++commQuantity2;
                        std::cout << ' ' << ii << " \""
                                  << std::get<0>(x12data[ii]) << "\" " << ustr
                                  << " conversion with commodity doesn't match "
                                  << to_string(unit) << " vs. "
                                  << to_string(x12unit) << '\n';
                    } else if (x12unit.is_per_unit()) {
                        ++matchWithPU;
                        continue;
                    } else if (
                        unit / units::precise::count == x12unit ||
                        unit * units::precise::count == x12unit) {
                        ++matchWithCount;
                        continue;
                    } else if (unit.has_same_base(x12unit)) {
                        ++matchUnitBase;
                    } else if (!std::isnan(units::convert(unit, x12unit))) {
                        ++convertible;
                    } else {
                        EXPECT_EQ(unit, x12unit)
                            << ' ' << ii << " \"" << std::get<0>(x12data[ii])
                            << "\" " << ustr << " conversion does not match "
                            << to_string(unit) << " vs. " << to_string(x12unit);

                        ++missed;
                    }
                }
            } else {
                ++correct;
            }
        } else {
            if (x12unit == units::precise::one / units::precise::count) {
                ++defaulted;
            } else if (x12unit.commodity() == 0) {
                std::cout << "FAILED CONVERSION " << ii << " \""
                          << std::get<0>(x12data[ii]) << "\" " << ustr
                          << " not convertible to " << to_string(x12unit)
                          << '\n';
                ++failedConversion;
            } else {
                std::cout << "SKIPPED " << ii << " \""
                          << std::get<0>(x12data[ii]) << "\" " << ustr
                          << " not convertible to " << to_string(x12unit)
                          << '\n';
                ++skipped;
            }
        }
    }
    std::cout << missed << " x12 units not translated properly\n";
    std::cout << mismatchCommodity << " x12 units with mismatched commodity\n";
    std::cout << commQuantity << " x12 units with commodities not matching\n";
    std::cout << commQuantity2
              << " unit translations with commodities not matching\n";
    std::cout << skipped << " x12 units skipped\n";
    std::cout << failedConversion << " x12 units with failed conversion\n";
    std::cout << defaulted << " x12 units are still on default value\n";
    std::cout << matchWithPU << " x12 units matched with pu modifier\n";
    std::cout << matchUnitBase
              << " x12 units matched with same base but different number\n";
    std::cout << matchWithCount << " x12 units match with a count modifier\n";
    std::cout << convertible << " x12 unit are convertible to eachother\n";
    std::cout << correct << " x12 units correctly translated\n";
}

// Load CSV file and verify X12->R20 mappings match
TEST(x12, csv_r20_mapping_verification)
{
    // Load raw data for R20
    std::size_t r20_count{0};
    const void* r20_ptr = units::detail::r20rawData(r20_count);
    const auto* r20data = reinterpret_cast<const unitD*>(r20_ptr);
    
    // Build lookup map for R20 units by code
    std::map<std::string, units::precise_unit> r20_lookup;
    for (size_t i = 0; i < r20_count; ++i) {
        r20_lookup[std::string(std::get<0>(r20data[i]))] = std::get<2>(r20data[i]);
    }
    
    // Open and parse CSV file
    
    std::string csv_path = TEST_FILE_FOLDER "/x12_r20.csv";
    std::ifstream csv_file(csv_path);
    
    if (!csv_file.is_open()) {
        GTEST_SKIP() << "CSV file not found: " << csv_path;
        return;
    }
    
    std::string line;
    int matches{0};
    int mismatches{0};
    
    // Skip header
    std::getline(csv_file, line);
    
    while (std::getline(csv_file, line)) {
        if (line.empty()) continue;
        
        // Parse CSV line (handle quoted fields)
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        bool in_quotes = false;
        std::string current_field;
        
        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            if (c == '"') {
                in_quotes = !in_quotes;
            } else if (c == ',' && !in_quotes) {
                fields.push_back(current_field);
                current_field.clear();
            } else {
                current_field += c;
            }
        }
        fields.push_back(current_field);
        
        if (fields.size() < 3) continue;
        
        // Extract columns (Description, X12, UN Rec 20)
        std::string description = fields[0];
        std::string x12_code = fields[1];
        std::string r20_code = fields[2];
        
        // Trim whitespace
        x12_code.erase(0, x12_code.find_first_not_of(" \t"));
        x12_code.erase(x12_code.find_last_not_of(" \t") + 1);
        r20_code.erase(0, r20_code.find_first_not_of(" \t"));
        r20_code.erase(r20_code.find_last_not_of(" \t") + 1);
        
        if (x12_code.empty() || r20_code.empty()) continue;
        
        // Look up units
        auto x12_unit = units::x12_unit(x12_code);
        auto r20_unit = units::r20_unit(r20_code);
        
        
        // Check if they match or are equivalent
        if (is_valid(x12_unit) && is_valid(r20_unit)) {
            if (x12_unit == r20_unit) {
                ++matches;
            } else if (units::unit_cast(x12_unit) == units::unit_cast(r20_unit)) {
                ++matches;  // Match after removing commodities
            } else if (!std::isnan(units::convert(x12_unit, r20_unit))) {
                ++matches;  // Convertible
            } else {
                ++mismatches;
                std::cout << "Mismatch for X12:" << x12_code << " (R20:" << r20_code
                          << "): " << to_string(x12_unit) << " vs "
                          << to_string(r20_unit) << '\n';
            }
        }
        else if ((!is_valid(x12_unit)) && (!is_valid(r20_unit))) {
         //   std::cout << "Both X12 code " << x12_code << " and R20 code "
          //            << r20_code << " are invalid "<< description<<'\n';
            ++mismatches;

        }
        else if (!is_valid(x12_unit))
        {
            ++mismatches;
           std::cout << "missing X12:" << x12_code << " supposed to match R20:"<<r20_code<<" - "<< description << '\n';
        }
        else if (!is_valid(r20_unit)) {
            ++mismatches;
         //   std::cout << "Invalid R20 unit for code: " << r20_code <<" "<< description << '\n';;
        }
    }
    
    csv_file.close();
    
    std::cout << matches << " X12->R20 mappings verified successfully\n";
    std::cout << mismatches << " X12->R20 mappings failed validation\n";
}

#endif

TEST(x12, units)
{
    using namespace units::precise;
    EXPECT_EQ(units::x12_unit("03"), s);

    EXPECT_EQ(
        units::x12_unit("17"),
        units::precise_unit(100.0, lb));

    EXPECT_FALSE(is_valid(units::x12_unit("chaos")));
}