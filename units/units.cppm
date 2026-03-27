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
// remain usable. The named module exports a stable namespace alias for
// importers.
export using namespace UNITS_NAMESPACE;
