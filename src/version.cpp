// Copyright 2026 The ndof Authors
// SPDX-License-Identifier: Apache-2.0

#include "ndof/core_utils/version.hpp"

#include <string_view>

namespace ndof::core_utils {

std::string_view library_name() noexcept {
    return NDOF_LIBRARY_NAME;
}

std::string_view library_version() noexcept {
    return NDOF_LIBRARY_VERSION;
}

} // namespace ndof::core_utils
