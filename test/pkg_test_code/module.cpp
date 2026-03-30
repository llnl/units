/*
Copyright (c) 2019-2026,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
import units;

#include <iostream>

int main()
{
    auto speed = 10.0 * units::m / units::s;
    auto speed2 = units::measurement_from_string("36 km/hr");

    if (speed2.value_as(units::m / units::s) == 10.0 &&
        speed.units() == (units::m / units::s)) {
        std::cout << "PASS\n";
    } else {
        std::cout << "FAIL\n";
    }
    return 0;
}
