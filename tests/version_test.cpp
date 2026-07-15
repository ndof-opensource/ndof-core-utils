// Copyright 2026 The ndof Authors
// SPDX-License-Identifier: Apache-2.0

#include "ndof/core_utils/version.hpp"

#include <gtest/gtest.h>

namespace {

TEST(Version, LibraryNameMatchesPackage) {
    EXPECT_EQ(ndof::core_utils::library_name(), "ndof-core-utils");
}

TEST(Version, LibraryVersionIsNonEmpty) {
    EXPECT_FALSE(ndof::core_utils::library_version().empty());
}

} // namespace
