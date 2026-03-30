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

using ::units::A;
using ::units::Ampere;
using ::units::bar;
using ::units::C;
using ::units::cd;
using ::units::cm;
using ::units::count;
using ::units::currency;
using ::units::deg;
using ::units::degC;
using ::units::degF;
using ::units::foot;
using ::units::ft;
using ::units::g;
using ::units::gal;
using ::units::giga;
using ::units::h;
using ::units::hr;
using ::units::Hz;
using ::units::in;
using ::units::inch;
using ::units::J;
using ::units::K;
using ::units::Kelvin;
using ::units::kg;
using ::units::kilo;
using ::units::kilogram;
using ::units::km;
using ::units::L;
using ::units::lb;
using ::units::m;
using ::units::mega;
using ::units::meter;
using ::units::micro;
using ::units::mile;
using ::units::milli;
using ::units::minute;
using ::units::mm;
using ::units::mol;
using ::units::mph;
using ::units::N;
using ::units::nano;
using ::units::ohm;
using ::units::one;
using ::units::Pa;
using ::units::pico;
using ::units::rad;
using ::units::s;
using ::units::second;
using ::units::V;
using ::units::W;
}  // namespace units
