#pragma once

#include <cstdint>
#include <span>

namespace netlab{
    std::uint16_t compute_checksum (std::span<const std::uint8_t> bytes);
}