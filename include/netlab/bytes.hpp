#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace netlab {

    std::array<std::uint8_t, 2> encode_u16_be(std::uint16_t value);
    std::uint16_t decode_u16_be(std::span<const std::uint8_t> bytes);
    
}