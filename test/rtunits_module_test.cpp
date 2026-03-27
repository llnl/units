/*
Copyright (c) 2019-2026,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

import units;

int main()
{
    auto volume = 1250.0 * cm.pow(3);
    auto liters = 1.25 * L;
    if (volume != liters) {
        return -1;
    }

    auto parsed = measurement_from_string("10 m/s");
    if (parsed.value_as(km / hr) != 36.0) {
        return -2;
    }

    return 0;
}
