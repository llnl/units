/*
Copyright (c) 2019-2026,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include UNITS_LIB_HEADER

#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    auto u1 = units::measurement_from_string("10.7 meters per second");

    std::cout << to_string(u1) << std::endl;
    return 0;
}
