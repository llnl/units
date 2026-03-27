/*
Copyright (c) 2020-2026,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/

import units;

#include <iostream>

int main()
{
    auto speed = 10.0 * m / s;
    auto speed2 = measurement_from_string("36 km/hr");

    if (speed2.value_as(m / s) == 10.0 && speed.units() == (m / s)) {
        std::cout << "PASS\n";
    } else {
        std::cout << "FAIL\n";
    }
    return 0;
}
