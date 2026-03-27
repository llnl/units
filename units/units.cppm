/*
Copyright (c) 2019-2026,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

module;
#include "units/units.hpp"

export module units;

// The implementation stays in the global module fragment so legacy headers
// remain usable. The named module re-exports a curated public surface in the
// expected namespace for module consumers.
export namespace units {
using ::units::addCustomCommodity;
using ::units::addUserDefinedInputUnit;
using ::units::addUserDefinedOutputUnit;
using ::units::addUserDefinedUnit;
using ::units::clearCustomCommodities;
using ::units::clearUserDefinedUnits;
using ::units::convert;
using ::units::default_unit;
using ::units::definedUnitsFromFile;
using ::units::dimensions;
using ::units::disableCustomCommodities;
using ::units::disableUserDefinedUnits;
using ::units::enableCustomCommodities;
using ::units::enableUserDefinedUnits;
using ::units::fixed_measurement;
using ::units::fixed_precise_measurement;
using ::units::getCommodity;
using ::units::getCommodityName;
using ::units::measurement;
using ::units::measurement_cast_from_string;
using ::units::measurement_from_string;
using ::units::precise_measurement;
using ::units::precise_unit;
using ::units::quick_convert;
using ::units::removeUserDefinedUnit;
using ::units::root;
using ::units::sqrt;
using ::units::to_string;
using ::units::uncertain_measurement;
using ::units::uncertain_measurement_from_string;
using ::units::unit;
using ::units::unit_cast;
using ::units::unit_cast_from_string;
using ::units::unit_from_string;
using ::units::operator*;
using ::units::operator/;

namespace domains = ::units::domains;
namespace precise = ::units::precise;

namespace constants {
    inline constexpr auto pi = ::units::constants::pi;
    inline constexpr auto tau = ::units::constants::tau;
    inline constexpr auto invalid_conversion =
        ::units::constants::invalid_conversion;
    inline constexpr auto infinity = ::units::constants::infinity;
    inline constexpr auto standard_gravity =
        ::units::constants::standard_gravity;
    inline constexpr auto speed_of_light = ::units::constants::speed_of_light;
}  // namespace constants

inline constexpr auto A = ::units::A;
inline constexpr auto C = ::units::C;
inline constexpr auto Hz = ::units::Hz;
inline constexpr auto J = ::units::J;
inline constexpr auto K = ::units::K;
inline constexpr auto L = ::units::L;
inline constexpr auto N = ::units::N;
inline constexpr auto V = ::units::V;
inline constexpr auto W = ::units::W;
inline constexpr auto cm = ::units::cm;
inline constexpr auto ft = ::units::ft;
inline constexpr auto g = ::units::g;
inline constexpr auto h = ::units::h;
inline constexpr auto hr = ::units::hr;
inline constexpr auto kg = ::units::kg;
inline constexpr auto km = ::units::km;
inline constexpr auto m = ::units::m;
inline constexpr auto min = ::units::min;
inline constexpr auto mm = ::units::mm;
inline constexpr auto mol = ::units::mol;
inline constexpr auto ohm = ::units::ohm;
inline constexpr auto one = ::units::one;
inline constexpr auto rad = ::units::rad;
inline constexpr auto s = ::units::s;
inline constexpr auto sec = ::units::s;
}  // namespace units
